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
//Created: 27/11/2011

#ifndef _HE_FX_PARTICLE_H_
#define _HE_FX_PARTICLE_H_
#pragma once

#include "vec3.h"
#include "vec4.h"

namespace he {
namespace gfx {

class ICamera;

enum FxFlags
{
    FxFlag_LockX = 1 << 0,
    FxFlag_LockY = 1 << 1,
    FxFlag_LockZ = 1 << 2
};

//16byte aligned
struct FxParticle
{
    vec3    m_Position;    //+ 12  = 12
    float   m_LifeTime;    //+  4  = 16

    vec3    m_Scale;       //+ 12  = 28
    float   m_Rotation;    //+  4  = 32

    vec3    m_Velocity;    //+ 12  = 44
    float   m_MaxLifeTime; //+  4  = 48

    vec4    m_BlendColor;  //+ 16  = 64

    vec2    m_UvTile;      //+ 12  = 76
    uint16  m_Id;          //+  2  = 78
    uint16  m_Flags;       //+  2  = 80

    void setToDefault()
    {
        m_Position = vec3::zero;
        m_LifeTime = 0.0f;
        m_MaxLifeTime = 3.0f;
        m_Scale = vec3::one;
        m_Rotation = 0.0f;
        m_Velocity = vec3::zero;
        m_UvTile = vec2(0, 0);
        m_Flags = 0;
        m_BlendColor = vec4(1, 1, 1, 1);
        m_Id = 0;
    }

    mat44 getWorld(ICamera* pCam)
    {
        return mat44::createTranslation(m_Position) * mat44::createBillboard(pCam) * mat44::createRotation(vec3(0, 0, 1), m_Rotation) * mat44::createScale(m_Scale);
    }
};

} } //end namespace

#endif
