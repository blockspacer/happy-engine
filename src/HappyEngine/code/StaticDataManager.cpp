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
//Created: 24/03/2012
#include "HappyPCH.h" 

#include "StaticDataManager.h"
#include "ResourceFactory.h"
#include "Texture2D.h"
#include "TextureCube.h"
#include "Model.h"
#include "ModelMesh.h"
#include "Font.h"
#include "PhysicsConcaveMesh.h"
#include "PhysicsConvexMesh.h"
#include "LightFactory.h"
#include "Shader.h"
#include "Material.h"
#include "View.h"
#include "Scene.h"
#include "Window.h"
#include "CullOctreeNodeFactory.h"
#include "ShaderGeneratorVariableFactory.h"
#include "Sprite.h"
#include "materialGenerator/MaterialGeneratorNodeFactory.h"
#include "SpriteCreator.h"
#include "PickingManager.h"
#include "BinaryVisitor.h"
#include "EntityManager.h"
#include "SystemStats.h"

namespace he {

void StaticDataManager::init()
{
    he::tools::SystemStats::sdmInit();
    he::HappyEngine::sdmInit();
    details::ObjectFactoryTypeManager::sdmInit();
    tools::Logger::sdmInit();
    ResourceFactory<gfx::Material>::sdmInit();
    ResourceFactory<gfx::Material>::getInstance()->init(0, 16, "MaterialFactory");
    ResourceFactory<gfx::Texture2D>::sdmInit();
    ResourceFactory<gfx::Texture2D>::getInstance()->init(0, 16, "TextureFactory");
    ResourceFactory<gfx::TextureCube>::sdmInit();
    ResourceFactory<gfx::TextureCube>::getInstance()->init(0, 5, "TextureCubeFactory");
    ResourceFactory<gfx::Model>::sdmInit();
    ResourceFactory<gfx::Model>::getInstance()->init(0, 16, "ModelFactory");
    ResourceFactory<gfx::ModelMesh>::sdmInit();
    ResourceFactory<gfx::ModelMesh>::getInstance()->init(0, 16, "ModelMeshFactory");
    ResourceFactory<gui::Font>::sdmInit();
    ResourceFactory<gui::Font>::getInstance()->init(0, 2, "FontFactory");
    ResourceFactory<gfx::Shader>::sdmInit();
    ResourceFactory<gfx::Shader>::getInstance()->init(0, 8, "ShaderFactory");
    ResourceFactory<px::PhysicsConvexMesh>::sdmInit();
    ResourceFactory<px::PhysicsConvexMesh>::getInstance()->init(0, 16, "PhysicsConvexMeshFactory");
    ResourceFactory<px::PhysicsConcaveMesh>::sdmInit();
    ResourceFactory<px::PhysicsConcaveMesh>::getInstance()->init(0, 16, "PhysicsConcaveMeshFactory");
    gui::SpriteFactory::sdmInit();
    gfx::ViewFactory::sdmInit();
    gfx::SceneFactory::sdmInit();
    gfx::WindowFactory::sdmInit();
    gfx::LightFactory::sdmInit();
    gfx::CullOctreeNodeFactory::sdmInit();
    ct::ShaderGeneratorVariableFactory::sdmInit();
    tools::MaterialGeneratorNodeFactory::sdmInit();
    ge::PickingManager::sdmInit();
    ge::EntityManager::sdmInit();
}

void StaticDataManager::destroy()
{
    ge::EntityManager::sdmDestroy();
    ge::PickingManager::sdmDestroy();
    tools::MaterialGeneratorNodeFactory::sdmDestroy();
    ct::ShaderGeneratorVariableFactory::sdmDestroy();
    gfx::CullOctreeNodeFactory::sdmDestroy();
    gfx::LightFactory::sdmDestroy();
    ResourceFactory<gui::Font>::sdmDestroy();
    ResourceFactory<gfx::Material>::sdmDestroy();
    ResourceFactory<gfx::TextureCube>::sdmDestroy();
    ResourceFactory<gfx::Texture2D>::sdmDestroy();
    ResourceFactory<gfx::Model>::sdmDestroy();
    ResourceFactory<gfx::ModelMesh>::sdmDestroy();
    ResourceFactory<gfx::Shader>::sdmDestroy();
    ResourceFactory<px::PhysicsConvexMesh>::sdmDestroy();
    ResourceFactory<px::PhysicsConcaveMesh>::sdmDestroy();
    gui::SpriteFactory::sdmDestroy();
    gfx::ViewFactory::sdmDestroy();
    gfx::SceneFactory::sdmDestroy();
    gfx::WindowFactory::sdmDestroy();
    tools::Logger::sdmDestroy();
    details::ObjectFactoryTypeManager::sdmDestroy();
    he::HappyEngine::sdmDestroy();
    he::tools::SystemStats::sdmDestroy();
}

void StaticDataManager::visit( he::io::BinaryVisitor& visitor )
{
    ge::EntityManager::sdmVisit(visitor);
    ge::PickingManager::sdmVisit(visitor);
    tools::MaterialGeneratorNodeFactory::sdmVisit(visitor);
    ct::ShaderGeneratorVariableFactory::sdmVisit(visitor);
    gfx::CullOctreeNodeFactory::sdmVisit(visitor);
    gfx::LightFactory::sdmVisit(visitor);
    ResourceFactory<gui::Font>::sdmVisit(visitor);
    ResourceFactory<gfx::Material>::sdmVisit(visitor);
    ResourceFactory<gfx::TextureCube>::sdmVisit(visitor);
    ResourceFactory<gfx::Texture2D>::sdmVisit(visitor);
    ResourceFactory<gfx::Model>::sdmVisit(visitor);
    ResourceFactory<gfx::ModelMesh>::sdmVisit(visitor);
    ResourceFactory<gfx::Shader>::sdmVisit(visitor);
    ResourceFactory<px::PhysicsConvexMesh>::sdmVisit(visitor);
    ResourceFactory<px::PhysicsConcaveMesh>::sdmVisit(visitor);
    gui::SpriteFactory::sdmVisit(visitor);
    gfx::ViewFactory::sdmVisit(visitor);
    gfx::SceneFactory::sdmVisit(visitor);
    gfx::WindowFactory::sdmVisit(visitor);
    tools::Logger::sdmVisit(visitor);
    details::ObjectFactoryTypeManager::sdmVisit(visitor);
    he::HappyEngine::sdmVisit(visitor);
    he::tools::SystemStats::sdmVisit(visitor);
}

} //end namespace