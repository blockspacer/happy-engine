//HappyTest Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyTest.
//
//    HappyTest is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyTest is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyTest.  If not, see <http://www.gnu.org/licenses/>.

#include "MainGame.h"

#include <vector>
#include <sstream>
#include <iostream>

#include "Matrix.h"
#include "Vector3.h"
#include "MathConstants.h"
#include "HappyEngine.h"
#include "MathFunctions.h"

#include "IniReader.h"
#include "FileNotFoundException.h"
#include "Circle.h"

#include "HappyNew.h"

namespace happytest {

MainGame::MainGame() : m_pDeferred3DRenderer(nullptr), m_pTestObject(nullptr), m_BackgroundIndex(0),
                       m_DrawTimer(0), m_UpdateTimer(0), m_pDeferredPreEffect(NEW DeferredPreEffect()),                   
					   m_pServer(nullptr), m_pClient(nullptr), m_pFPSGraph(NEW happyengine::tools::FPSGraph()),
					   m_pCamera(nullptr), m_SplashAlpha(1.0f), m_pGroundPlane(NEW GroundPlane())
{
    using namespace happyengine;
    m_BackgroundColors[0] = Color((byte)10, (byte)130, (byte)131, (byte)255);
    m_BackgroundColors[1] = Color((byte)122, (byte)186, (byte)122, (byte)255);
    m_BackgroundColors[2] = Color((byte)255, (byte)127, (byte)80, (byte)255);
    m_BackgroundColors[3] = Color((byte)255, (byte)165, (byte)0, (byte)255);
    m_BackgroundColors[4] = Color((byte)30, (byte)144, (byte)255, (byte)255);
}


MainGame::~MainGame()
{
    delete m_pDeferred3DRenderer;
    delete m_pTestObject;
    delete m_pDeferredPreEffect;
    std::for_each(m_Bullets.cbegin(), m_Bullets.cend(), [&](TestBullet* pBullet)
    {
        delete pBullet;
    });

    delete m_pServer;
    delete m_pClient;

	delete m_pFPSGraph;
	delete m_pCamera;

    delete m_pGroundPlane;

    NETWORK->stop();
}

void MainGame::init()
{
    using namespace happyengine;
    io::IniReader reader;
    try { reader.open("../data/settings.ini"); }
    catch (error::FileNotFoundException& e)
    { std::wcout << e.getMsg() << "\n"; }
    if (reader.isOpen())
    {
        math::Vector2 windowDim(reader.readVector2(L"Window", L"dimension", math::Vector2(1280, 720)));
        GRAPHICS->setScreenDimension(static_cast<int>(windowDim.x), static_cast<int>(windowDim.y));
        GRAPHICS->setWindowTitle(reader.readString(L"Window", L"title", "Test"));

        math::Vector4 bColor(reader.readVector4(L"Background", L"color"));
        GRAPHICS->setBackgroundColor(Color(bColor.x, bColor.y, bColor.z, bColor.w));

        GRAPHICS->setVSync(reader.readBool(L"Graphics", L"vsync", true));
        GRAPHICS->toggleFullscreen(reader.readBool(L"Graphics", L"fullscreen"));
    }
}
void MainGame::load()
{
    using namespace happyengine;

	m_SplashImage = CONTENT->asyncLoadTexture("../data/textures/happy_splash.png");

    PHYSICS->startSimulation();

	m_pCamera = NEW FlyCamera(GRAPHICS->getScreenWidth(), GRAPHICS->getScreenHeight());
	m_pCamera->lookAt(math::Vector3(-5, 5, -4), math::Vector3(0, 0, 0), math::Vector3(0, 1, 0));
	m_pCamera->setLens(16.0f/9.0f,math::piOverFour,1.0f,100.0f);
	m_pCamera->setActive(true);
	//m_pCamera->controllable(false);

    m_pDeferred3DRenderer = NEW graphics::Deferred3DRenderer();
    m_pDeferred3DRenderer->getLightManager()->addPointLight(math::Vector3(-1, 0, -1), Color((byte)255, 50, 50, 255), 2.0f, 1, 10);
    m_pSpotLight = m_pDeferred3DRenderer->getLightManager()->addSpotLight(math::Vector3(-1, 0, -1), math::Vector3(-1, 0, 0), Color((byte)255, 255, 200, 255), 3.0f, sinf(math::piOverFour/2), 1, 30);
    m_pDeferred3DRenderer->getLightManager()->addDirectionalLight(math::Vector3(0, -1, 0), Color((byte)150, 200, 255, 255), 0.5f);
    m_pTestObject = NEW TestObject();
    m_pTestObject->load();

    /*TestBullet* pBullet(NEW TestBullet());
    pBullet->load();
    m_Bullets.push_back(pBullet);*/

    m_pDeferredPreEffect->load();

	m_TestImage = CONTENT->asyncLoadTexture("../data/textures/v8_vantage_color.png");
	
	m_SplashTimer.Reset();

	happyengine::content::FontLoader fontLoader;
    fontLoader.load("../data/fonts/Ubuntu-Regular.ttf", 12, m_pFont);

    m_pGroundPlane->load();
}
void MainGame::tick(float dTime)
{
    m_UpdateTimer += dTime;

	m_pCamera->tick(dTime);

    if (m_UpdateTimer >= 1.0f)
    {
        std::cout << "update fps: " << 1.0f/dTime << "\n";
        m_UpdateTimer -= 1.0f;
    }

    if (CONTROLS->getKeyboard()->isKeyPressed(happyengine::io::Key_Escape))
        HAPPYENGINE->quit();

    m_pTestObject->tick(dTime);
    
    if (m_pClient == nullptr && m_pServer == nullptr)
    {
        if (CONTROLS->getKeyboard()->isKeyPressed(happyengine::io::Key_F11))
        {
            std::cout << "Starting server\n";
            m_pServer = NEW MyServer();
            m_pServer->start(30000, 16);
            NETWORK->start();
        }
        else if (CONTROLS->getKeyboard()->isKeyPressed(happyengine::io::Key_F12))
        {
            std::cout << "Starting client\n";
            m_pClient = NEW MyClient();
            m_pClient->asyncConnect("78.21.63.179", 30000);
            NETWORK->start();
        }
    }

    std::for_each(m_Bullets.cbegin(), m_Bullets.cend(), [&dTime](TestBullet* pBullet)
    {
        pBullet->tick(dTime);
    });

    if (CONTROLS->getKeyboard()->isKeyPressed(happyengine::io::Key_Space))
    {
        TestBullet* pBullet(NEW TestBullet(m_pCamera->getPosition(), m_pCamera->getLook() * 20));
        pBullet->load();
        m_Bullets.push_back(pBullet);
        std::cout << m_Bullets.size() << "\n";
    }

    m_pSpotLight->position = m_pCamera->getPosition();
    m_pSpotLight->direction = -happyengine::math::normalize(m_pCamera->getLook());

	m_pFPSGraph->tick(dTime, 0.5f);
}
void MainGame::draw(float dTime)
{
	using namespace happyengine;
	using namespace graphics;
	using namespace math;

    m_DrawTimer += dTime;

    if (m_DrawTimer >= 1.0f)
    {
        std::cout << "draw fps: " << 1.0f/dTime << "\n";
        m_DrawTimer -= 1.0f;
    }

    if (CONTROLS->getKeyboard()->isKeyPressed(happyengine::io::Key_Return))
    {
        ++m_BackgroundIndex;
        if (m_BackgroundIndex > 4)
            m_BackgroundIndex = 0;
        GRAPHICS->setBackgroundColor(m_BackgroundColors[m_BackgroundIndex]);
    }

    GRAPHICS->clearAll();


	m_SplashTimer.Tick();

	if(m_SplashTimer.GetGameTime() > 3.1f && m_SplashAlpha > 0.0f)
	{
		m_SplashAlpha -= 0.02f;
	}

	if (m_SplashTimer.GetGameTime() > 4.0f)
	{
		m_pDeferred3DRenderer->begin();
		m_pDeferredPreEffect->begin();

			m_pTestObject->draw(m_pDeferred3DRenderer, m_pDeferredPreEffect, dTime, m_pCamera);

			std::for_each(m_Bullets.cbegin(), m_Bullets.cend(), [&](TestBullet* pBullet)
			{
				pBullet->draw(m_pDeferred3DRenderer, m_pDeferredPreEffect, m_pCamera);
			});
            m_pGroundPlane->draw(m_pDeferred3DRenderer, m_pDeferredPreEffect, m_pCamera);
		m_pDeferredPreEffect->end();
		m_pDeferred3DRenderer->end(m_pCamera);

		// 2D test stuff
		HE2D->begin();
            
			//Matrix mat = Matrix::createRotation(Vector3(0,0,1), piOverFour);
			//HE2D->setTransformationMatrix(mat);

			//HE2D->setColor(1,1,1,0.5f);
			//HE2D->drawRectangle(Vector2(200,20), Vector2(500,50));

			//HE2D->drawTexture2D(Vector2(100,100), m_TestImage, Vector2(500,500));

			//HE2D->setAntiAliasing(true);	
		
			/*HE2D->setColor(0.6f,0.5f,0.2f);
			HE2D->setFontVerticalAlignment(FontVAlignment_Center);
			HE2D->drawText(Vector2(200,100), "Test", m_pFont);*/

			/*std::vector<Vector2> points;
			points.push_back(Vector2(10,10));
			points.push_back(Vector2(8,20));
			points.push_back(Vector2(30,50));
			points.push_back(Vector2(50,40));
			points.push_back(Vector2(50,20));
			points.push_back(Vector2(20,10));

			HE2D->setColor(1.0f,1.0f,1.0f);
			HE2D->fillPolygon(points, points.size());*/

			HE2D->setColor(1.0f,1.0f,1.0f);
			HE2D->drawEllipseInstanced(Vector2(150,150), Vector2(101,101));
	
			HE2D->setColor(1.0f,0.0f,0.0f,0.5f);
			HE2D->fillEllipseInstanced(Vector2(150,150), Vector2(100,100));

			//HE2D->setRotation(math::toRadians(45));

			/*HE2D->setColor(0.0f,1.0f,0.0f,0.5f);
			HE2D->fillRectangle(Vector2(50,200), Vector2(100,100));*/

			HE2D->setColor(1.0f,1.0f,1.0f,0.2f);

			for (int i = 0; i < 2; ++i)
			{
				HE2D->setRotation((float)i*45);
				HE2D->fillRectangleInstanced(Vector2(150,300), Vector2(50,50));
				HE2D->resetTransformation();
			}

			for (int i = 0; i < 2; ++i)
			{
				HE2D->setScale(Vector2(2.0f, 2.0f));
				HE2D->setRotation((float)i*45);
				HE2D->fillRectangleInstanced(Vector2(150,420), Vector2(50,50));
				HE2D->resetTransformation();
			}

			//HE2D->fillRectangleInstanced(math::Vector2(500,200), math::Vector2(50,50));
			//HE2D->resetTransformation();
			
			/*shapes::Circle c(Vector2(200, 200), 128);
			HE2D->drawEllipse(c.getPosition(), Vector2(c.getRadius()*2, c.getRadius()*2));

			Vector2 mPos(CONTROLS->getMouse()->getPosition());
			HE2D->drawEllipse(mPos, Vector2(8, 8));

			shapes::Circle c2((mPos + Vector2(200, 200))/2.0f, length(mPos - c.getPosition())/2.0f);
			std::vector<Vector2> tan(c.intersect(c2));
			HE2D->drawEllipse(c2.getPosition(), Vector2(c2.getRadius()*2, c2.getRadius()*2));*/

			/*if (tan.size() > 0)
			{
				std::vector<Vector2> line1;
				line1.push_back(mPos);
				line1.push_back(tan[0]);
				HE2D->drawPolygon(line1, 2);

				std::vector<Vector2> line2;
				line2.push_back(mPos);
				line2.push_back(tan[1]);
				HE2D->drawPolygon(line2, 2);
			}*/

			m_pFPSGraph->draw();

		HE2D->end();
	}
	else
	{
		HE2D->begin();

			//HE2D->setRotation(45);
			HE2D->drawTexture2D(m_SplashImage, Vector2(0,0), Vector2(0,0), m_SplashAlpha);
			//HE2D->drawTexture2D(m_SplashImage, Vector2(200,500), Vector2(500,200), 1.0f, RectF(200,500,500,200));
			//HE2D->resetTransformation();

		HE2D->end();
	}
}

} //end namespace