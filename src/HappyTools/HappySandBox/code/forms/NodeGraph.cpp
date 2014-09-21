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
#include "HappySandBoxPCH.h"
#include "NodeGraph.h"

#include "NodeGraphEnums.h"
#include "NodeGraphNode.h"
#include "NodeGraphNodeInput.h"
#include "NodeGraphNodeOutput.h"

#include <ContentManager.h>
#include <Canvas2D.h>
#include <GraphicsEngine.h>
#include <Renderer2D.h>
#include <View.h>
#include <ControlsManager.h>
#include <IMouse.h>
#include <IKeyboard.h>
#include <Font.h>
#include <Game.h>

#define ZOOM_STEP 0.0001f
#define ZOOM_MIN 0.1f
#define ZOOM_MAX 1.0f

namespace hs {

NodeGraph::NodeGraph(QWidget *parent)
: RenderWidget(parent)
, m_View(GRAPHICS->createView())
, m_2DRenderer(NEW he::gfx::Renderer2D())
, m_Active(false)
, m_Offset(0, 0)
, m_Scale(0.5f)
, m_GrabWorldPos(0.0f, 0.0f)
, m_State(State_Idle)
{
    QWidget::setMouseTracking(true);

    create(false);

    m_View->setWindow(this);
    m_View->addRenderPlugin(m_2DRenderer);
    he::gfx::CameraSettings settings;
    m_View->init(settings, true);

    m_2DRenderer->attachToRender(this);

    he::gui::Font* font(CONTENT->loadFont("Ubuntu-Medium.ttf", 14));
    m_DebugText.setFont(font);
    m_DebugText.setHorizontalAlignment(he::gui::Text::HAlignment_Left);
    font->release();

    he::eventCallback0<void> shownCallback(std::bind(&NodeGraph::activate, this));
    Shown += shownCallback;
    he::eventCallback0<void> hiddenCallback(std::bind(&NodeGraph::deactivate, this));
    Hidden += hiddenCallback;
}

NodeGraph::~NodeGraph()
{
    deactivate();

    m_2DRenderer->detachFromRender(this);

    GRAPHICS->removeView(m_View);
    delete m_2DRenderer;

    destroy();

    m_Nodes.forEach([](NodeGraphNode* node)
    {
        delete node;
    });
}

void NodeGraph::activate()
{
    if (!m_Active)
    {
        m_Active = true;
        GAME->addToTickList(this);
    }
}

void NodeGraph::deactivate()
{
    if (m_Active)
    {
        m_Active = false;
        GAME->removeFromTickList(this);
    }
}

he::vec2 NodeGraph::screenToWorldPos( const he::vec2& screenPos ) const
{
    return m_Offset + screenPos / m_Scale;
}

he::vec2 NodeGraph::worldToScreenPos( const he::vec2& worldPos ) const
{
    return (worldPos - m_Offset) * m_Scale;
}

void NodeGraph::tick( float dTime )
{
    updateStates(dTime);
    updateZoom(dTime);
}

void NodeGraph::updateZoom( const float /*dTime*/ )
{
    using namespace he;
    const io::IMouse* const mouse(CONTROLS->getMouse(getHandle()));
    const int scroll(mouse->getScroll());
    if (scroll != 0)
    {
        const vec2 mousePos(mouse->getPosition());
        const vec2 mouseWorldPos(screenToWorldPos(mouse->getPosition()));
        m_Scale += mouse->getScroll() * ZOOM_STEP;
        m_Scale = he::clamp(m_Scale, ZOOM_MIN, ZOOM_MAX);
        const vec2 wrongMouseWorldPos(screenToWorldPos(mousePos));
        const vec2 offset(wrongMouseWorldPos - mouseWorldPos);
        m_Offset -= offset;
    }
}

void NodeGraph::createNode(const he::vec2& pos)
{
    const size_t inputs(rand() % 5 + 1);
    const size_t outputs(rand() % 5 + 1);

    NodeGraphNode* node(NEW NodeGraphNode());
    node->setPosition(pos);
    for (size_t i(0); i < inputs; ++i)
    {
        node->addInput(NEW NodeGraphNodeInput());
    }
    for (size_t i(0); i < outputs; ++i)
    {
        node->addOutput(NEW NodeGraphNodeOutput());
    }
    m_Nodes.add(node);
}

void NodeGraph::updateStates( const float dTime )
{
    switch (m_State)
    {
    case State_Idle:
        {
            updateIdleState(dTime);
        } break;
    case State_StartPan:
        {
            updateStartPanState(dTime);
        } break;
    case State_Pan:
        {
            updatePanState(dTime);
        } break;
    }
}

void NodeGraph::updateIdleState( const float /*dTime*/ )
{
    const he::io::ControlsManager* const controls(CONTROLS);
    const he::io::IMouse* const mouse(controls->getMouse(getHandle()));
    const he::io::IKeyboard* const keyboard(controls->getKeyboard(getHandle()));
    const he::vec2 mouseWorld(screenToWorldPos(mouse->getPosition()));

    m_GrabWorldPos = mouseWorld;
    const bool leftDown(mouse->isButtonPressed(he::io::MouseButton_Left));
    if (leftDown)
    {
        if (keyboard->isKeyDown(he::io::Key_Ctrl))
        {
            createNode(screenToWorldPos(mouse->getPosition()));
        }
        else
        {
            m_State = State_StartPan;
        }
    }
}

void NodeGraph::updateStartPanState( const float /*dTime*/ )
{
    const he::io::ControlsManager* const controls(CONTROLS);
    const he::io::IMouse* const mouse(controls->getMouse(getHandle()));
    const he::vec2 mousePos(mouse->getPosition());
    const he::vec2 grabScreenPos(worldToScreenPos(m_GrabWorldPos));
    const he::vec2 diff(mousePos - grabScreenPos);
    if (mouse->isButtonReleased(he::io::MouseButton_Left))
    {
        m_State = State_Idle;
    }
    else if (fabs(diff.x) > 4 || fabs(diff.y) > 4)
    {
        m_State = State_Pan;
    }
}

void NodeGraph::updatePanState( const float /*dTime*/ )
{
    const he::io::ControlsManager* const controls(CONTROLS);
    const he::io::IMouse* const mouse(controls->getMouse(getHandle()));
    const he::vec2 mousePos(mouse->getPosition());
    const he::vec2 worldMouse(screenToWorldPos(mousePos));
    const he::vec2 diff(worldMouse - m_GrabWorldPos);
    m_Offset -= diff; 
    if (mouse->isButtonReleased(he::io::MouseButton_Left))
    {
        m_State = State_Idle;
    }
}

void NodeGraph::updateStartMoveNodeState( const float /*dTime*/ )
{

}

void NodeGraph::updateMoveNodeState( const float /*dTime*/ )
{

}

void NodeGraph::updateConnectNodeState( const float /*dTime*/ )
{

}

void NodeGraph::draw2D( he::gui::Canvas2D* canvas )
{
    NodeGraphDrawContext context;
    const he::vec2 canvasSize(canvas->getSize());
    context.canvas = canvas;
    context.transform = he::mat33::createScale2D(he::vec2(m_Scale, m_Scale)) * he::mat33::createTranslation2D(-m_Offset);
    context.clipRect = he::RectF(0.0f, 0.0f, canvasSize.x, canvasSize.y);
    context.worldRect = he::RectF(m_Offset.x, m_Offset.y, canvasSize.x / m_Scale, canvasSize.y / m_Scale);

    drawBackground(context);
    drawNodes(context);
    drawDebug(context);
}

void NodeGraph::drawBackground(const NodeGraphDrawContext& context)
{
    // Background
    const he::RectI clipRectI(0, 0, static_cast<int>(context.clipRect.width), static_cast<int>(context.clipRect.height));
    context.canvas->setColor(he::Color(static_cast<he::uint8>(50), 41, 50));
    context.canvas->fillRect(clipRectI);

    // Grid
    he::Color lineColor(static_cast<he::uint8>(84), 80, 114);
    he::Color majorLineColor(static_cast<he::uint8>(89), 130, 147);
    context.canvas->setColor(lineColor);
    
    const static float worldUnitsPerGridLine = 32;
    const static int majorGridLineEveryNth = 8;

    const float screenUnitsPerGridLine(worldUnitsPerGridLine * m_Scale);

    const he::vec2 screenOffset(worldToScreenPos(he::vec2(
        ceil(m_Offset.x / worldUnitsPerGridLine) * worldUnitsPerGridLine,
        ceil(m_Offset.y / worldUnitsPerGridLine) * worldUnitsPerGridLine))); 

    int xMajorOffset(static_cast<int>(ceil(m_Offset.x / worldUnitsPerGridLine)) % majorGridLineEveryNth);
    int yMajorOffset(static_cast<int>(ceil(m_Offset.y / worldUnitsPerGridLine)) % majorGridLineEveryNth);

    for (float x(screenOffset.x); x < context.clipRect.width; x += screenUnitsPerGridLine, ++xMajorOffset)
    {
        const bool isMajor((xMajorOffset % majorGridLineEveryNth) == 0);
        if (isMajor)
            context.canvas->setColor(majorLineColor);
        context.canvas->drawLine(he::vec2(x, 0), he::vec2(x, context.clipRect.height));
        if (isMajor)
            context.canvas->setColor(lineColor);
    }
    for (float y(screenOffset.y); y < context.clipRect.height; y += screenUnitsPerGridLine, ++yMajorOffset)
    {
        const bool isMajor((yMajorOffset % majorGridLineEveryNth) == 0);
        if (isMajor)
            context.canvas->setColor(majorLineColor);
        context.canvas->drawLine(he::vec2(0, y), he::vec2(context.clipRect.width, y));
        if (isMajor)
            context.canvas->setColor(lineColor);
    }
}

void NodeGraph::drawNodes(const NodeGraphDrawContext& context)
{
    m_Nodes.forEach([&context](NodeGraphNode* node)
    {
        node->draw(context);
    });
}

void NodeGraph::drawDebug(const NodeGraphDrawContext& context)
{
    m_DebugText.clear();
    const he::vec2 mouseWorld(screenToWorldPos(CONTROLS->getMouse(getHandle())->getPosition()));
    m_DebugText.addTextExt("&5F5Zoom: &AFA%.2f\n&5F5Region: &AFA%.2f, %.2f, %.2f, %.2f\n&5F5Mouse: &AFA%.2f, %.2f\n\n&5F5: Undo/Redo: &5F5\n", 
        m_Scale, context.worldRect.x, context.worldRect.y, context.worldRect.width, context.worldRect.height, mouseWorld.x, mouseWorld.y);

    context.canvas->setColor(he::Color(0.0f, 0.0f, 0.0f, 1.0f));
    context.canvas->fillText(m_DebugText, he::vec2(14.0f, 14.0f));
    context.canvas->setColor(he::Color(1.0f, 1.0f, 1.0f, 1.0f));
    context.canvas->fillText(m_DebugText, he::vec2(12.0f, 12.0f));
}

}
