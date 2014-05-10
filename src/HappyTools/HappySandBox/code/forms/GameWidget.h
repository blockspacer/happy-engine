//HappyEngine Copyright (C) 2011 - 2012  Bastian Damman, Sebastiaan Sprengers 
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

#ifndef _HS_GAME_WIDGET_H_
#define _HS_GAME_WIDGET_H_
#pragma once

#include <QGLWidget>
#include "Game.h"

namespace he {
namespace io {
    class QtMouse;
    class QtKeyboard;
}}

namespace hs {

class GameWidget :  public QGLWidget, public he::ge::Game
{
    Q_OBJECT
public:
    explicit GameWidget(QWidget *parent = 0);
    virtual ~GameWidget();

    void present();
    
private:
    // From he::ge::Game
    void init();
    void destroy();

    // From QGLWidget
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);
    void closeEvent(QCloseEvent* event);

    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);

    virtual void keyPressEvent(QKeyEvent* event);
    virtual void keyReleaseEvent(QKeyEvent* event);

    //Disable default copy constructor and default assignment operator
    GameWidget(const GameWidget&);
    GameWidget& operator=(const GameWidget&);
};

} //end namespace

#endif
