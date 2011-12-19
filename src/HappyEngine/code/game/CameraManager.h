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
//Created: 06/12/2011

#ifndef _HE_CAMERA_MANAGER_H_
#define _HE_CAMERA_MANAGER_H_
#pragma once

#include <map>
#include "ITickable.h"

namespace he {
namespace gfx{
class Camera;
}

namespace game{

class CameraManager : public ITickable
{
public:
    CameraManager();
    virtual ~CameraManager(); //deleteAllCameras is not auto called, user can decide when to delete using deleteCamera or deleteAllCamera

    void init(); // Internal called

    void addCamera(const std::string& id, gfx::Camera* pCamera);
    void deleteCamera(const std::string& id);
    void deleteAllCameras();
    void setActiveCamera(const std::string& id);

    gfx::Camera* getActiveCamera() const;

    virtual void tick(float dTime); // Internal called

private:

    gfx::Camera* m_pActiveCamera;
    std::map<std::string, gfx::Camera*> m_Cameras;


    //Disable default copy constructor and default assignment operator
    CameraManager(const CameraManager&);
    CameraManager& operator=(const CameraManager&);
};

} } //end namespace

#endif