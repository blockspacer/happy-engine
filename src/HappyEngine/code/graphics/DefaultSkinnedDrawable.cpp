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
//Created: 18/12/2011
#include "HappyPCH.h" 

#include "DefaultSkinnedDrawable.h"
#include "ICamera.h"
#include "ModelMesh.h"
#include "Material.h"
#include "Scene.h"

namespace he {
namespace gfx {

DefaultSkinnedDrawable::DefaultSkinnedDrawable(): m_CastsShadow(true), m_Bound(AABB(vec3(-1, -1, -1), vec3(1, 1, 1)))
{
}


DefaultSkinnedDrawable::~DefaultSkinnedDrawable()
{
}

void DefaultSkinnedDrawable::applyMaterial( const ICamera* pCamera ) const
{
    getMaterial()->apply(this, pCamera);
}

void DefaultSkinnedDrawable::applyMaterial( const Material* customMaterial, const ICamera* pCamera ) const
{
    customMaterial->apply(this, pCamera);
}

bool DefaultSkinnedDrawable::getCastsShadow() const
{
    return m_CastsShadow;
}

void DefaultSkinnedDrawable::setCastsShadow( bool castShadow )
{
    m_CastsShadow = castShadow;
}

void DefaultSkinnedDrawable::draw()
{
    GL::heBindVao(getModelMesh()->getVertexArraysID());
    glDrawElements(GL_TRIANGLES, getModelMesh()->getNumIndices(), getModelMesh()->getIndexType(), 0);
}

void DefaultSkinnedDrawable::drawShadow()
{
    GL::heBindVao(getModelMesh()->getVertexShadowArraysID());
    glDrawElements(GL_TRIANGLES, getModelMesh()->getNumIndices(), getModelMesh()->getIndexType(), 0);
}

void DefaultSkinnedDrawable::detachFromScene()
{
    HE_IF_ASSERT(m_Scene != nullptr, "Object not attached to scene")
    {
        m_Scene->detachFromScene(this);
        m_Scene = nullptr;
    }
}

void DefaultSkinnedDrawable::attachToScene( Scene* scene, bool dynamic )
{
    HE_IF_ASSERT(m_Scene == nullptr, "Object already attached to scene")
    {
        m_Scene = scene;
        m_Scene->attachToScene(this, dynamic);
    }
}

Scene* DefaultSkinnedDrawable::getScene() const
{
    return m_Scene;
}

bool DefaultSkinnedDrawable::isAttachedToScene() const
{
    return m_Scene != nullptr;
}

} } //end namespace