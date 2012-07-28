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
//Created: 30/09/2011
//Changed: 04/11/2011
#include "HappyPCH.h" 

#include "Entity.h"
#include "Scene.h"
#include "GraphicsEngine.h"

namespace he {
namespace ge {

Entity::Entity(): m_IsSerializeDataDirty(false), m_Scene(nullptr)
{
}


Entity::~Entity()
{
    std::for_each(m_Components.cbegin(), m_Components.cend(), [](IComponent* pComponent)
    {
        delete pComponent;
    });
}
void Entity::init( gfx::Scene* scene )
{
    m_Scene = scene;
}


mat44 Entity::getWorldMatrix() const
{
    return m_mtxWorld;
}

void Entity::setWorldMatrix(const mat44& mtxWorld)
{
    m_mtxWorld = mtxWorld;
}

void Entity::addComponent( IComponent* pComponent )
{
    HE_ASSERT(m_Scene != nullptr, "Init Entity first!");
    m_Components.push_back(pComponent);
    pComponent->init(this);
}

void Entity::deleteComponent( IComponent* pComponent )
{
    m_Components.erase(std::remove(m_Components.begin(), m_Components.end(), pComponent), m_Components.end());
    delete pComponent;
}

void Entity::serializeCreate( NetworkStream* stream ) const
{
    stream->Write(m_Scene->getId());
    std::for_each(m_Components.cbegin(), m_Components.cend(), [&stream](IComponent* component)
    {
        component->serializeCreate(stream);
    });
}

bool Entity::deserializeCreate( NetworkStream* stream )
{
    SceneID id;
    stream->Read(id);
    m_Scene = GRAPHICS->getScene(id);
    bool keep(true);
    std::for_each(m_Components.cbegin(), m_Components.cend(), [&keep,&stream](IComponent* component)
    {
        keep &= component->deserializeCreate(stream);
    });
    return keep;
}

void Entity::serializeRemove( NetworkStream* stream ) const
{
    std::for_each(m_Components.cbegin(), m_Components.cend(), [&stream](IComponent* component)
    {
        component->serializeRemove(stream);
    });
}

bool Entity::deserializeRemove( NetworkStream* stream )
{
    bool remove(true);
    std::for_each(m_Components.cbegin(), m_Components.cend(), [&remove,&stream](IComponent* component)
    {
        remove &= component->deserializeRemove(stream);
    });
    return remove;
}

bool Entity::isSerializeDataDirty() const
{
    if (m_IsSerializeDataDirty)
        return true;

    std::vector<IComponent*>::const_iterator it(m_Components.cbegin());
    for (; it != m_Components.cend(); ++it)
    {
        if ((*it)->isSerializeDataDirty())
            return true;
    }
    return false;
}

void Entity::serialize( net::NetworkSerializer& serializer )
{
    std::for_each(m_Components.cbegin(), m_Components.cend(), [&serializer](IComponent* component)
    {
        component->serialize(serializer);
    });
    m_IsSerializeDataDirty = false;
}

void Entity::deserialize( net::NetworkDeserializer& serializer )
{
    std::for_each(m_Components.cbegin(), m_Components.cend(), [&serializer](IComponent* component)
    {
        component->deserialize(serializer);
    });
}

bool Entity::isSleeping() const
{
    std::vector<boost::function0<bool>>::const_iterator it(m_SleepEvaluaters.cbegin());
    for( ; it != m_SleepEvaluaters.cend(); ++it)
    {
        if ((*it)())
            return true;
    }
    return false;
}

void Entity::addSleepEvaluator( const boost::function0<bool>& evaluater )
{
    m_SleepEvaluaters.push_back(evaluater);
}

void Entity::removeSleepEvaluator( const boost::function0<bool>& evaluater )
{
    HE_IF_ASSERT(std::find(m_SleepEvaluaters.cbegin(), m_SleepEvaluaters.cend(), evaluater) != m_SleepEvaluaters.cend(), "Sleep evaluator not found")
    {
        *std::find(m_SleepEvaluaters.begin(), m_SleepEvaluaters.end(), evaluater) = m_SleepEvaluaters.back();
        m_SleepEvaluaters.pop_back();
    }
}

} } //end namespace