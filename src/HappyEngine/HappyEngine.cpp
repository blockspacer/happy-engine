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

#include "HappyEngine.h"
#include "HappyNew.h"

#include <iostream>

#include "Assert.h"
#include "ExternalError.h"
#include "IL/il.h"
#include "IL/ilu.h"

namespace happyengine {

HappyEngine::pointer HappyEngine::s_pHappyEngine = boost::shared_ptr<HappyEngine>(NEW HappyEngine());

HappyEngine::HappyEngine(): m_pGame(nullptr), m_Quit(false), m_Loaded(false), 
                            m_pGraphicsEngine(nullptr), m_pControlsManager(nullptr),
                            m_pPhysicsEngine(nullptr), m_pContentManager(nullptr)
{
}
HappyEngine::~HappyEngine()
{
}

void HappyEngine::quit()
{
    m_Quit = true;
}
void HappyEngine::dispose()
{
    cleanup();
    std::cout << "\n--Thank you for using HappyEngine--\n";
}
void HappyEngine::cleanup()
{   
    //dispose/delete all sub engines here
    delete m_pGraphicsEngine;
    m_pGraphicsEngine = nullptr;
    delete m_pControlsManager;
    m_pControlsManager = nullptr;
    delete m_pPhysicsEngine;
    m_pPhysicsEngine = nullptr;
    delete m_pContentManager;
    m_pContentManager = nullptr;

    SDL_Quit();
}
void HappyEngine::initSubEngines()
{
    ilInit();
    iluInit();
    iluSetLanguage(ILU_ENGLISH);
    //init all sub engines here
    m_pGraphicsEngine = NEW graphics::GraphicsEngine();
    m_pControlsManager = NEW io::ControlsManager();
    m_pPhysicsEngine = NEW physics::PhysicsEngine();
    m_pContentManager = NEW content::ContentManager();
}

void HappyEngine::start(IGame* pGame)
{
    cleanup();

    using namespace std;
    cout << "       ******************************       \n";
    cout << "  ****************        ****************  \n";
    cout << "*************  HappyEngine :D  *************\n";
    cout << "  ***************          ***************  \n";
    cout << "       ******************************       \n";
    cout << "os: " << SDL_GetPlatform() << "\n\n";

    m_pGame = pGame;

    //Init SDL
    SDL_Init(SDL_INIT_EVERYTHING);

    //Init other stuff
    initSubEngines();

    //Init Game
    pGame->init();
    
    //Run update loop
    boost::thread updateThread(boost::bind(&HappyEngine::updateLoop, this));

    //Run draw loop
    drawLoop();
    
    updateThread.join();
}
void HappyEngine::updateLoop()
{
    Uint32 prevTicks(SDL_GetTicks());
    while (m_Quit == false)
    {
        Uint32 ticks(SDL_GetTicks());
        float dTime((ticks - prevTicks) / 1000.0f);
        prevTicks = ticks;

        if (m_Loaded)
        {
            m_pContentManager->tick(dTime);
            m_pPhysicsEngine->tick(dTime);
            m_pGame->tick(dTime);
        }

        SDL_Delay(1);
    }
}
void HappyEngine::drawLoop()
{
    m_pGraphicsEngine->init();
    m_pGame->load();

    m_Loaded = true;
    Uint32 prevTicks(SDL_GetTicks());
    SDL_Event event;
    while (m_Quit == false)
    {
        Uint32 ticks(SDL_GetTicks());
        float dTime((ticks - prevTicks) / 1000.0f);
        prevTicks = ticks;     

        while (SDL_PollEvent(&event)) //Events are window related ==> need to be in the same thread
        {
            switch (event.type)
            {
                case SDL_QUIT: m_Quit = true; break;
            }
        }            
        m_pControlsManager->tick(); //tick here because needs to be in this thread

        m_pContentManager->glThreadInvoke();

        m_pGame->draw(dTime);
        
        m_pGraphicsEngine->present();
    }
}

HappyEngine::pointer HappyEngine::getPointer()
{
    return s_pHappyEngine;
}


//SubEngines
graphics::GraphicsEngine* HappyEngine::getGraphicsEngine() const
{
    return m_pGraphicsEngine;
}
const io::ControlsManager* HappyEngine::getControls() const
{
    return m_pControlsManager;
}
physics::PhysicsEngine* HappyEngine::getPhysics() const
{
    return m_pPhysicsEngine;
}
content::ContentManager* HappyEngine::getContentManager() const
{
    return m_pContentManager;
}

} //end namespace
