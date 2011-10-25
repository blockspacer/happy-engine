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
//Author:  Bastian Damman
//Created: 30/09/2011
#include "StdAfx.h" 

#include "Entity.h"
#include "HappyNew.h"

namespace he {
namespace game {

Entity::Entity(const EntityDesc& desc): m_Material(desc.material), m_pModel(desc.pModel)
{
}


Entity::~Entity()
{
}

const mat44& Entity::getWorldMatrix() const
{
    return m_mtxWorld;
}

void Entity::setModel(const gfx::Model::pointer& pModel)
{
    m_pModel = pModel;
}
void Entity::setWorldMatrix(const mat44& mtxWorld)
{
    m_mtxWorld = mtxWorld;
}
const gfx::Material& Entity::getMaterial() const
{
    return m_Material;
}
const gfx::Model::pointer Entity::getModel() const
{
    return m_pModel;
}

} } //end namespace