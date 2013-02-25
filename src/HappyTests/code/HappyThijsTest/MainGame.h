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
//Author:  Thijs Morlion
//Created: 27/01/2013

#ifndef _HT_MainGame_H_
#define _HT_MainGame_H_
#pragma once

#include "Game.h"
#include "IDrawable2D.h"

namespace he {
    namespace tools {
        class FPSGraph;
    }
    namespace gfx {
        class Font;
        class View;
        class Window;
        class Renderer2D;
    }
}

namespace ht {

class MainGame : public he::ge::Game, public he::gfx::IDrawable2D
{
public:
    MainGame();
    virtual ~MainGame();

    virtual void init();
    virtual void destroy() {}
    virtual void load();
    virtual void tick(float dTime);
    virtual void draw2D(he::gfx::Canvas2D* canvas);

private:

    /* DATAMEMBERS */
    he::tools::FPSGraph* m_FpsGraph;
    he::gfx::Window* m_Window;
    he::gfx::View*   m_View;
    he::gfx::Renderer2D* m_Renderer;
    he::gui::Font* m_Font;

    /* DEFAULT COPY & ASSIGNMENT */
    MainGame(const MainGame&);
    MainGame& operator=(const MainGame&);
};

} //end namespace

#endif