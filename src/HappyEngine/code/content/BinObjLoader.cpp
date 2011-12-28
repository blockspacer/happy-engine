//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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

#include <algorithm>
#include <iostream>
#include <sstream>

#include "BinaryStream.h"
#include "FileNotFoundException.h"

#include "HeAssert.h"

namespace he {
namespace ct {
namespace models {

BinObjLoader::BinObjLoader(): m_NumIndices(0)
{
}


BinObjLoader::~BinObjLoader()
{
    std::for_each(m_Vertices.begin(), m_Vertices.end(), [&](void* pVert)
    {
        free(pVert);
    });
    std::for_each(m_Indices.begin(), m_Indices.end(), [&](void* pInd)
    {
        free(pInd);
    });
}
void BinObjLoader::load(const std::string& path, const gfx::BufferLayout& vertLayout, bool allowByteIndices)
{
    read(path, allowByteIndices);
    
    std::for_each(m_Vertices.begin(), m_Vertices.end(), [&](void* pVert)
    {
        free(pVert);
    });
    m_Vertices.clear();

    for (uint i = 0; i < m_VertexData.size(); ++i)
    {
        void* pVert(malloc(vertLayout.getSize() * m_VertexData[i].size()));
        ASSERT(pVert != nullptr, "not enough memory!");
        m_Vertices.push_back(pVert);
    }
    
    fill(vertLayout);
}

void BinObjLoader::read(const std::string& path, bool allowByteIndices)
{
    //Clean
    std::for_each(m_Indices.begin(), m_Indices.end(), [&](void* pInd)
    {
        free(pInd);
    });
    m_Indices.clear();
    m_VertexData.clear();

    using namespace std;

    io::BinaryStream stream(path, io::BinaryStream::Read); //throws err::FileNotFoundException
    
    uint meshes(stream.readDword());

    for(uint i = 0; i < meshes; ++i)
    {
        m_MeshName.push_back(stream.readString());

        //////////////////////////////////////////////////////////////////////////
        ///                             Bones                                  ///
        //////////////////////////////////////////////////////////////////////////
        uint numBones(stream.readByte());
        m_BoneData.push_back(std::vector<gfx::Bone>());
        m_BoneData.back().reserve(numBones);
        for (uint iBone = 0; iBone < numBones; ++iBone)
        {
            gfx::Bone bone;
            bone.m_Name = stream.readString();
            bone.m_BaseTransform = stream.readMatrix();
            m_BoneData.back().push_back(bone);
        }

        //////////////////////////////////////////////////////////////////////////
        ///                             Vertices                               ///
        //////////////////////////////////////////////////////////////////////////
        uint numVertices(stream.readDword());
        m_VertexData.push_back(std::vector<InternalVertex>(numVertices));
        stream.readBuffer(&m_VertexData.back()[0], numVertices * sizeof(InternalVertex));

        //////////////////////////////////////////////////////////////////////////
        ///                             Indices                                ///
        //////////////////////////////////////////////////////////////////////////
        m_NumIndices.push_back(stream.readDword());
        gfx::IndexStride stride(static_cast<gfx::IndexStride>(stream.readByte()));
        if (stride == gfx::IndexStride_Byte && allowByteIndices == false)
            m_IndexStride.push_back(gfx::IndexStride_UShort);
        else
            m_IndexStride.push_back(stride);
        
        void* pInd = malloc(stride * m_NumIndices.back());
        ASSERT(pInd != nullptr, "not enough memory!");
        stream.readBuffer(pInd, stride * m_NumIndices.back());
        m_Indices.push_back(pInd);
    }
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
    
    for (uint i = 0; i < m_VertexData.size(); ++i)
    {
        //optimazation for struct == internal struct
        if (sizeof(InternalVertex) == vertLayout.getSize())
        {
            if (pOff == 0 && tOff == 12 && nOff == 20 && tanOff == 32 && boneOff == 44 && weightOff == 44 + gfx::Bone::MAX_BONES * 1)
            {
                memcpy(m_Vertices[i], &m_VertexData[i][0], m_VertexData[i].size() * vertLayout.getSize());
                break;
            }
        } 

        char* pCharData = static_cast<char*>(m_Vertices[i]);
        uint count(0);
        std::for_each(m_VertexData[i].cbegin(), m_VertexData[i].cend(), [&](const InternalVertex& vert)
        {
            if (pOff != -1)
            {
                memcpy(&pCharData[count * vertLayout.getSize() + pOff], &vert.pos, sizeof(vec3));
            }
            if (tOff != -1)
            {
                memcpy(&pCharData[count * vertLayout.getSize() + tOff], &vert.tex, sizeof(vec2));
            }
            if (nOff != -1)
            {
                memcpy(&pCharData[count * vertLayout.getSize() + nOff], &vert.norm, sizeof(vec3));
            }
            if (tanOff != -1)
            {
                memcpy(&pCharData[count * vertLayout.getSize() + tanOff], &vert.tan, sizeof(vec3));
            }
            if (boneOff != -1)
            {
                ASSERT(gfx::Bone::MAX_BONEWEIGHTS == 4, "Unsupported max boneWeight value only 4 is supported");
                vec4 boneIDs(vert.boneID[0], vert.boneID[1], vert.boneID[2], vert.boneID[3]);
                memcpy(&pCharData[count * vertLayout.getSize() + boneOff], &boneIDs, sizeof(vec4));
            }
            if (weightOff != -1)
            {
                memcpy(&pCharData[count * vertLayout.getSize() + weightOff], vert.boneWeight, sizeof(float) * gfx::Bone::MAX_BONEWEIGHTS);
            }
            ++count;
        });
    }
}

const void* BinObjLoader::getVertices(uint mesh) const
{
    return m_Vertices[mesh];
}
const void* BinObjLoader::getIndices(uint mesh) const 
{
    return m_Indices[mesh];
}
gfx::IndexStride BinObjLoader::getIndexStride(uint mesh) const
{
    return m_IndexStride[mesh];
}
uint BinObjLoader::getNumVertices(uint mesh) const
{
    return m_VertexData[mesh].size();
}
uint BinObjLoader::getNumIndices(uint mesh) const
{
    return m_NumIndices[mesh];
}

uint BinObjLoader::getNumMeshes() const
{
    return m_MeshName.size();
}
const std::string& BinObjLoader::getMeshName(uint mesh) const
{
    return m_MeshName[mesh];
}

const std::vector<gfx::Bone>& BinObjLoader::getBones( uint mesh ) const
{
    ASSERT(mesh < m_BoneData.size(), "mesh outside array");
    return m_BoneData[mesh];
}

} } } //end namespace