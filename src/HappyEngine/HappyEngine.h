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

#ifndef _HE_HAPPYENGINE_H_
#define _HE_HAPPYENGINE_H_
#pragma once

#define SDL_NO_COMPAT
#include "boost/shared_ptr.hpp"
#include "boost/thread.hpp"
#include "SDL.h"
#include "GL/glew.h"

#include <vector>

#include "IGame.h"

#include "GraphicsEngine.h"
#include "ControlsManager.h"
#include "PhysicsEngine.h"
#include "ContentManager.h"
#include "NetworkManager.h"
#include "Happy2DRenderer.h"

#define HAPPYENGINE happyengine::HappyEngine::getPointer()
#define GRAPHICS HAPPYENGINE->getGraphicsEngine()
#define CONTROLS HAPPYENGINE->getControls()
#define PHYSICS HAPPYENGINE->getPhysics()
#define CONTENT HAPPYENGINE->getContentManager()
#define NETWORK HAPPYENGINE->getNetworkManager()
#define HE2D HAPPYENGINE->get2DRenderer()

namespace happyengine {
enum SubEngine
{
    SubEngine_None = 0,
    SubEngine_Graphics = 1 << 0,
    SubEngine_Physics = 1 << 1,
    SubEngine_Networking = 1 << 2,
    SubEngine_Controls = 1 << 3,
    SubEngine_Content = 1 << 4,
	SubEngine_2DRenderer = 1 << 5,
    SubEngine_All = 1<<0 | 1<<1 | 1<<2 | 1<<3 | 1<<4 | 1<<5
};
class HappyEngine
{
public:
    virtual ~HappyEngine();

    static void init(int subengines);
    void start(IGame* pGame);
    static void dispose();

    static HappyEngine* getPointer();

    void quit();

    //subengines
    graphics::GraphicsEngine* getGraphicsEngine() const;
    const io::ControlsManager* getControls() const;
    physics::PhysicsEngine* getPhysics() const;
    content::ContentManager* getContentManager() const;
    networking::NetworkManager* getNetworkManager() const;
	graphics::Happy2DRenderer* get2DRenderer() const;

private:
    // Singleton design pattern
    HappyEngine();
    static HappyEngine* s_pHappyEngine;
    void initSubEngines(int subengines);

    IGame* m_pGame;

    graphics::GraphicsEngine* m_pGraphicsEngine;
    io::ControlsManager* m_pControlsManager;
    physics::PhysicsEngine* m_pPhysicsEngine;
    content::ContentManager* m_pContentManager;
    networking::NetworkManager* m_pNetworkManager;
	graphics::Happy2DRenderer* m_p2DRenderer;

    bool m_Quit;

    int m_SubEngines;

    // Methods
    void initWindow();
    void updateLoop(float dTime);
    void drawLoop(float dTime);
    void cleanup();

    //Disable default copy constructor and default assignment operator
    HappyEngine(const HappyEngine&);
    HappyEngine& operator=(const HappyEngine&);
};

} //end namespace

#endif
