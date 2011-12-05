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

#ifndef _HE_FX_CAMERA_EFFECT_H_
#define _HE_FX_CAMERA_EFFECT_H_
#pragma once

#include "IFxComponent.h"
#include "IFxVariable.h"

#include "Random.h"

namespace he {
namespace gfx {
   
class FxCameraEffect : public IFxComponent
{
public:
    FxCameraEffect();
    virtual ~FxCameraEffect();

    //////////////////////////////////////////////////////////////////////////
    ///                             IFxComponent                           ///
    //////////////////////////////////////////////////////////////////////////
    virtual FxType getType() const { return FxType_CameraEffect; };

    virtual void start();
    virtual void stop();

    virtual void tick(float currentTime, float dTime);

    //////////////////////////////////////////////////////////////////////////
    ///                             Properties                             ///
    //////////////////////////////////////////////////////////////////////////
    void toggleShake(bool enable); 
    void setShakeIntensity(const IFxVariable<vec3>::pointer& pVar); 

private:
    Random m_Random;
    bool m_ShakeEnabled;
    IFxVariable<vec3>::pointer m_pShakeIntensity;

    //Disable default copy constructor and default assignment operator
    FxCameraEffect(const FxCameraEffect&);
    FxCameraEffect& operator=(const FxCameraEffect&);
};

} } //end namespace

#endif