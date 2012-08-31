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
//Author:  
//Created: //

#ifndef _HT_MainGame_H_
#define _HT_MainGame_H_
#pragma once

#include "Game.h"
#include "IShapeDrawable.h"

namespace he {
    namespace tools {
        class FPSGraph;
    }
    namespace ge {
        class Entity;
    }
    namespace gfx {
        class SkyBox;
        class Window;
        class Scene;
        class View;
        class ModelMesh;
        class CameraPerspective;
    }
}

namespace ht {
class FlyCamera;

class MainGame : public he::ge::Game, public he::gfx::IShapeDrawable
{
public:
    MainGame();
    virtual ~MainGame();

    virtual void init();
    virtual void load();
    virtual void tick(float dTime);

    virtual void drawShapes(he::gfx::ShapeRenderer* renderer);

private:
    void fillDebugCameraMeshes(he::gfx::CameraPerspective* camera, he::gfx::ModelMesh* mesh);

    he::tools::FPSGraph* m_FpsGraph;

    bool m_SpinShadows;

    he::gfx::Window* m_Window;
    he::gfx::View*   m_View;
    he::gfx::Scene*  m_Scene;

    he::gfx::Window* m_Window2;
    he::gfx::View*   m_View2;

    std::vector<he::ge::Entity*> m_EntityList;
    he::gfx::SkyBox* m_pSkyBox;

    std::vector<he::gfx::ModelMesh*> m_CameraFrustumMeshes;
    std::vector<he::gfx::ModelMesh*> m_CameraConeMeshes;
    std::vector<he::gfx::ModelMesh*> m_CameraSphereMeshes;
    std::vector<he::vec3> m_PointBuffer;

    //Disable default copy constructor and default assignment operator
    MainGame(const MainGame&);
    MainGame& operator=(const MainGame&);
};

} //end namespace

#endif
