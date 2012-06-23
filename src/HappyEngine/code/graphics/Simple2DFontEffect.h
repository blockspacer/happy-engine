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
//Created: 07/05/2012

#ifndef _HE_SIMPLE_2D_FONT_EFFECT_H_
#define _HE_SIMPLE_2D_FONT_EFFECT_H_
#pragma once

namespace he {
namespace gfx {
class Texture2D;
class Shader;

class Simple2DFontEffect
{
public:
    Simple2DFontEffect();
    virtual ~Simple2DFontEffect();

    void load();
    void begin() const;
    void end() const;

    void setWorldMatrix(const he::mat44& mat) const;
    void setDiffuseMap(const he::gfx::Texture2D* diffuseMap) const;
    void setDepth(float depth) const;
    void setFontColor(const Color& col) const;
    void setTCOffset(const vec2& offset) const;
    void setTCScale(const vec2& scale) const;

private:
    gfx::Shader* m_Shader;

    uint m_ShaderWVPPos;
    uint m_ShaderDiffTexPos;
    uint m_ShaderDepthPos;
    uint m_ShaderFontColorPos;
    uint m_ShaderTCOffsetPos;
    uint m_ShaderTCScalePos;

    //Disable default copy constructor and default assignment operator
    Simple2DFontEffect(const Simple2DFontEffect&);
    Simple2DFontEffect& operator=(const Simple2DFontEffect&);
};

} } //end namespace

#endif