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
#include "SimpleColorEffect.h"

#include "ContentManager.h"
#include "Material.h"
#include "MaterialInstance.h"
#include "MaterialParameter.h"

namespace he {
namespace gfx {

/* CONSTRUCTOR - DESTRUCTOR */
SimpleColorEffect::SimpleColorEffect()
    : m_Material(nullptr)
    , m_ViewProj(-1)
    , m_World(-1)
    , m_Color(-1)
{
}

SimpleColorEffect::~SimpleColorEffect()
{
    delete m_Material;
}

/* GENERAL */
void SimpleColorEffect::init(const BufferLayout& layout)
{
    Material* mat(CONTENT->loadMaterial("engine/2D/simple.hm"));
    m_Material = mat->createMaterialInstance(eShaderType_Normal);
    m_Material->calculateMaterialLayout(layout);

    m_ViewProj = m_Material->findParameter(HEFS::strmatVP);
    m_World = m_Material->findParameter(HEFS::strmatW);
    m_Color = m_Material->findParameter(HEFS::strcolor);

    setViewProjection(mat44::Identity);
    setWorld(mat44::Identity);
    setColor(Color(1.0f, 1.0f, 1.0f, 1.0f));
}

void SimpleColorEffect::begin(const DrawContext& context) const
{
    m_Material->apply(context);
}

void SimpleColorEffect::end() const
{
}

/* SETTERS */
void SimpleColorEffect::setViewProjection(const mat44& mat)
{
    m_Material->getParameter(m_ViewProj).setFloat44(mat);
}

void SimpleColorEffect::setWorld(const mat44& mat)
{
    m_Material->getParameter(m_World).setFloat44(mat);
}

void SimpleColorEffect::setColor(const Color& color)
{
    m_Material->getParameter(m_Color).setFloat4(color.rgba());
}

} } //end namespace
