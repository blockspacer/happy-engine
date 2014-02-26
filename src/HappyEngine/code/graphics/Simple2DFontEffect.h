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
struct DrawContext;
class BufferLayout;
class MaterialInstance;
class MaterialParameter;

class Simple2DFontEffect
{
public:
    Simple2DFontEffect();
    ~Simple2DFontEffect();

    void init(const BufferLayout& layout);
    void begin(const he::gfx::DrawContext& context) const;
    void end() const;

    void setWorldMatrix(const he::mat44& mat) const;
    void setDiffuseMap(const he::gfx::Texture2D* diffuseMap) const;
    void setBlendColor(const Color& col) const;

private:
    MaterialInstance* m_Material;

    int8 m_WVP;
    int8 m_DiffTex;
    int8 m_BlendColor;

    //Disable default copy constructor and default assignment operator
    Simple2DFontEffect(const Simple2DFontEffect&);
    Simple2DFontEffect& operator=(const Simple2DFontEffect&);
};

} } //end namespace

#endif
