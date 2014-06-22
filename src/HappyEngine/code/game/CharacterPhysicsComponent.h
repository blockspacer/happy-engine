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
//Created: 28/11/2012

#ifndef _HE_CHARACTER_PHYSICS_COMPONENT_H_
#define _HE_CHARACTER_PHYSICS_COMPONENT_H_
#pragma once

#include "EntityComponent.h"
#include "ITickable.h"

namespace he {
namespace px {
    class PhysicsCharacterController;
}
namespace ge {
struct EntityComponentDesc;
class HAPPY_ENTRY CharacterPhysicsComponent : public EntityComponent, public Object3D, public ITickable
{
    IMPLEMENT_IOBJECT3D_FROM(Object3D)
public:
    CharacterPhysicsComponent();
    virtual ~CharacterPhysicsComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         EntityComponent                            ///
    //////////////////////////////////////////////////////////////////////////
    virtual void visit(he::io::BinaryVisitor* const /*visitor*/) {}

    virtual void activate();
    virtual void deactivate();

    virtual const he::FixedString& getComponentID() const { return HEFS::strCharacterPhysicsComponent; }

    //// Editor //////////////////////////////////////////////////////////////
    static void fillEntityComponentDesc(EntityComponentDesc& desc);
    virtual bool setProperty(const Property* const inProperty);
    virtual bool getProperty(Property* const inOutProperty);
    //////////////////////////////////////////////////////////////////////////

    //////////////////////////////////////////////////////////////////////////
     
    //////////////////////////////////////////////////////////////////////////
    ///                            ITickable                               ///
    //////////////////////////////////////////////////////////////////////////
    virtual void tick(float dTime);
    //////////////////////////////////////////////////////////////////////////
    

protected:
    virtual void init(Entity* parent);

    px::PhysicsCharacterController* getCharacterController() const { return m_CharacterController; }

private:
    px::PhysicsCharacterController* m_CharacterController;
    
    Entity* m_Parent;
    
    //Disable default copy constructor and default assignment operator
    CharacterPhysicsComponent(const CharacterPhysicsComponent&);
    CharacterPhysicsComponent& operator=(const CharacterPhysicsComponent&);
};
} } //end namespace

#endif
