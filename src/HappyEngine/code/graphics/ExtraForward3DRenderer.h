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
//Author:  Sebastiaan Sprengers
//Created: 06/11/2011

#ifndef _HE_SIMPLE_FORWARD_3D_RENDERER_H_
#define _HE_SIMPLE_FORWARD_3D_RENDERER_H_
#pragma once

#include "ModelMesh.h"
#include "mat44.h"
#include "Color.h"
#include "SimpleColorEffect.h"
#include "Camera.h"
#include "Texture2D.h"
#include "vec3.h"
#include "BillboardEffect.h"

namespace he {
namespace gfx {

class ExtraForward3DRenderer
{
public:

    /* CONSTRUCTOR - DESCTRUCTOR */
    ExtraForward3DRenderer();
    virtual ~ExtraForward3DRenderer();

    /* GENERAL */
    void init();
    void begin(const Camera* pCamera);
    void end();
    void resize();

    /* DRAW METHODS */
    void drawColored(const ModelMesh::pointer& model, const mat44& world, const Color& color) const;
    void drawColoredNoDepth(const ModelMesh::pointer& model, const mat44& world, const Color& color) const;
    void drawSpline(const ModelMesh::pointer& spline, const mat44& world, const Color& color) const;
    void drawBillboard(const Texture2D::pointer& tex2D, const vec3& pos);

private:

    void createBillboardQuad();

    /* DATAMEMBERS */
    SimpleColorEffect* m_pColorEffect;

    mat44 m_ViewProjection;
    mat44 m_BillboardMatrix;

    uint m_RenderFboID;
    Texture2D::pointer m_pRenderTexture;

    ModelMesh::pointer m_pBillboardQuad;
    BufferLayout m_VertexLayoutBillboard;
    BillboardEffect* m_pBillboardEffect;

    vec2 m_ScreenDimensions;

    const Camera* m_pCamera;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    ExtraForward3DRenderer(const ExtraForward3DRenderer&);
    ExtraForward3DRenderer& operator=(const ExtraForward3DRenderer&);
};

} } //end namespace

#endif