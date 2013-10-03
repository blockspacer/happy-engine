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
// Author: Bastian Damman

#ifndef _HE_SHADER_H_
#define _HE_SHADER_H_
#pragma once

#include "ShaderLayout.h"
#include "Resource.h"

namespace he {
namespace gfx {

class Shader;
class Texture2D;
class TextureCube;
typedef ResourceFactory<Shader> ShaderFactory;

class IShaderUniform;

struct ShaderUniformID
{
    const static size_t Unassigned;
    size_t m_ID;

    ShaderUniformID(): m_ID(Unassigned) {}
};

class Shader : public Resource<Shader>
{
public:
    Shader();
    virtual ~Shader();

    // Init
    bool initFromFile(const he::String& vsPath, const he::String& fsPath, const ShaderLayout& shaderLayout);
    bool initFromFile(const he::String& vsPath, const he::String& fsPath, const ShaderLayout& shaderLayout, const he::ObjectList<he::String>& outputs);
    bool initFromFile(const he::String& vsPath, const he::String& fsPath, const ShaderLayout& shaderLayout, const std::set<he::String>& defines, const he::ObjectList<he::String>& outputs = he::ObjectList<he::String>());
    
    bool initFromMem(const he::String& vs, const he::String& fs, const ShaderLayout& shaderLayout, const he::String& debugVertName, const he::String& debugFragName);
    bool initFromMem(const he::String& vs, const he::String& fs, const ShaderLayout& shaderLayout, const he::String& debugVertName, const he::String& debugFragName, const he::ObjectList<he::String>& outputs);
    bool initFromMem(const he::String& vs, const he::String& fs, const ShaderLayout& shaderLayout, const he::String& debugVertName, const he::String& debugFragName, const std::set<he::String>& defines, const he::ObjectList<he::String>& outputs = he::ObjectList<he::String>());

    // Use
    void bind();

    // Getters
    ShaderUniformID getUniformID(const he::FixedString& name) const;
    IShaderUniform* getUniform(const ShaderUniformID id) const;
    const ShaderLayout& getShaderLayout() const { return m_Layout; }
    
    // Setters
    void setShaderVar(uint32 id, const int value) const;
    void setShaderVar(uint32 id, const uint32 value) const;
    void setShaderVar(uint32 id, const float value) const;
    void setShaderVar(uint32 id, const vec2& vec) const;
    void setShaderVar(uint32 id, const vec3& vec) const;
    void setShaderVar(uint32 id, const vec4& vec) const;
    void setShaderVar(uint32 id, const mat44& matrix) const;
    void setShaderVar(uint32 id, const he::PrimitiveList<mat44>& matrixArray) const;
    void setShaderVar(uint32 id, const gfx::Texture2D* tex2D) const;
    void setShaderVar(uint32 id, const gfx::TextureCube* texCube) const;
    
private:
    uint32 m_Id;
    he::PrimitiveList<IShaderUniform*> m_Uniforms;

    uint32 m_VsId;
    uint32 m_FsId;
    ShaderLayout m_Layout;

    he::String m_FragShaderName;
    he::String m_VertShaderName;

    static uint32 s_CurrentBoundShader;

    //Disable default copy constructor and default assignment operator
    Shader(const Shader&);
    Shader& operator=(const Shader&);
};

} } //end namespace

#endif
