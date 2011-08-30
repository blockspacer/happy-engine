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
//Author:  Sebastiaan Sprengers
//Created: 25/08/2011

#include "Simple2DRenderer.h"
#include "HappyNew.h"
#include "GL/glew.h"
#include "Model.h"
#include "HappyEngine.h"

#include <vector>

namespace happyengine {
namespace graphics {

Simple2DRenderer::Simple2DRenderer() :	m_pEffect(NEW happyengine::graphics::Simple2DEffect()),
										m_bAntiAliasing(false)
{
    
}

Simple2DRenderer::~Simple2DRenderer()
{
	delete m_pEffect;
}

void Simple2DRenderer::begin()
{
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (m_bAntiAliasing)
		glEnable(GL_LINE_SMOOTH);

	m_pEffect->begin();
}

void Simple2DRenderer::end()
{
	glDisable(GL_BLEND);
	
	if (m_bAntiAliasing)
		glDisable(GL_LINE_SMOOTH);

	m_pEffect->end();
}

void Simple2DRenderer::initialize(bool)
{
	m_VertexLayout.addElement(VertexElement(0, VertexElement::Type_Vector3, VertexElement::Usage_Position, 12, 0, "inPosition"));
	m_VertexLayout.addElement(VertexElement(1, VertexElement::Type_Vector4, VertexElement::Usage_Other, 16, 12, "inColor"));

	glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE); //disable enable writing to depth buffer
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);

	m_pEffect->load();
}

void Simple2DRenderer::setColor(float r, float g, float b, float a)
{
	m_CurrentColor.r(r);
	m_CurrentColor.g(g);
	m_CurrentColor.b(b);
	m_CurrentColor.a(a);
}

void Simple2DRenderer::setTransformationMatrix(const happyengine::math::Matrix &mat) const
{
	m_pEffect->setWorldMatrix(mat);
}

void Simple2DRenderer::setAntiAliasing(bool bAA)
{
	m_bAntiAliasing = bAA;
}

void Simple2DRenderer::drawText(const std::string &, float, float) const
{

}

void Simple2DRenderer::drawRectangle(float x, float y, float width, float height, float strokeSize) const
{
	int viewportWidth = GRAPHICS->getViewport().width, 
        viewportHeight = GRAPHICS->getViewport().height;

	x = (x / viewportWidth) * 2.0f - 1.0f;
	y = -((y / viewportHeight) * 2.0f - 1.0f);
    width = (width / viewportWidth) * 2.0f;
    height = -(height / viewportHeight) * 2.0f;

	std::vector<VertexPosCol> vertices;
    vertices.push_back(VertexPosCol(math::Vector3(x, y, 0), m_CurrentColor.rgba()));
	vertices.push_back(VertexPosCol(math::Vector3(x + width, y, 0), m_CurrentColor.rgba()));
	vertices.push_back(VertexPosCol(math::Vector3(x + width, y + height, 0), m_CurrentColor.rgba()));
	vertices.push_back(VertexPosCol(math::Vector3(x, y + height, 0), m_CurrentColor.rgba()));
	
    std::vector<byte> indices;
    indices.push_back(0); indices.push_back(1);
	indices.push_back(2); indices.push_back(3);

	Model model;
    model.init();
    model.setVertices(&vertices[0], 4, m_VertexLayout);
    model.setIndices(&indices[0], 4, IndexType_Byte);

	glLineWidth(strokeSize);
	glPointSize(strokeSize);

	glBindVertexArray(model.getVertexArraysID());

	glDrawElements(GL_LINE_LOOP, model.getNumIndices(), model.getIndexType(), 0);

    glBindVertexArray(0);

	glLineWidth(1.0f);
	glPointSize(1.0f);
}

void Simple2DRenderer::fillRectangle(float x, float y, float width, float height) const
{
	int viewportWidth = GRAPHICS->getViewport().width, 
        viewportHeight = GRAPHICS->getViewport().height;

	x = (x / viewportWidth) * 2 - 1;
	y = -((y / viewportHeight) * 2 - 1);
    width = (width / viewportWidth) * 2;
    height = -(height / viewportHeight) * 2;

	std::vector<VertexPosCol> vertices;
    vertices.push_back(VertexPosCol(math::Vector3(x, y, 0), m_CurrentColor.rgba()));
	vertices.push_back(VertexPosCol(math::Vector3(x + width, y, 0), m_CurrentColor.rgba()));
	vertices.push_back(VertexPosCol(math::Vector3(x, y + height, 0), m_CurrentColor.rgba()));
	vertices.push_back(VertexPosCol(math::Vector3(x + width, y + height, 0), m_CurrentColor.rgba()));

    std::vector<byte> indices;
    indices.push_back(0); indices.push_back(2); indices.push_back(1);
    indices.push_back(1); indices.push_back(2); indices.push_back(3);

	Model model;
    model.init();
    model.setVertices(&vertices[0], 4, m_VertexLayout);
    model.setIndices(&indices[0], 6, IndexType_Byte);

	glBindVertexArray(model.getVertexArraysID());

	glDrawElements(GL_TRIANGLES, model.getNumIndices(), model.getIndexType(), 0);

    glBindVertexArray(0);
}

} } //end namespace