//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
//Author:  Sebastiaan Sprengers
//Created: 25/04/2012

#ifndef _HE_MESH2D_H_
#define _HE_MESH2D_H_
#pragma once

namespace he {
class Polygon;
namespace gfx {

class HAPPY_ENTRY Mesh2D
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Mesh2D(bool staticDraw = false);
    virtual ~Mesh2D();

    /* GENERAL */
    void addVertex(const vec2& point);
    void clear();

    bool triangulate();

    void createBuffer(bool outline = false);

    /* GETTERS */
    uint32 getBufferID() const;
    const mat44& getWorldMatrix() const;
    const he::PrimitiveList<vec2>& getVertices() const;
    const he::PrimitiveList<uint32>& getIndices() const;
    bool hasBuffer() const { return m_HasBuffer; }

    /* SETTERS */
    void setWorldMatrix(const mat44& mat);

private:

    /* INTERNAL */
    void initVao(GLContext* context);
    void destroyVao(GLContext* context);

    /* DATAMEMBERS */
    Polygon* m_Polygon;
    mat44 m_WorldMatrix;
    uint32 m_VBOID;
    uint32 m_IBOID;
    VaoID m_VAOID[MAX_VERTEX_ARRAY_OBJECTS];

    eventCallback1<void, GLContext*> m_ContextCreatedHandler;
    eventCallback1<void, GLContext*> m_ContextRemovedHandler;

    bool m_StaticDraw;
    bool m_HasBuffer;

    /* DEFAULT COPY & ASSIGNMENT */
    Mesh2D(const Mesh2D&);
    Mesh2D& operator=(const Mesh2D&);
};

} } //end namespace

#endif
