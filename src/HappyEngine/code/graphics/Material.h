//HappyEngine Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
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
//Created: 30/09/2011

#ifndef _HE_MATERIAL_H_
#define _HE_MATERIAL_H_
#pragma once

#include "Shader.h"
#include "HappyTypes.h"
#include "boost/shared_ptr.hpp"
#include "ShaderVar.h"
#include "BufferLayout.h"

namespace he {
namespace gfx {

class IInstancedDrawable;
class ISingleDrawable;
class ISkinnedDrawable;
class ICamera;

class Material
{
public:
    Material();
    virtual ~Material();
    // default copy constructor and default assignment operator are OK

    void addVar(const ShaderVar::pointer& var);
    const ShaderVar::pointer& getVar(const std::string& var);
    void setShader(const Shader::pointer& pShader, const BufferLayout& compatibleVertexLayout, const BufferLayout& compatibleInstancingLayout);

    const BufferLayout& getCompatibleVertexLayout() const;
    const BufferLayout& getCompatibleInstancingLayout() const;

    void apply(const IInstancedDrawable* pObj,  const ICamera* pCamera) const;
    void apply(const ISingleDrawable*    pObj,  const ICamera* pCamera) const;
    void apply(const ISkinnedDrawable*   pObj,  const ICamera* pCamera) const;

    void setIsBlended(bool isBlended, BlendEquation equation = BlendEquation_Add, 
                                      BlendFunc sourceBlend  = BlendFunc_One,
                                      BlendFunc destBlend    = BlendFunc_Zero);
    void setNoPost(bool noPost);

    bool isBlended() const;
    BlendEquation getBlendEquation() const;
    BlendFunc getSourceBlend() const;
    BlendFunc getDestBlend() const;
    bool noPost() const;
    bool isUsedForInstancing() const;

private:

    BlendEquation m_BlendEquation;
    BlendFunc m_SourceBlend, m_DestBlend;
    bool m_IsBlended, m_UsedForInstancing, m_NoPost;

    Shader::pointer m_pShader;
    std::vector<ShaderVar::pointer> m_ShaderVar;

    BufferLayout m_CompatibleVL;
    BufferLayout m_CompatibleIL;
};

} } //end namespace

#endif
