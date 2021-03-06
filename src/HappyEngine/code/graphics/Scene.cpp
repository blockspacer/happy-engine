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
//Author:  Bastian Damman
//Created: 14/07/2012
#include "HappyPCH.h" 

#include "Scene.h"

#include "DrawListContainer.h"
#include "Drawable.h"

#include "CameraManager.h"
#include "InstancingManager.h"
#include "LightManager.h"

#include "ShadowCaster.h"
#include "SkyBox.h"

namespace he {
namespace gfx {
IMPLEMENT_OBJECT(Scene)

#pragma warning(disable:4355) // use of this in initializerlist
Scene::Scene():
          m_CameraManager(HENew(ge::CameraManager))
        , m_LightManager(HENew(LightManager)(this))
        , m_InstancingManager(HENew(InstancingManager))
        , m_Active(true)
        , m_ShadowCaster(HENew(ShadowCaster))
        , m_SkyBox(nullptr)
        , m_Picker(nullptr)
{
    m_ShadowCaster->init();
}
#pragma warning(default:4355)


Scene::~Scene()
{
    if (m_SkyBox != nullptr && m_SkyBox->getDrawable() != nullptr)
        m_SkyBox->getDrawable()->detachFromScene();
    HEDelete(m_SkyBox);
    HEDelete(m_ShadowCaster);
    HEDelete(m_LightManager);
    HEDelete(m_InstancingManager);
    HEDelete(m_CameraManager);
}

void Scene::loadSkybox( const he::String& asset )
{
    if (m_SkyBox == nullptr)
    {
        m_SkyBox = HENew(SkyBox)();
        m_SkyBox->load(asset);
        if (m_SkyBox != nullptr && m_SkyBox->getDrawable() != nullptr)
            m_SkyBox->getDrawable()->attachToScene(this);
    }
    else
    {
        m_SkyBox->load(asset);
    }
}

void Scene::forceReevalute( Drawable* drawable )
{
    m_DrawList.forceReevalute(drawable);
}

void Scene::doReevalute( Drawable* drawable )
{
    m_DrawList.doReevalute(drawable);
}

void Scene::attachToScene( Drawable* drawable )
{
    m_DrawList.insert(drawable);
}

void Scene::detachFromScene( Drawable* drawable )
{
    m_DrawList.remove(drawable);
}

void Scene::prepareForRendering()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    m_DrawList.prepareForRendering();
    m_ShadowCaster->render(this);
}


} } //end namespace
