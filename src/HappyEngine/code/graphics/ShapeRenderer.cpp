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

#include "HappyPCH.h" 

#include "ShapeRenderer.h"

#include "GraphicsEngine.h"
#include "Renderer2D.h"
#include "Vertex.h"
#include "SimpleColorEffect.h"
#include "BillboardEffect.h"
#include "CameraPerspective.h"
#include "ModelMesh.h"
#include "RenderTarget.h"
#include "Scene.h"
#include "View.h"
#include "CameraManager.h"
#include "CameraPerspective.h"
#include "IShapeDrawable.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESCTRUCTOR */
ShapeRenderer::ShapeRenderer() :	
    m_pColorEffect(NEW SimpleColorEffect()),
    m_pBillboardEffect(NEW BillboardEffect()),
    m_pBillboardQuad(nullptr),
    m_View(nullptr),
    m_RenderTarget(nullptr)
{
}

ShapeRenderer::~ShapeRenderer()
{
    delete m_pColorEffect;
    delete m_pBillboardEffect;
    if (m_pBillboardQuad != nullptr)
        m_pBillboardQuad->release();
}

void ShapeRenderer::createBillboardQuad()
{
    m_VertexLayoutBillboard.addElement(BufferElement(0, BufferElement::Type_Vec3, BufferElement::Usage_Position, 12, 0));
    m_VertexLayoutBillboard.addElement(BufferElement(1, BufferElement::Type_Vec2, BufferElement::Usage_TextureCoordinate, 8, 12));

    m_pBillboardQuad = ResourceFactory<ModelMesh>::getInstance()->get(ResourceFactory<ModelMesh>::getInstance()->create());
    m_pBillboardQuad->init(m_VertexLayoutBillboard, gfx::MeshDrawMode_Triangles);

    he::ObjectList<VertexPosTex> vertices(4);
    vertices.add(
        VertexPosTex(vec3(-0.5f, 0.5f,0.0f),
        vec2(0, 1)));

    vertices.add(
        VertexPosTex(vec3(0.5f, 0.5f,0.0f),
        vec2(1, 1)));

    vertices.add(
        VertexPosTex(vec3(-0.5f, -0.5f,0.0f),
        vec2(0, 0)));

    vertices.add(
        VertexPosTex(vec3(0.5f, -0.5f,0.0f),
        vec2(1, 0)));

    he::PrimitiveList<uint8> indices(6);
    indices.add(0); indices.add(1); indices.add(2);
    indices.add(1); indices.add(3); indices.add(2);

    m_pBillboardQuad->setVertices(&vertices[0], 4, gfx::MeshUsage_Static);
    m_pBillboardQuad->setIndices(&indices[0], 6, IndexStride_Byte, gfx::MeshUsage_Static);
    m_pBillboardQuad->setLoaded();
}

/* GENERAL */
void ShapeRenderer::init(View* view, const RenderTarget* target)
{
    m_View = view;
    m_RenderTarget = target;

    createBillboardQuad();

    m_pColorEffect->load();
    m_pBillboardEffect->load();
}

/* DRAW METHODS */
void ShapeRenderer::drawColored(const ModelMesh* model, const mat44& world, const Color& color) const
{
    m_pColorEffect->begin();
    m_pColorEffect->setViewProjection(m_ViewProjection);
    m_pColorEffect->setWorld(world);
    m_pColorEffect->setColor(color);

    GL::heBindVao(model->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, model->getNumIndices(), model->getIndexType(), 0);
}

void ShapeRenderer::drawColoredNoDepth(const ModelMesh* model, const mat44& world, const Color& color) const
{
    GL::heSetDepthRead(false);
    GL::heSetDepthWrite(false);

    m_pColorEffect->begin();
    m_pColorEffect->setViewProjection(m_ViewProjection);
    m_pColorEffect->setWorld(world);
    m_pColorEffect->setColor(color);

    GL::heBindVao(model->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, model->getNumIndices(), model->getIndexType(), 0);

    GL::heSetDepthRead(true);
    GL::heSetDepthWrite(true);
}

void ShapeRenderer::drawMeshColor(const ModelMesh* spline, const mat44& world, const Color& color) const
{
    m_pColorEffect->begin();
    m_pColorEffect->setViewProjection(m_ViewProjection);
    m_pColorEffect->setWorld(world);
    m_pColorEffect->setColor(color);

    GL::heBindVao(spline->getVertexArraysID());
    glDrawElements(spline->getDrawMode(), spline->getNumIndices(), spline->getIndexType(), 0);
}

void ShapeRenderer::drawBillboard(const Texture2D* tex2D, const vec3& pos)
{
    vec2 tcScale(1.0f,1.0f);
    
    mat44 world(mat44::createTranslation(pos));

    m_pBillboardEffect->begin();

    m_pBillboardEffect->setWorldViewProjection(m_ViewProjection * world * m_BillboardMatrix);

    m_pBillboardEffect->setDiffuseMap(tex2D);
    m_pBillboardEffect->setTCScale(tcScale);

    GL::heBindVao(m_pBillboardQuad->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, m_pBillboardQuad->getNumIndices(), m_pBillboardQuad->getIndexType(), 0);
}

void ShapeRenderer::render()
{
    GL::heBlendEnabled(true);
    GL::heBlendFunc(BlendFunc_SrcAlpha, BlendFunc_OneMinusSrcAlpha);
    GL::heSetDepthWrite(true);	
    GL::heSetDepthRead(true);

    m_RenderTarget->prepareForRendering(1);

    gfx::CameraPerspective* cam(m_View->getCamera());
    m_ViewProjection = cam->getViewProjection();
    m_BillboardMatrix = mat44::createBillboard(cam);

    std::for_each(m_Drawables.cbegin(), m_Drawables.cend(), [this](IShapeDrawable* drawable)
    {
        drawable->drawShapes(this);
    });
}

void ShapeRenderer::attachToRenderer( IShapeDrawable* drawable )
{
    m_Drawables.add(drawable);
}

void ShapeRenderer::detachFromRenderer( IShapeDrawable* drawable )
{
    HE_IF_ASSERT(m_Drawables.contains(drawable), "drawable not found in draw list")
    {
        m_Drawables.remove(drawable);
    }
}

} } //end namespace