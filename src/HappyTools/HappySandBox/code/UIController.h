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
//Author:  Sebastiaan Sprengers
//Created: 10/07/2012

#ifndef _HS_UI_CONTROLLER_H_
#define _HS_UI_CONTROLLER_H_
#pragma once

#include "IDrawable2D.h"
#include "Text.h"

namespace he {
    namespace gfx {
        class View;
    }
    namespace gui {
        class WebView;
        class Canvas2D;
    }
}

namespace hs {

class UIController : public he::gfx::IDrawable2D
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    UIController();
    virtual ~UIController();

    /* GENERAL */
    void init(he::gfx::Renderer2D* renderer);
    void load(const std::string& file);
    void draw2D(he::gui::Canvas2D* canvas);
    void updateSceneInfo(const he::vec3& camPos);

    /* GETTERS */
    he::gui::WebView* getWebView() const;

private:

    /* DATAMEMBERS */
    std::string m_GUIDirectory;

    he::gui::WebView* m_WebView;
    he::gfx::Renderer2D* m_Renderer;

    he::vec3 m_CamPos;

    he::gui::Text m_SceneInfo;

    /* DEFAULT COPY & ASSIGNMENT */
    UIController(const UIController&);
    UIController& operator=(const UIController&);


};

} //end namespace

#endif
