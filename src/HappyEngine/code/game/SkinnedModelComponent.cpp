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
//Created: 29/10/2011
#include "HappyPCH.h" 

#include "SkinnedModelComponent.h"

#include "Entity.h"
#include "GraphicsEngine.h"
#include "ModelMesh.h"
#include "Material.h"
#include "EntityComponentDesc.h"

namespace he {
namespace ge {
    
SkinnedModelComponent::SkinnedModelComponent(): m_ModelMesh(nullptr), m_Parent(nullptr), m_Material(nullptr)
{
}


SkinnedModelComponent::~SkinnedModelComponent()
{
    setModelMesh(he::ObjectHandle::unassigned);
    if (m_Material != nullptr)
        m_Material->release();
}

void SkinnedModelComponent::init( Entity* parent )
{
    m_Parent = parent;
}

const gfx::Material* SkinnedModelComponent::getMaterial() const
{
    return m_Material;
}

const gfx::ModelMesh* SkinnedModelComponent::getModelMesh() const
{
    return m_ModelMesh;
}

void SkinnedModelComponent::setModelMesh( const ObjectHandle& modelHandle )
{
    if (m_ModelMesh != nullptr)
    {
        gfx::ModelMesh* const oldMesh(ResourceFactory<gfx::ModelMesh>::getInstance()->get(m_ModelMesh->getHandle()));
        oldMesh->cancelLoadCallback(this);
        oldMesh->release();
    }
    gfx::ModelMesh* const newMesh(ResourceFactory<gfx::ModelMesh>::getInstance()->get(modelHandle));
    m_ModelMesh = newMesh;
    m_BoneTransform.clear();
    m_Bones.clear();
    if (newMesh != nullptr)
    {
        newMesh->instantiate();
        newMesh->callbackOnceIfLoaded(this, boost::bind(&SkinnedModelComponent::modelLoadedCallback, this));
    }
}
void SkinnedModelComponent::modelLoadedCallback()
{
    m_BoneTransform.reserve(m_ModelMesh->getBones().size());
    std::for_each(m_ModelMesh->getBones().cbegin(), m_ModelMesh->getBones().cend(), [&](const gfx::Bone& bone)
    {
        //m_BoneTransform.push_back(bone.m_BaseTransform);
        m_BoneTransform.add(mat44::Identity);

        BoneTransform transform;
        transform.m_ToOrigTransform = bone.m_BaseTransform;
        transform.m_FromOrigTransform = bone.m_BaseTransform.inverse();
        transform.m_RealTransform = &m_BoneTransform.back();

        m_Bones[bone.m_Name] = transform;
    });

    if (m_BoneTransform.size() > 0)
    {

        onModelLoaded();
    }
    else
    {
        HE_ERROR("SkinnedMesh error: no bones found in %s", m_ModelMesh->getName().c_str());
    }
}


void SkinnedModelComponent::setMaterial( const ObjectHandle& material )
{
    if (m_Material != nullptr)
        m_Material->release();
    m_Material = ResourceFactory<gfx::Material>::getInstance()->get(material);
    if (m_Material != nullptr)
        ResourceFactory<gfx::Material>::getInstance()->instantiate(material);
}

const he::PrimitiveList<mat44>& SkinnedModelComponent::getBoneTransforms() const
{
    return m_BoneTransform;
}

SkinnedModelComponent::BoneTransform SkinnedModelComponent::getBone( const he::String& name ) const
{
    std::map<he::String, BoneTransform>::const_iterator it(m_Bones.find(name));
    if (it == m_Bones.cend())
    {
        HE_ERROR("SkinnedModelComponent error: No bone exists with name: %s", name.c_str());
        return BoneTransform();
    }
    else
    {
        return it->second;
    }
}

void SkinnedModelComponent::fillEntityComponentDesc( EntityComponentDesc& /*desc*/ )
{
    LOG(LogType_ProgrammerAssert, "Not implemented");
}

bool SkinnedModelComponent::setProperty( const Property* const inProperty )
{
    return EntityComponent::setProperty(inProperty);
}

bool SkinnedModelComponent::getProperty( Property* const inOutProperty )
{
    return EntityComponent::getProperty(inOutProperty);
}


} } //end namespace
