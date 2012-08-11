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
//Author:  Sebastiaan Sprengers
//Created: 19/03/2012

#ifndef _HE_RENDERER2D_H_
#define _HE_RENDERER2D_H_
#pragma once

#include "IRenderer.h"

namespace he {
namespace gfx {
class WebView;
class Canvas2D;
class Simple2DTextureEffect;
class ModelMesh;
class I2DDrawable;

class Renderer2D : public IRenderer
{
public:

    /* CONSTRUCTOR - DESTRUCTOR */
    Renderer2D();
    virtual ~Renderer2D();

    /* GENERAL */
    Canvas2D* createCanvasAbsolute(const RectI& viewport);
    Canvas2D* createCanvasRelative(const RectF& viewportPercent);
    void removeCanvas(Canvas2D* canvas);

    WebView* createWebViewAbsolute(const RectI& viewport, bool enableUserInput = false);
    WebView* createWebViewRelative(const RectF& viewportPercent, bool enableUserInput = false);
    void removeWebView(WebView* webview);
 
    void drawTexture2DToScreen( const Texture2D* tex2D, const vec2& pos = vec2(),
                                bool useBlending = true,
                                const vec2& newDimensions = vec2(),
                                const RectF& regionToDraw = RectF());

    View* getView() const { return m_View; }

    /* Attach */
    void attachToRender(I2DDrawable* drawable);
    void detachFromRender(I2DDrawable* drawable);

    /* IRenderer */
    virtual void init(View* view, const RenderTarget* target);
    virtual void draw();

private:

    /* DATAMEMBERS */
    View* m_View;
    const RenderTarget* m_RenderTarget;

    std::vector<WebView*> m_WebViews;
    std::vector<Canvas2D*> m_Canvas2Ds;
    std::vector<I2DDrawable*> m_Drawables;

    Simple2DTextureEffect* m_TextureEffect;
    ModelMesh* m_TextureQuad;

    /* DEFAULT COPY & ASSIGNMENT */
    Renderer2D(const Renderer2D&);
    Renderer2D& operator=(const Renderer2D&);
};

} } //end namespace

#endif
