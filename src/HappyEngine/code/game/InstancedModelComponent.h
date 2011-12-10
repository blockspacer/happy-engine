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
//Created: 09/12/2011

#ifndef _HE_INSTANCED_MODEL_COMPONENT_H_
#define _HE_INSTANCED_MODEL_COMPONENT_H_
#pragma once

#include "IComponent.h"
#include "I3DObject.h"
#include "IPickable.h"

namespace he {
namespace game {
    
class InstancedModelComponent : public IComponent, public gfx::I3DObject, public gfx::IPickable
{
public:
    InstancedModelComponent();
    virtual ~InstancedModelComponent();

    //////////////////////////////////////////////////////////////////////////
    ///                         IComponent                                 ///
    //////////////////////////////////////////////////////////////////////////
    virtual void init(Entity* pParent);

    virtual void serialize(SerializerStream& stream);
    virtual void deserialize(const SerializerStream& stream);
    //////////////////////////////////////////////////////////////////////////
    

    //////////////////////////////////////////////////////////////////////////
    ///                         I3DObject                                  ///
    ////////////////////////////////////////////////////////////////////////// 
    virtual mat44 getWorldMatrix() const;
    //////////////////////////////////////////////////////////////////////////
    
    void setLocalTransform(const mat44& mtxWorld);
    const mat44& getLocalTransform() const;

    void setController(const std::string& key);
    const std::string& getController() const;

private:
    std::string m_ControllerKey;
    uint m_InstanceId;

    mat44 m_mtxLocalTransform;

    Entity* m_pParent;
    

    //Disable default copy constructor and default assignment operator
    InstancedModelComponent(const InstancedModelComponent&);
    InstancedModelComponent& operator=(const InstancedModelComponent&);
};

} } //end namespace

#endif
