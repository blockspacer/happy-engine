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
//Created: 19/08/2011
#include "StdAfx.h" 

#include "PhysicsEngine.h"
#include "HeAssert.h"
#include "SDL.h"
#include "HappyTypes.h"
#include "boost/bind.hpp"
#include "MathConstants.h"
#include "HappyNew.h"
#include "PxCudaContextManager.h"
#include "PxProfileZoneManager.h"
#include "PhysicsCarManager.h"


namespace he {
namespace px {

PhysicsEngine::PhysicsEngine(): m_pPhysXSDK(nullptr), m_pScene(nullptr), 
                            m_pCpuDispatcher(nullptr), m_pCudaContextManager(nullptr), 
                            m_pAllocator(NEW HappyPhysicsAllocator()), m_pErrorCallback(NEW err::HappyPhysicsErrorCallback()),
                            m_Simulate(false), m_pMaterials(NEW ct::AssetContainer<physx::PxMaterial*>()),
                            m_pCarManager(NEW PhysicsCarManager())
{
    bool memDebug(false);
    #if _DEBUG || DEBUG
        memDebug = true;
    #endif
    
    m_pPhysXSDK = PxCreatePhysics(PX_PHYSICS_VERSION, *m_pAllocator, *m_pErrorCallback, 
                                    physx::PxTolerancesScale(), memDebug);
    
    ASSERT(m_pPhysXSDK != nullptr, "init of physX failed");
   
    if (!PxInitExtensions(*m_pPhysXSDK))
        ASSERT("PxInitExtensions failed!");

//#if _DEBUG || DEBUG
    std::cout << "connecting to PVD\n";
    PVD::PvdConnection* pConnection(physx::PxExtensionVisualDebugger::connect(m_pPhysXSDK->getPvdConnectionManager(), "localhost", 5425, 100, true));
    if (pConnection == nullptr)
        std::cout << "    NOT CONNECTED!\n";
    else
        std::cout << "    CONNECTED!\n";
//#endif

    createScene();

}
void PhysicsEngine::createScene()
{
    ASSERT(m_pPhysXSDK != nullptr, "m_pPhysXSDK == null");
    physx::PxSceneDesc sceneDesc(m_pPhysXSDK->getTolerancesScale());

    sceneDesc.gravity = physx::PxVec3(0.0f, -9.81f, 0.0f);

    if(!sceneDesc.cpuDispatcher)
    {
        std::cout << "PhysX using Cpu - " << SDL_GetCPUCount() << " cores\n";
        m_pCpuDispatcher = physx::PxDefaultCpuDispatcherCreate(SDL_GetCPUCount());

        ASSERT(m_pCpuDispatcher != nullptr, "PxDefaultCpuDispatcherCreate failed!");

        sceneDesc.cpuDispatcher = m_pCpuDispatcher;
    }

    if(!sceneDesc.filterShader)
        sceneDesc.filterShader = physx::PxDefaultSimulationFilterShader;

    #ifdef PX_WINDOWS
    #ifndef _DEBUG
    physx::pxtask::CudaContextManagerDesc cudaDesc;
    m_pCudaContextManager = physx::pxtask::createCudaContextManager(cudaDesc, &m_pPhysXSDK->getProfileZoneManager());
    if(!sceneDesc.gpuDispatcher && m_pCudaContextManager != nullptr)
    {
        std::cout << "PhysX using Gpu - " << m_pCudaContextManager->getDeviceName() << ", " << 
            m_pCudaContextManager->getMultiprocessorCount() << " cores @" << m_pCudaContextManager->getClockRate() << "\n";
        sceneDesc.gpuDispatcher = m_pCudaContextManager->getGpuDispatcher();
    }
    #endif
    #endif

    m_pScene = m_pPhysXSDK->createScene(sceneDesc);
    ASSERT(m_pScene != nullptr, "createScene failed!");

    physx::PxRigidStatic* plane = m_pPhysXSDK->createRigidStatic(physx::PxTransform(physx::PxVec3(0, -20, 0), physx::PxQuat(piOverTwo, physx::PxVec3(0, 0, 1))));
    ASSERT(plane != nullptr, "");
    physx::PxShape* pShape = plane->createShape(physx::PxPlaneGeometry(), *m_pPhysXSDK->createMaterial(0.3f, 0.6f, 0.2f) );
    ASSERT(pShape != nullptr, "");
    m_pScene->addActor(*plane);

}


PhysicsEngine::~PhysicsEngine()
{
    if (m_Simulate)
        stopSimulation();

    m_pScene->fetchResults(true);
    m_pScene->release();
    delete m_pMaterials;
    delete m_pCarManager;

    m_pCpuDispatcher->release();
    if (m_pCudaContextManager != nullptr)
        m_pCudaContextManager->release();
    m_pPhysXSDK->release();
    delete m_pAllocator;
    delete m_pErrorCallback;
}

void PhysicsEngine::startSimulation()
{
    m_Timer = 0.0f;
    m_Simulate = true;
    m_PhysXThread = boost::thread(boost::bind(&PhysicsEngine::physXThread, this));
}
void PhysicsEngine::stopSimulation()
{
    m_Simulate = false;
    m_PhysXThread.join();
}
void PhysicsEngine::tick(float /*dTime*/)
{

}
void PhysicsEngine::physXThread()
{
    const physx::PxReal fixedStep(1.0f / 120.0f);
    boost::chrono::high_resolution_clock::time_point m_PrevTime(boost::chrono::high_resolution_clock::now());
    while (m_Simulate)
    {
        boost::chrono::high_resolution_clock::duration elapsedTime(boost::chrono::high_resolution_clock::now() - m_PrevTime);
        m_PrevTime = boost::chrono::high_resolution_clock::now();
        float dTime(elapsedTime.count() / static_cast<float>(boost::nano::den));
        //PROFILER_BEGIN("PhysicsEngine::tick");
        //m_Timer += dTime;
        //if (m_Timer >= s_fixedStep)
        //{
        //    m_Timer -= s_fixedStep;

            m_pScene->fetchResults(true);
            m_pScene->simulate(fixedStep);
            m_pCarManager->tick(fixedStep);
            //}
        //PROFILER_END("PhysicsEngine::tick");
        boost::this_thread::sleep(boost::posix_time::milliseconds(static_cast<int64_t>((fixedStep - (dTime - fixedStep))*boost::milli::den)));
    }
}


physx::PxPhysics* PhysicsEngine::getSDK() const
{
    return m_pPhysXSDK;
}
physx::PxScene* PhysicsEngine::getScene() const
{
    return m_pScene;
}

physx::PxMaterial* PhysicsEngine::createMaterial( float staticFriction, float dynamicFriction, float restitution )
{
    std::stringstream key;
    key << staticFriction << " " << dynamicFriction << " " << restitution;
    if (m_pMaterials->isAssetPresent(key.str()))
    {
        return m_pMaterials->getAsset(key.str());
    }
    else
    {
        physx::PxMaterial* pMat(m_pPhysXSDK->createMaterial(staticFriction, dynamicFriction, restitution));
        if(pMat == nullptr)
        {
            CONSOLE->addMessage("physx error: createMaterial failed", CMSG_TYPE_ERROR);
        }
        m_pMaterials->addAsset(key.str(), pMat);
        return pMat;
    }
}

PhysicsCarManager* PhysicsEngine::getCarManager() const
{
    return m_pCarManager;
}

const px::PhysicsMaterial& PhysicsEngine::getDriveableMaterial( byte id )
{
    return m_pCarManager->getFrictionTable()->getMaterial(id);
}


} } //end namespace