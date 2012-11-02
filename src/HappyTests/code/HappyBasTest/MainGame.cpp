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
//Created: 20/03/2012
#include "HappyBasTestPCH.h" 

#include "MainGame.h"

#include "GraphicsEngine.h"
#include "CameraManager.h"
#include "ControlsManager.h"

#include "ModelMesh.h"

#include "FlyCamera.h"
#include "FPSGraph.h"

#include "Entity.h"
#include "ModelComponent.h"
#include "ContentManager.h"

#include "LightManager.h"
#include "SkyBox.h"

#include "LightComponent.h"
#include "DirectionalLight.h"
#include "Random.h"

#include "Scene.h"
#include "View.h"
#include "Window.h"

#include "IKeyboard.h"
#include "Renderer2D.h"
#include "ShapeRenderer.h"
#include "MessageBox.h"
#include "Canvas2D.h"
#include "SpotLight.h"
#include "PointLight.h"
#include "Font.h"

#define CONE_VERTICES 16
#define NUM_MOVING_ENTITIES 200

//#define ENABLE_WINDOW2

namespace ht {

he::Random MainGame::s_Random(0);

MainGame::MainGame()
    : m_FpsGraph(nullptr)
    , m_SkyBox(nullptr)
    , m_SpinShadows(false)
    , m_MovingEntityFase(0)
    , m_ShowDebugMesh(true)
{
    for (size_t i(0); i < NUM_MOVING_ENTITIES; ++i)
    {
        MovingEntityRandomness r;
        r.a = he::vec3(s_Random.nextFloat(10, 100), s_Random.nextFloat(10, 100), s_Random.nextFloat(10, 100));
        r.b = he::vec3(s_Random.nextFloat(10, 100), s_Random.nextFloat(10, 100), s_Random.nextFloat(10, 100));
        r.c = he::vec3((float)s_Random.nextInt(1, 5), (float)s_Random.nextInt(1, 5), (float)s_Random.nextInt(1, 5));
        m_MovingEntityRandomness.push_back(r);
    }
}


MainGame::~MainGame()
{
    m_DebugMesh->release();
    delete m_FpsGraph;
    delete m_SkyBox;
    std::for_each(m_EntityList.cbegin(), m_EntityList.cend(), [&](he::ge::Entity* entity)
    {
        delete entity;     
    });

    CONSOLE->setView(nullptr);
    GRAPHICS->removeView(m_View);
    GRAPHICS->removeWindow(m_Window);

#ifdef ENABLE_WINDOW2
    GRAPHICS->removeView(m_View2);
    GRAPHICS->removeWindow(m_Window2);
#endif

    m_Scene->getCameraManager()->deleteAllCameras();
    GRAPHICS->removeScene(m_Scene);
}

void MainGame::init()
{
    m_View = GRAPHICS->createView3D();
    m_Scene = GRAPHICS->createScene();
    m_Window = GRAPHICS->createWindow();

#ifdef ENABLE_WINDOW2
    m_View2 = GRAPHICS->createView3D();
    m_Window2 = GRAPHICS->createWindow();
#endif

    m_Window->setResizable(true);
    m_Window->setVSync(false);
    m_Window->setWindowDimension(1280, 720);
    m_Window->setWindowTitle("HappyBasTest");
    he::eventCallback0<void> quitHandler(boost::bind(&he::HappyEngine::quit, HAPPYENGINE));
    m_Window->Closed += quitHandler;
    m_Window->create();
}

void MainGame::load()
{
    using namespace he;

    #pragma region Window/View/Scene
    he::gfx::RenderSettings settings;
    settings.enableDeferred = true;
    settings.enablePost = true;

    settings.lightingSettings.enableLighting = true;
    settings.lightingSettings.enableNormalMap = true;
    settings.lightingSettings.enableShadows = false;
    settings.lightingSettings.enableSpecular = true;

    settings.shadowSettings.shadowMult = 2;

    settings.postSettings.shaderSettings.enableAO = false;
    settings.postSettings.shaderSettings.enableBloom = true;
    settings.postSettings.shaderSettings.enableDepthEdgeDetect = false;
    settings.postSettings.shaderSettings.enableFog = true;
    settings.postSettings.shaderSettings.enableHDR = true;
    settings.postSettings.shaderSettings.enableNormalEdgeDetect = false;
    settings.postSettings.shaderSettings.enableVignette = true;

    CONTENT->setRenderSettings(settings);

    m_View->setScene(m_Scene);
    m_View->setWindow(m_Window);
    m_View->setRelativeViewport(he::RectF(0, 0, 1.0f, 1.0f));
    m_View->init(settings);
    

#ifdef ENABLE_WINDOW2
    m_Window2->setResizable(true);
    m_Window2->setVSync(false);
    m_Window2->setWindowDimension(720, 720);
    m_Window2->setWindowTitle("HappyBasTest - 2");
    m_Window2->create();
    
    m_View2->setScene(m_Scene);
    m_View2->setWindow(m_Window2);
    m_View2->setRelativeViewport(he::RectF(0, 0, 1.0f, 1.0f));
    m_View2->init(settings);
#endif
    
    CONSOLE->setView(m_View);
    m_View->get2DRenderer()->attachToRender(this);
  
    FlyCamera* flyCamera = NEW FlyCamera();
    m_Scene->getCameraManager()->addCamera("default", flyCamera);
    flyCamera->setLens(1280/720.0f, piOverTwo / 3.0f * 2.0f, 1.0f, 100.0f);
    flyCamera->lookAt(vec3(), vec3(1, 0, 0), vec3(0, 1, 0));
    m_View->setCamera("default");

#ifdef ENABLE_WINDOW2
    FlyCamera* flyCamera2 = NEW FlyCamera();
    m_Scene->getCameraManager()->addCamera("default2", flyCamera2);
    flyCamera2->setLens(1280/720.0f, piOverTwo / 3.0f * 2.0f, 1.0f, 1000.0f);
    flyCamera2->lookAt(vec3(), vec3(1, 0, 0), vec3(0, 1, 0));
    m_View2->setCamera("default2");
#endif
    
    
    m_FpsGraph = NEW tools::FPSGraph();
    m_FpsGraph->setView(m_View);
    m_FpsGraph->setType(tools::FPSGraph::Type_TextOnly);
    //
    //m_View->get2DRenderer()->attachToRender(m_FpsGraph);
    
    //CONSOLE->setView(m_View);
    m_View->get2DRenderer()->attachToRender(CONSOLE);

    //m_View->getShapeRenderer()->attachToRenderer(this);
#ifdef ENABLE_WINDOW2
    m_View2->getShapeRenderer()->attachToRenderer(this);
#endif
    
    #pragma endregion
    
    #pragma region Scene
    ge::Entity* scene(NEW ge::Entity());
    scene->init(m_Scene);
    ge::ModelComponent* modelComp(NEW ge::ModelComponent());
    scene->addComponent(modelComp);
    modelComp->setModelMeshAndMaterial("testScene3.material", "testScene3.binobj");

    modelComp = NEW ge::ModelComponent();
    modelComp->setModelMeshAndMaterial("testScene4.material", "testScene4.binobj");
    modelComp->setLocalTranslate(he::vec3(1, 1, 1));
    modelComp->setLocalRotate(he::mat33::createRotation3D(vec3(0, 1, 0), he::pi));
    modelComp->setLocalScale(vec3(100.0f, 100.0f, 100.0f));
    scene->addComponent(modelComp);
    
    m_EntityList.push_back(scene);

    //for (size_t i(0); i < NUM_MOVING_ENTITIES; ++i)
    //{
    //    ge::Entity* entity(NEW he::ge::Entity());
    //    entity->init(m_Scene);
    //    modelComp = NEW ge::ModelComponent();
    //    modelComp->setModelMeshAndMaterial("cube.material", "cube.binobj");
    //    entity->addComponent(modelComp);
    //    m_MovingEntityList.push_back(entity);
    //    m_EntityList.push_back(entity);
    //
    //    const MovingEntityRandomness& r(m_MovingEntityRandomness[i]);
    //    m_MovingEntityList[i]->setLocalTranslate(
    //        he::vec3(pow(cos(m_MovingEntityFase), r.c.x) * r.a.x + r.b.x, 
    //                    pow(sin(m_MovingEntityFase), r.c.y) * r.a.y + r.b.y, 
    //                    pow(cos(m_MovingEntityFase), r.c.z) * r.a.z + r.b.z));
    //}

    #pragma endregion
    
    #pragma region Lights
    m_Scene->getLightManager()->setAmbientLight(Color(0.9f, 1.0f, 1.0f, 1.0f), 0.6f);
    m_Scene->getLightManager()->setDirectionalLight(normalize(vec3(-4.0f, 5.f, 1.0f)), Color(1.0f, 0.9f, 0.8f, 1.0f), 0.0f);

/*
    for (size_t i(0); i < 5; ++i)
    {
        vec3 direction(rand() / (float)RAND_MAX * 2.0f - 1.0f, rand() / (float)RAND_MAX * 2.0f - 1.0f, rand() / (float)RAND_MAX * 2.0f - 1.0f);
        direction = normalize(direction);
        float len(33.0f);
        vec3 pos(direction * len + vec3(20, 20, 20));

        ge::PointLightComponent* pTempPointLightComp(NEW ge::PointLightComponent());
        scene->addComponent(pTempPointLightComp);
        pTempPointLightComp->setLocalTranslate(pos);
        pTempPointLightComp->setMultiplier(0.5f);
        pTempPointLightComp->setColor(Color((he::byte)(rand()%255), 128, 255, 255));
        pTempPointLightComp->setAttenuation(0, 15);

        modelComp = NEW ge::ModelComponent();
        modelComp->setModelMeshAndMaterial("cube.material", "cube.binobj");
        modelComp->setLocalTranslate(pos);
        scene->addComponent(modelComp);
    }*/

    m_DebugSpotLight = m_Scene->getLightManager()->addSpotLight();
    m_DebugSpotLight->setLocalTranslate(vec3(-42.71f, 10.20f, 30.74f));
    m_DebugSpotLight->setDirection(vec3(-0.70f, -0.67f, -0.27f));
    m_DebugSpotLight->setMultiplier(2.0f);
    m_DebugSpotLight->setAttenuation(1.0f, 20.0f);
    m_DebugSpotLight->setFov(he::piOverTwo);
    m_DebugSpotLight->setColor(he::Color(1.0f, 0.4f, 0.4f));
    m_DebugSpotLight->setShadowResolution(gfx::ShadowResolution_512);

    he::gfx::SpotLight* spotlight = m_Scene->getLightManager()->addSpotLight();
    spotlight->setLocalTranslate(vec3(-35.32f, 6.04f, 31.85f));
    spotlight->setDirection(vec3(0.80f, 0.13f, -0.58f));
    spotlight->setMultiplier(2.0f);
    spotlight->setAttenuation(1.0f, 30.0f);
    spotlight->setFov(he::piOverFour);
    spotlight->setColor(he::Color(0.4f, 0.4f, 1.0f));
    spotlight->setShadowResolution(gfx::ShadowResolution_128);

    //he::gfx::PointLight* pointlight(m_Scene->getLightManager()->addPointLight());
    //pointlight->setLocalTranslate(vec3(-41.91f, 11.20f, 34.58f));
    //pointlight->setMultiplier(3);
    //pointlight->setAttenuation(1.0f, 10.0f);
    //pointlight->setColor(he::Color(1.0f, 0.4f, 0.4f));

    #pragma endregion
    
    #pragma region Camera Debug Shape
    ResourceFactory<gfx::ModelMesh>* meshFactory(ResourceFactory<gfx::ModelMesh>::getInstance());
    gfx::BufferLayout debugCameraLayout;
    debugCameraLayout.addElement(gfx::BufferElement(0, gfx::BufferElement::Type_Vec3, gfx::BufferElement::Usage_Position, sizeof(vec3), 0));
    m_DebugMesh = meshFactory->get(meshFactory->create());
    m_DebugMesh->setName("Debug Mesh");
    m_DebugMesh->init(debugCameraLayout, he::gfx::MeshDrawMode_Lines);
    m_DebugMesh->setVertices(nullptr, 0, he::gfx::MeshUsage_Dynamic);
    m_DebugMesh->setIndices(nullptr, 0, he::gfx::IndexStride_UInt, he::gfx::MeshUsage_Dynamic);
    m_DebugMesh->setLoaded();
    #pragma endregion
    
    gfx::Font* font(CONTENT->getDefaultFont(14));
    m_DebugText.setFont(font);
    font->release();

    m_SkyBox = NEW he::gfx::SkyBox();
    m_SkyBox->load("");
    m_SkyBox->attachToScene(m_Scene);

    he::MessageBox::showExt("Test Messagebox", 
        "Hallo\n\nIk test even of deze messagebox wel goed werk\n\n* 1\n* 2\n* 3", MessageBoxIcon_Warning, 
        "Het werkt!", "Ik weet het niet", "Werkt niet");
    
    PROFILER->setView(m_View);
}

void MainGame::tick( float dTime )
{
    he::ge::Game::tick(dTime);

    m_MovingEntityFase += dTime / 2.0f;
    if (m_MovingEntityFase >= he::twoPi)
        m_MovingEntityFase -= he::twoPi;

    m_DebugSpotLight->setLocalTranslate(he::vec3(-42.71f, 10.20f, 30.74f) + 
        he::vec3(pow(cos(m_MovingEntityFase), 4) * 5, 
                 pow(sin(m_MovingEntityFase), 3) * 3, 
                 pow(cos(m_MovingEntityFase), 2) * 3));
    m_DebugSpotLight->setLocalRotate(he::mat33::createRotation3D(he::vec3::up, m_MovingEntityFase));
    
    /*for (size_t i(0); i < NUM_MOVING_ENTITIES; ++i)
    {
        const MovingEntityRandomness& r(m_MovingEntityRandomness[i]);
        m_MovingEntityList[i]->setLocalTranslate(
            he::vec3(pow(cos(m_MovingEntityFase), r.c.x) * r.a.x + r.b.x, 
                     pow(sin(m_MovingEntityFase), r.c.y) * r.a.y + r.b.y, 
                     pow(cos(m_MovingEntityFase), r.c.z) * r.a.z + r.b.z));
    }*/

    if (CONTROLS->getKeyboard()->isKeyPressed(he::io::Key_Return))
    {
        he::gfx::SpotLight* spotlight = m_Scene->getLightManager()->addSpotLight();
        spotlight->setLocalTranslate(m_View->getCamera()->getPosition());
        spotlight->setDirection(m_View->getCamera()->getLook());
        spotlight->setMultiplier(s_Random.nextFloat(1.0f, 5.0));
        spotlight->setAttenuation(1.0f, s_Random.nextFloat(10.0f, 20.0f));
        spotlight->setFov(he::piOverFour * s_Random.nextFloat(0.5f, 4.0f));
        he::vec3 color(he::normalize(he::vec3(s_Random.nextFloat(0, 1), s_Random.nextFloat(0, 1), s_Random.nextFloat(0, 1))));
        spotlight->setColor(he::Color(color.x, color.y, color.z, 1.0f));
        spotlight->setShadowResolution(he::gfx::ShadowResolution_256);
    }

    if (m_ShowDebugMesh)
        fillDebugMeshes();
    m_FpsGraph->tick(dTime);
}

void MainGame::fillDebugMeshes()
{
    HIERARCHICAL_PROFILE(__HE_FUNCTION__);
    using namespace he;
    gfx::CameraPerspective* camera(m_View->getCamera());
    
    const Frustum& frustum(camera->getBound().getFrustum());
    Frustum::generateFrustumIndices<uint>(m_View->getDebugIndices(), (uint)m_View->getDebugVertices().size());
    frustum.generateFrustumPoints(m_View->getDebugVertices());

    const Cone& cone(camera->getBound().getCone());
    Cone::generateConeIndices<uint>(CONE_VERTICES, (uint)m_View->getDebugVertices().size(), m_View->getDebugIndices());
    cone.generateConeVertices(CONE_VERTICES, m_View->getDebugVertices());

    m_DebugMesh->setVertices(m_View->getDebugVertices().data(), m_View->getDebugVertices().size(), gfx::MeshUsage_Dynamic);
    m_DebugMesh->setIndices(m_View->getDebugIndices().data(), m_View->getDebugIndices().size(), gfx::IndexStride_UInt, gfx::MeshUsage_Dynamic);
}

void MainGame::drawShapes( he::gfx::ShapeRenderer* renderer )
{
    renderer->drawMeshColor(m_DebugMesh, he::mat44::Identity, he::Color(1.0f, 0, 0, 1)); 
}

void MainGame::draw2D(he::gfx::Canvas2D* canvas)
{
    he::gfx::CameraPerspective* camera(m_View->getCamera());
    const he::vec3& position(camera->getPosition());
    const he::vec3& look(camera->getLook());
    
    m_DebugText.clear();

    char buff[100];
    sprintf(buff, "Position: %.2f, %.2f, %.2f\0", position.x, position.y, position.z);
    m_DebugText.addLine(buff);

    sprintf(buff, "Look: %.2f, %.2f, %.2f\0", look.x, look.y, look.z);
    m_DebugText.addLine(buff);

    canvas->fillText(m_DebugText, he::vec2(12, 12));

    canvas->getRenderer2D()->drawTexture2DToScreen(m_DebugSpotLight->getShadowMap(), he::vec2(12, 300), false, he::vec2(256, 256));
}

} //end namespace
