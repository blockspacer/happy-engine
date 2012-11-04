//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
//
//This file is part of HappyEngine.
//
//    HappyEngine is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyEngine is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyEngine.  If not, see <http://www.gnu.org/licenses/>.
//
//Author: Bastian Damman
//Created: 28/08/2011
#include "HappyPCH.h" 

#include "BinObjLoader.h"

#include "BinaryStream.h"
#include "BufferLayout.h"
#include "Model.h"
#include "Bone.h"
#include "ModelMesh.h"

namespace he {
namespace ct {
namespace models {

BinObjLoader::BinObjLoader()
{
}


BinObjLoader::~BinObjLoader()
{
    m_BoneData.forEach([&](he::ObjectList<gfx::Bone>* bones)
    {
        delete bones;
    });
    m_VertexData.forEach([&](he::ObjectList<InternalVertex>* vertexData)
    {
        delete vertexData;
    });
    m_Vertices.forEach([&](void* pVert)
    {
        he_free(pVert);
    });
    m_Indices.forEach([&](void* pInd)
    {
        he_free(pInd);
    });
}
bool BinObjLoader::load(const std::string& path, const gfx::BufferLayout& vertLayout, bool allowByteIndices)
{
    if (read(path, allowByteIndices) == false)
        return false;

    m_Vertices.forEach([&](void* pVert)
    {
        he_free(pVert);
    });
    m_Vertices.clear();

    for (uint32 i = 0; i < m_VertexData.size(); ++i)
    {
        void* pVert(he_malloc(vertLayout.getSize() * m_VertexData[i]->size()));
        HE_ASSERT(pVert != nullptr, "not enough memory!");
        m_Vertices.add(pVert);
    }
    
    fill(vertLayout);

    return true;
}

bool BinObjLoader::read(const std::string& path, bool allowByteIndices)
{
    //Clean
    std::for_each(m_Indices.begin(), m_Indices.end(), [&](void* pInd)
    {
        he_free(pInd);
    });
    m_Indices.clear();
    m_BoneData.forEach([&](he::ObjectList<gfx::Bone>* bones)
    {
        delete bones;
    });
    m_BoneData.clear();
    m_VertexData.forEach([&](he::ObjectList<InternalVertex>* vertexData)
    {
        delete vertexData;
    });
    m_VertexData.clear();

    using namespace std;

    io::BinaryStream stream;
    if (stream.open(path, io::BinaryStream::Read) == false)
        return false;
    
    uint32 meshes(stream.readDword());

    for(uint32 i = 0; i < meshes; ++i)
    {
        m_MeshName.add(stream.readString());

        //////////////////////////////////////////////////////////////////////////
        ///                             Bones                                  ///
        //////////////////////////////////////////////////////////////////////////
        uint32 numBones(stream.readByte());
        m_BoneData.add(NEW he::ObjectList<gfx::Bone>());
        m_BoneData.back()->reserve(numBones);
        for (uint32 iBone = 0; iBone < numBones; ++iBone)
        {
            gfx::Bone bone;
            bone.m_Name = stream.readString();
            bone.m_BaseTransform = stream.readMatrix();
            m_BoneData.back()->add(bone);
        }

        //////////////////////////////////////////////////////////////////////////
        ///                             Vertices                               ///
        //////////////////////////////////////////////////////////////////////////
        uint32 numVertices(stream.readDword());
        m_VertexData.add(NEW he::ObjectList<InternalVertex>((size_t)numVertices));
        m_VertexData.back()->resize(numVertices);
        stream.read(&(m_VertexData.back()->front()), numVertices * sizeof(InternalVertex));

        //////////////////////////////////////////////////////////////////////////
        ///                             Indices                                ///
        //////////////////////////////////////////////////////////////////////////
        m_NumIndices.add(stream.readDword());
        gfx::IndexStride stride(static_cast<gfx::IndexStride>(stream.readByte()));
        if (stride == gfx::IndexStride_Byte && allowByteIndices == false)
            m_IndexStride.add(gfx::IndexStride_UShort);
        else
            m_IndexStride.add(stride);
        
        void* pInd = he_malloc(stride * m_NumIndices.back());
        HE_ASSERT(pInd != nullptr, "not enough memory!");
        stream.read(pInd, stride * m_NumIndices.back());
        m_Indices.add(pInd);
    }
    return true;
}

void BinObjLoader::fill(const gfx::BufferLayout& vertLayout) const
{
    int pOff = -1;
    int tOff = -1;
    int nOff = -1;
    int tanOff = -1;
    int boneOff = -1;
    int weightOff = -1;

    std::for_each(vertLayout.getElements().cbegin(), vertLayout.getElements().cend(), [&](const gfx::BufferElement& element)
    {
        if (element.getUsage() == gfx::BufferElement::Usage_Position)
            pOff = element.getByteOffset();
        else if (element.getUsage() == gfx::BufferElement::Usage_TextureCoordinate)
            tOff = element.getByteOffset(); 
        else if (element.getUsage() == gfx::BufferElement::Usage_Normal)
            nOff = element.getByteOffset();
        else if (element.getUsage() == gfx::BufferElement::Usage_Tangent)
            tanOff = element.getByteOffset();
        else if (element.getUsage() == gfx::BufferElement::Usage_BoneIDs)
            boneOff = element.getByteOffset();
        else if (element.getUsage() == gfx::BufferElement::Usage_BoneWeights)
            weightOff = element.getByteOffset();
    });
    
    for (uint32 i = 0; i < m_VertexData.size(); ++i)
    {
        //optimazation for struct == internal struct
        if (sizeof(InternalVertex) == vertLayout.getSize())
        {
            if (pOff == 0 && tOff == 12 && nOff == 20 && tanOff == 32 && boneOff == 44 && weightOff == 44 + gfx::Bone::MAX_BONES * 1)
            {
                he_memcpy(m_Vertices[i], &m_VertexData[i]->front(), m_VertexData[i]->size() * vertLayout.getSize());
                break;
            }
        } 

        char* pCharData = static_cast<char*>(m_Vertices[i]);
        uint32 count(0);
        m_VertexData[i]->forEach([&](const InternalVertex& vert)
        {
            if (pOff != -1)
            {
                he_memcpy(&pCharData[count * vertLayout.getSize() + pOff], &vert.pos, sizeof(vec3));
            }
            if (tOff != -1)
            {
                he_memcpy(&pCharData[count * vertLayout.getSize() + tOff], &vert.tex, sizeof(vec2));
            }
            if (nOff != -1)
            {
                he_memcpy(&pCharData[count * vertLayout.getSize() + nOff], &vert.norm, sizeof(vec3));
            }
            if (tanOff != -1)
            {
                he_memcpy(&pCharData[count * vertLayout.getSize() + tanOff], &vert.tan, sizeof(vec3));
            }
            if (boneOff != -1)
            {
                HE_ASSERT(gfx::Bone::MAX_BONEWEIGHTS == 4, "Unsupported max boneWeight value only 4 is supported");
                vec4 boneIDs(vert.boneID[0], vert.boneID[1], vert.boneID[2], vert.boneID[3]);
                he_memcpy(&pCharData[count * vertLayout.getSize() + boneOff], &boneIDs, sizeof(vec4));
            }
            if (weightOff != -1)
            {
                he_memcpy(&pCharData[count * vertLayout.getSize() + weightOff], vert.boneWeight, sizeof(float) * gfx::Bone::MAX_BONEWEIGHTS);
            }
            ++count;
        });
    }
}

const void* BinObjLoader::getVertices(uint32 mesh) const
{
    return m_Vertices[mesh];
}
const void* BinObjLoader::getIndices(uint32 mesh) const 
{
    return m_Indices[mesh];
}
gfx::IndexStride BinObjLoader::getIndexStride(uint32 mesh) const
{
    return m_IndexStride[mesh];
}
uint32 BinObjLoader::getNumVertices(uint32 mesh) const
{
    return m_VertexData[mesh]->size();
}
uint32 BinObjLoader::getNumIndices(uint32 mesh) const
{
    return m_NumIndices[mesh];
}

uint32 BinObjLoader::getNumMeshes() const
{
    return m_MeshName.size();
}
const std::string& BinObjLoader::getMeshName(uint32 mesh) const
{
    return m_MeshName[mesh];
}

const he::ObjectList<gfx::Bone>& BinObjLoader::getBones( uint32 mesh ) const
{
    HE_ASSERT(mesh < m_BoneData.size(), "mesh outside array");
    return *m_BoneData[mesh];
}

} } } //end namespace