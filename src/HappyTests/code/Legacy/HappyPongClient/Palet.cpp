//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
#include "HappyPongClientPCH.h" 

#include "Palet.h"

#include "ControlsManager.h"
#include "IKeyboard.h"
#include "ContentManager.h"

#include "ModelComponent.h"
#include "ModelMesh.h"

#include "Material.h"
#include "Game.h"
#include "LightFlashComponent.h"

#include "MainGame.h"
#include "Ball.h"

#include "NetworkSerializer.h"

namespace hpc {

#define AI_REACTION_TIME 0.0333f

IMPLEMENT_OBJECT(Palet)

Palet::Palet(): 
    m_Speed(50.0f), 
    m_PaletDim(1.0f, 5.0f),
    m_Player(0),
    m_Ai(false),
    m_GoDown(false),
    m_GoUp(false),
    m_LightFlashAddPointComponent(nullptr),
    m_FlashLightScore(nullptr),
    m_AiActionTime(0.0f)
{
}

void Palet::init( he::gfx::Scene* scene )
{
    Entity::init(scene);

    MainGame* game(static_cast<MainGame*>(GAME));

    he::ge::ModelComponent* model(NEW(he::ge::ModelComponent)());
    model->setModelMeshAndMaterial("pong/palet.material", "pong/palet.binobj");
    model->setLocalScale(he::vec3(100, 100, 100));
    addComponent(model);

    m_LightFlashComponent = NEW(LightFlashComponent)();
    addComponent(m_LightFlashComponent);
    m_LightFlashComponent->setAttenuation(1.0f, 50);
    if (m_Player == 0)
        m_LightFlashComponent->setColor(he::Color(0.2f, 1.0f, 0.2f));
    else
        m_LightFlashComponent->setColor(he::Color(0.2f, 0.2f, 1.0f));
    m_LightFlashComponent->setLocalTranslate(he::vec3(0.0f, 1.5f, 0));
    m_LightFlashComponent->setFlashMultiplier(5);
    m_LightFlashComponent->setNormalMultiplier(2);
    m_LightFlashComponent->setFlashDuration(0.5f);

    HE_ASSERT(m_FlashLightScore == nullptr, "Entity is not nullptr!");
    m_FlashLightScore = NEW(Entity)();
    m_FlashLightScore->init(scene);
    m_LightFlashAddPointComponent = NEW(LightFlashComponent)();
    m_FlashLightScore->addComponent(m_LightFlashAddPointComponent);
    m_LightFlashAddPointComponent->setAttenuation(1.0f, 100);
    if (m_Player == 0)
    {
        m_LightFlashAddPointComponent->setLocalTranslate(he::vec3(-game->getBoardDimension().x/2, 0.5f, 0));
        m_LightFlashAddPointComponent->setColor(he::Color(0.2f, 1.0f, 0.2f));
    }
    else
    {
        m_LightFlashAddPointComponent->setLocalTranslate(he::vec3(game->getBoardDimension().x/2, 0.5f, 0));
        m_LightFlashAddPointComponent->setColor(he::Color(0.2f, 0.2f, 1.0f));
    }
    m_LightFlashAddPointComponent->setFlashMultiplier(50);
    m_LightFlashAddPointComponent->setNormalMultiplier(0);
    m_LightFlashAddPointComponent->setFlashDuration(1.0f);

    GAME->addToTickList(this);
}


Palet::~Palet()
{
    HEDelete(m_FlashLightScore);
    GAME->removeFromTickList(this);
}

void Palet::tick( float dTime )
{
    MainGame* game(static_cast<MainGame*>(GAME));

    if (NETWORK->getNetworkId() == getOwner())
    {
        if (m_Ai)
        {
            m_AiActionTime -= dTime;
            Ball* ball(game->getBall());
            if (ball != nullptr && m_AiActionTime <= 0.0f)
            {
                m_AiActionTime = AI_REACTION_TIME;
                float moveToZ(0.0f);
                if (he::dot(ball->getVelocity(), ball->getPosition() - m_Position) < 0.0)
                { // Move to ball
                    moveToZ = ball->getPosition().z + ball->getVelocity().z * AI_REACTION_TIME;
                }
                else
                {// return to center
                }

                float epsilon(m_PaletDim.y / 8);
                if (fabs(m_MoveTo.z - moveToZ) > m_Speed * AI_REACTION_TIME)
                {
                    if (m_MoveTo.z < moveToZ - epsilon)
                    {
                        if (m_GoUp == false)
                        {
                            m_GoUp = true;
                            m_GoDown = false;
                            setSerializeDataDirty();
                        }
                    }                    
                    else if (m_MoveTo.z > moveToZ + epsilon)
                    {
                        if (m_GoDown == false)
                        {
                            m_GoUp = false;
                            m_GoDown = true;
                            setSerializeDataDirty();
                        }
                    }
                    else if (m_GoUp == true || m_GoDown == true)
                    {
                        m_GoUp = false;
                        m_GoDown = false;
                        setSerializeDataDirty();
                    }
                }
            }
        }
        else
        {
            he::io::Key up(he::io::Key_Up);
            he::io::Key down(he::io::Key_Down);
            if (CONTROLS->getKeyboard()->isKeyDown(up) != m_GoUp)
            {
                m_GoUp = CONTROLS->getKeyboard()->isKeyDown(up);
                setSerializeDataDirty();
            }
            if (CONTROLS->getKeyboard()->isKeyDown(down) != m_GoDown)
            {
                m_GoDown = CONTROLS->getKeyboard()->isKeyDown(down);
                setSerializeDataDirty();
            }
        }
    }
    if (m_GoUp)
    {
        m_MoveTo.z += m_Speed * dTime;
        if (m_MoveTo.z + m_PaletDim.y / 2 > game->getBoardDimension().y / 2)
            m_MoveTo.z = game->getBoardDimension().y / 2 - m_PaletDim.y / 2;
    }
    if (m_GoDown)
    {
        m_MoveTo.z -= m_Speed * dTime;
        if (m_MoveTo.z - m_PaletDim.y / 2 <  -game->getBoardDimension().y / 2)
            m_MoveTo.z = -game->getBoardDimension().y / 2 + m_PaletDim.y / 2;
    }

    float len(he::length(m_MoveTo - m_Position));
    if (len > m_Speed * dTime)
        m_Position += (m_MoveTo - m_Position) / len * m_Speed * dTime;
    else
        m_Position = m_MoveTo;


    setLocalTranslate(m_Position);
}

const he::vec2& Palet::getDimension() const
{
    return m_PaletDim;
}

const he::vec3& Palet::getPosition() const
{
    return m_Position;
}

void Palet::hitByBall()
{
    m_LightFlashComponent->flash();
}

void Palet::addPoint()
{
    m_LightFlashAddPointComponent->flash();
}

void Palet::serializeCreate( he::net::NetworkStream* /*stream*/ ) const
{
}

bool Palet::deserializeCreate( he::net::NetworkStream* stream )
{
    stream->Read(m_Position);
    m_MoveTo = m_Position;
    stream->Read(m_Player);

    init(static_cast<MainGame*>(GAME)->getActiveScene());

    return true;
}

void Palet::serializeRemove( he::net::NetworkStream* /*stream*/ ) const
{
}

bool Palet::deserializeRemove( he::net::NetworkStream* /*stream*/ )
{
    return true;
}

void Palet::serialize(const he::net::NetworkSerializer& serializer )
{
    serializer.serializeVariable(m_MoveTo);
    serializer.serializeVariable(m_GoUp);
    serializer.serializeVariable(m_GoDown);
    
    setSerializeDataDirty(false);
}

void Palet::deserialize(const he::net::NetworkDeserializer& deserializer )
{
    deserializer.deserializeVariable(m_MoveTo);
    deserializer.deserializeVariable(m_GoUp);
    deserializer.deserializeVariable(m_GoDown);
    //tick(deserializer.getDTime());
}

} //end namespace