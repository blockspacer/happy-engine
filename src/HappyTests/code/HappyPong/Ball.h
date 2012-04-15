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
//Created: 30/03/2012

#ifndef _HT_BALL_H_
#define _HT_BALL_H_
#pragma once

#include "Entity.h"
#include "ITickable.h"
#include "Random.h"


namespace ht {
class MainGame;
class LightFlashComponent;
class Ball : public he::game::Entity, public he::game::ITickable
{
public:
    Ball(MainGame* mainGame);
    virtual ~Ball();

    virtual void tick(float dTime);

    void reset();
    const he::vec3& getPosition() const;
    const he::vec3& getVelocity() const;
    float getRadius() const;

private:

    he::vec3 m_Velocity;
    he::vec3 m_Position;

    float m_Restitution;

    const float m_Radius;

    MainGame* m_MainGame;

    LightFlashComponent* m_LightFlashComponent;

    bool m_Dead;

    he::Random m_Random;

    //Disable default copy constructor and default assignment operator
    Ball(const Ball&);
    Ball& operator=(const Ball&);
};

} //end namespace

#endif