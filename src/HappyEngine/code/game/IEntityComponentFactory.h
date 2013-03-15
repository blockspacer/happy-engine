//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 2013/03/05

#ifndef _HE_IEntityComponentFactory_H_
#define _HE_IEntityComponentFactory_H_
#pragma once

#include "EntityComponentType.h"
#include "EntityComponentDesc.h"

namespace he {
namespace ge {

class EntityComponent;
class HAPPY_ENTRY IEntityComponentFactory
{
public:
    virtual ~IEntityComponentFactory() {}

    virtual EntityComponent* createEntityComponent(const EntityComponentID& type) const = 0;
    virtual void fillComponentDescList(he::ObjectList<EntityComponentDesc>& list) const = 0;
};

} } //end namespace

#endif
