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
//Author:  Bastian Damman
//Created: 15/09/2011 (copy of the previous Model class)

#ifndef _HE_MODELMESH_H_
#define _HE_MODELMESH_H_
#pragma once

#include "BufferLayout.h"
#include "Bone.h"

#include "Resource.h"
#include "Bound.h"

namespace he {
namespace gfx {

ENUM(IndexStride, uint8)
{
    IndexStride_Byte = sizeof(uint8),
    IndexStride_UShort = sizeof(uint16),
    IndexStride_UInt = sizeof(uint32)
};

enum MeshUsage
{
    MeshUsage_Static  =  GL_STATIC_DRAW,    // Update rarely to never
    MeshUsage_Stream  =  GL_STREAM_DRAW,    // Update frequently
    MeshUsage_Dynamic =  GL_DYNAMIC_DRAW    // Update every frame
};


enum MeshDrawMode
{
    MeshDrawMode_Points     =   GL_POINTS,
    MeshDrawMode_Lines      =   GL_LINES,
    MeshDrawMode_Triangles  =   GL_TRIANGLES
};

struct PickingData
{
    PickingData() : m_Vertices(nullptr), m_Indices(nullptr), m_IndexStride(IndexStride_Byte), m_TriangleCount(0) {}
    vec3* m_Vertices;
    void* m_Indices;
    IndexStride m_IndexStride;
    size_t m_TriangleCount;
};

class ModelMesh : public Resource<ModelMesh>
{
public:
    ModelMesh();
    virtual ~ModelMesh();

    void init(const BufferLayout& vertexLayout, MeshDrawMode mode);
    void setVertices(const void* const vertices, const uint32 num, const MeshUsage usage, const bool calcBound);
    void setIndices(const void* const indices, const uint32 num, const IndexStride type, const MeshUsage usage);
    void setBones(const he::ObjectList<Bone>& boneList);
    void setLoaded();

    // Getters
    inline const he::ObjectList<Bone>& getBones() const { return m_BoneList; }

    inline VaoID getVertexArraysID() const { return m_VaoID[GL::s_CurrentContext->id]; } 
    inline VaoID getVertexShadowArraysID() const { return m_VaoShadowID[GL::s_CurrentContext->id]; }
    inline uint32 getVBOID() const { return m_VertexVboID; }
    inline uint32 getVBOIndexID() const { return m_IndexVboID; }
    inline const MeshDrawMode& getDrawMode() const { return m_DrawMode; }

    void createPickingData(const void* const vertices, const size_t vertexCount, const BufferLayout& vertexLayout, const void* const indices, const size_t indexCount, const IndexStride indexStride);
    void destroyPickingData();
    const PickingData& getPickingData() const { return m_PickingData; }
    bool hasPickingData() const { return m_PickingData.m_Vertices != nullptr; }

    inline uint32 getNumVertices() const { return m_NumVertices; }
    inline uint32 getNumIndices() const { return m_NumIndices; }

    inline uint32 getIndexType() const { return m_IndexType; }
    inline const BufferLayout& getVertexLayout() const { return m_VertexLayout; }

    inline bool isLoaded() const { return m_IsLoaded; }

    inline const Bound& getBound() const { return m_Bound; }

    // Events
    void callbackOnceIfLoaded(const boost::function<void()>& callback);

private:
    void initVAO(GLContext* context);
    void destroyVAO(GLContext* context);    

    struct ShadowSkinnedVertex
    {
        vec3 pos;
        vec4 boneId;
        vec4 boneWeight;
    };


    event0<void> Loaded;
    boost::mutex m_LoadMutex;

    VaoID m_VaoID[MAX_VERTEX_ARRAY_OBJECTS];
    VaoID m_VaoShadowID[MAX_VERTEX_ARRAY_OBJECTS];
    uint32 m_VertexVboID;
    uint32 m_IndexVboID;

    uint32 m_NumVertices;
    uint32 m_NumIndices;

    BufferLayout m_VertexLayout;
    uint32 m_IndexType;

    bool m_isVisible;
    bool m_IsLoaded;

    Bound m_Bound;
    PickingData m_PickingData;

    he::ObjectList<Bone> m_BoneList;

    MeshDrawMode m_DrawMode;

    eventCallback1<void, GLContext*> m_ContextCreatedHandler;
    eventCallback1<void, GLContext*> m_ContextRemovedHandler;

    //Disable default copy constructor and default assignment operator
    ModelMesh(const ModelMesh&);
    ModelMesh& operator=(const ModelMesh&);
};

typedef ResourceFactory<ModelMesh> ModelMeshFactory;

} } //end namespace

#endif
