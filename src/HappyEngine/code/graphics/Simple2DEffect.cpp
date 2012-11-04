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
//Created: 26/08/2011
#include "HappyPCH.h" 

#include "Simple2DEffect.h"
#include "HappyNew.h"
#include "HeAssert.h"

#include "ContentManager.h"

namespace he {
namespace gfx {

Simple2DEffect::Simple2DEffect() : m_Shader(nullptr)
{ 
}


Simple2DEffect::~Simple2DEffect()
{
    m_Shader->release();
}

void Simple2DEffect::load()
{

    ShaderLayout layout;
    layout.addElement(ShaderLayoutElement(0, "inPosition"));

    m_Shader = ResourceFactory<Shader>::getInstance()->get(ResourceFactory<Shader>::getInstance()->create());
    he::ObjectList<std::string> shaderOutputs;
    shaderOutputs.add("outColor");
    const std::string& folder(CONTENT->getShaderFolderPath().str());
    bool shaderInited(m_Shader->initFromFile(folder + "2D/simple2DShader.vert", 
                                              folder + "2D/simple2DShader.frag", layout, shaderOutputs));
    HE_ASSERT(shaderInited == true, "simple2DShader init failed");

    m_ShaderWVPPos = m_Shader->getShaderVarId("matWVP");
    m_ShaderColorPos = m_Shader->getShaderVarId("color");
    m_ShaderDepthPos = m_Shader->getShaderVarId("depth");
    m_ShaderBlendPos = m_Shader->getShaderVarId("blending");

    m_Shader->bind();
    mat44 MatWVP = mat44::createTranslation(vec3(0.0f,0.0f,0.0f));
    m_Shader->setShaderVar(m_ShaderWVPPos, MatWVP);
    m_Shader->setShaderVar(m_ShaderColorPos, vec4(1.0f,1.0f,1.0f,1.0f));
}

void Simple2DEffect::begin() const
{
    m_Shader->bind();
}

void Simple2DEffect::end() const
{
}

void Simple2DEffect::setColor(const Color& color) const
{
    m_Shader->setShaderVar(m_ShaderColorPos, vec4(color.rgba()));
}

void Simple2DEffect::setWorldMatrix(const he::mat44 &mat) const
{
    m_Shader->setShaderVar(m_ShaderWVPPos, mat);
}

void Simple2DEffect::setDepth(float depth) const
{
    m_Shader->setShaderVar(m_ShaderDepthPos, depth);
}

void Simple2DEffect::setBlending(bool blend) const
{
    m_Shader->setShaderVar(m_ShaderBlendPos, blend);
}

} } //end namespace