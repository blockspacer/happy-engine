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
//Created: 20/08/2011

#ifndef _HE_PHYSICS_STATIC_ACTOR_H_
#define _HE_PHYSICS_STATIC_ACTOR_H_
#pragma once

#include "IPhysicsActor.h"
#include "PhysicsUserData.h"

namespace physx {
    class PxShape;
    class PxRigidStatic;
}

namespace he {
namespace px {
class IPhysicsShape;
class PhysicsMaterial;

class PhysicsStaticActor : public IPhysicsActor
{
public:
    PhysicsStaticActor(const mat44& pose, const IPhysicsShape* shape, const PhysicsMaterial& material);
    PhysicsStaticActor(const mat44& pose);
    virtual ~PhysicsStaticActor();
    
    virtual vec3 getPosition() const;
    virtual mat44 getPose() const;
    virtual physx::PxRigidActor* getInternalActor() const;

    void addShape(const IPhysicsShape* shape, const PhysicsMaterial& material, 
                uint32 collisionGroup = 0xffffffff, 
                const mat44& localPose = mat44::Identity);

    virtual const PhysicsUserData& getUserData() { return m_UserData; }
    template<typename T>
    void setUserData(T* rttiType)
    {
        if (rttiType != nullptr)
            m_UserData.setRTTI(T::getRTTI());
        else
            m_UserData.setRTTI(0);
        m_UserData.setData(rttiType);
    }

private:
    void addShape(physx::PxShape* shape, uint32 collisionGroup);

    physx::PxRigidStatic* m_Actor;
    PhysicsUserData m_UserData;

    //Disable default copy constructor and default assignment operator
    PhysicsStaticActor(const PhysicsStaticActor&);
    PhysicsStaticActor& operator=(const PhysicsStaticActor&);
};

} } //end namespace

#endif
