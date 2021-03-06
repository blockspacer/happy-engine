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
//Author:  Sebastiaan Sprengers
//Created: 08/10/2011

#ifndef _HE_CONSOLE_H_
#define _HE_CONSOLE_H_
#pragma once

#include <typeinfo>
#include "IDrawable2D.h"

namespace he {

enum CMSG_TYPE
{
    CMSG_TYPE_INFO = 0,
    CMSG_TYPE_WARNING = 1,
    CMSG_TYPE_ERROR = 2,
    CMSG_TYPE_ENGINE = 3,
    CMSG_TYPE_COMMAND = 4
};

namespace gui {
    class TextBox;
    class Text;
    class Scrollbar;
    class Sprite;
}

namespace gfx {
    class Font;
    class Renderer2D;
}

namespace tools {
class ITypeHandler;

class HAPPY_ENTRY Console : public gfx::IDrawable2D
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Console();
    virtual ~Console();

    /* GENERAL */
    void load();
    void tick();

    void attachToRenderer(gfx::Renderer2D* renderer);
    void detachFromRenderer();

    virtual void draw2D(gui::Canvas2D* canvas); // auto called

    void addMessage(const char* msg, CMSG_TYPE type = CMSG_TYPE_INFO);

    template <typename T>
    void registerVar(T* /*pVar*/, const he::String& /*varKey*/)
    {
        HE_IF_ASSERT(m_TypeHandlers.contains(typeid(T).name()), "Type handler for '%s'not specified!", typeid(T).name())
        {
            /*HE_IF_ASSERT(m_ValueContainer.find(varKey) == m_ValueContainer.end(), "Variable: '%s' already registered!", varKey.c_str())
            {
                m_ValueContainer[varKey] = pVar;
            }*/
        }
    }

    void registerCmd(const std::function<void()>& command, const he::String& cmdKey);

    void addTypeHandler(ITypeHandler* typeHandler);
    void flushMessageHistory();

    /* SETTERS */
    void setKeyboardShortcut(io::Key key);
    void setMessageColors(	const Color& infoColor = Color(1.0f,1.0f,1.0f),
                            const Color& warningColor = Color(1.0f,1.0f,0.0f),
                            const Color& errorColor = Color(1.0f,0.0f,0.0f),
                            const Color& engineColor = Color(0.0f,1.0f,0.0f),
                            const Color& commandColor = Color(0.0f,0.5f,1.0f));
    // * Sets the max messages to keep in console history, 0 == no limit. *
    void setMaxMsgHistory(uint32 maxMessages = 0);
    void toggleShowMessages(CMSG_TYPE type, bool show);

private:

    void processCommand(const he::String& command);
    void displayHelp();
    void displayVars();
    void displayCmds();
    void onResize();
    void renderBackground();

    /* DATAMEMBERS */
    //he::Map<he::String, boost::any> m_ValueContainer;
    he::Map<he::String, std::function<void()> > m_FunctionContainer;
    he::Map<CMSG_TYPE, Color> m_MsgColors;
    he::Map<he::String, ITypeHandler*> m_TypeHandlers;
    he::ObjectList<std::pair<CMSG_TYPE, he::String> > m_MsgHistory;
    he::ObjectList<he::String> m_CmdHistory;
    he::Map<CMSG_TYPE, bool> m_ShowMessageTypes;

    io::Key m_Shortcut;

    uint32 m_MaxMessages;
    uint32 m_CmdHistoryPos;
    uint32 m_MaxMessagesInWindow;

    bool m_IsOpen;

    gui::TextBox* m_TextBox;
    he::String m_Help;
    gui::Scrollbar* m_ScrollBar;

    he::String m_HelpCommand;

    gui::Font* m_Font;
    gui::Text m_Text;
    gfx::Renderer2D* m_Renderer;
    gui::Sprite* m_Background;

    vec2 m_ViewportSize;
    eventCallback0<void> m_ResizeHandler;

    /* DEFAULT COPY & ASSIGNMENT */
    Console(const Console&);
    Console& operator=(const Console&);
};

} } //end namespace

#endif
