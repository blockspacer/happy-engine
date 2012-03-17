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
//Author:  Bastian Damman
//Created: 30/09/2011
#include "HappyPCH.h" 

#include "MaterialLoader.h"
#include "HappyNew.h"
#include "IniReader.h"
#include "HappyEngine.h"
#include "BufferLayout.h"
#include "Texture2D.h"
#include "ContentManager.h"
#include "Bone.h"

namespace he {
namespace ct {

MaterialLoader::MaterialLoader(): m_pAssetContainer(NEW AssetContainer<gfx::Material>())
{
}


MaterialLoader::~MaterialLoader()
{
    delete m_pAssetContainer;
}

BlendEquation blendEquationFromString(const std::string& str)
{
    if (str == "ADD")
    {
        return BlendEquation_Add;
    }
    else if (str == "SUBTRACT")
    {
        return BlendEquation_Subtract;
    }
    else if (str == "REVERSE_SUBTRACT" || str == "INVERSE_SUBTRACT")
    {
        return BlendEquation_ReverseSubtract;
    }
    else if (str == "MIN")
    {
        return BlendEquation_Min;
    }
    else if (str == "MAX")
    {
        return BlendEquation_Max;
    }
    else 
    {
        HE_ERROR("unknown blendEquationFromString: " + str);
        return BlendEquation_Add;
    }
}
BlendFunc blendFuncFromString(const std::string& str)
{
    if (str == "ZERO")
    {
        return BlendFunc_Zero;
    }
    else if (str == "ONE")
    {
        return BlendFunc_One;
    }
    else if (str == "SRC_COLOR")
    {
        return BlendFunc_SrcColor;
    }
    else if (str == "INV_SRC_COLOR" || str == "ONE_MIN_SRC_COLOR")
    {
        return BlendFunc_OneMinusSrcColor;
    }
    else if (str == "DEST_COLOR")
    {
        return BlendFunc_DestColor;
    }
    else if (str == "INV_DEST_COLOR" || str == "ONE_MIN_DEST_COLOR")
    {
        return BlendFunc_OneMinusDestColor;
    }
    else if (str == "SRC_ALPHA")
    {
        return BlendFunc_SrcAlpha;
    }
    else if (str == "INV_SRC_ALPHA" || str == "ONE_MIN_SRC_ALPHA" )
    {
        return BlendFunc_OneMinusSrcAlpha;
    }
    else if (str == "DEST_ALPHA")
    {
        return BlendFunc_DestAlpha;
    }
    else if (str == "INV_DEST_ALPHA" || str == "ONE_MIN_DEST_ALPHA" )
    {
        return BlendFunc_OneMinusDestAlpha;
    }
    else if (str == "SRC_ALPHA_SAT")
    {
        return BlendFunc_SrcAlphaSaturate;
    }
    else 
    {
        HE_ERROR("unknown blendFuncFromString: " + str);
        return BlendFunc_One;
    }
}

gfx::Material MaterialLoader::load(const std::string& path)
{
    if (m_pAssetContainer->isAssetPresent(path))
    {
        return m_pAssetContainer->getAsset(path);
    }
    else
    {
        io::IniReader reader;
        
        gfx::Material material;
        if (reader.open(path) == false)
        {
            HE_ERROR("Error loading material: " + path);
            return material;

        }
        
        if (reader.isOpen())
        {     
            gfx::BufferLayout vertexLayout;
            // [Shader]
            {
                std::string file;
                file = reader.readString(L"Forward", L"shader", "");
                if (m_RenderSettings.enableDeferred)
                {
                    std::string temp = reader.readString(L"Deferred", L"shader", file);
                    if (temp != file)
                    {
                        file = temp;
                    }
                }

                io::IniReader shaderReader;
                if (shaderReader.open(CONTENT->getRootDir() + CONTENT->getShaderFolder() + file) == false)
                { 
                    HE_ERROR("Error loading material shader: " + path);
                    return material;
                }

                std::vector<std::string> shaderOutputs;

                // [out]
                if (shaderReader.containsRoot(L"out"))
                {
                    const std::map<std::wstring, std::wstring>& outNodes(shaderReader.getNodes(L"out"));
                    if (outNodes.size() == 3)
                    {
                        shaderOutputs.resize(3);
                        std::for_each(outNodes.cbegin(), outNodes.cend(), [&](const std::pair<std::wstring, std::wstring>& p)
                        {
                            if (p.second == L"GBUFFER_COLOR")
                                shaderOutputs[0] = std::string(p.first.cbegin(), p.first.cend());
                            else if (p.second == L"GBUFFER_SGI")
                                shaderOutputs[1] = std::string(p.first.cbegin(), p.first.cend());
                            else if (p.second == L"GBUFFER_NORMAL")
                                shaderOutputs[2] = std::string(p.first.cbegin(), p.first.cend());
                            else
                                HE_ASSERT(false, "unknow semantic");
                        });
                    }
                    else if (outNodes.size() == 2)
                    {
                        shaderOutputs.resize(2);
                        std::for_each(outNodes.cbegin(), outNodes.cend(), [&](const std::pair<std::wstring, std::wstring>& p)
                        {
                            if (p.second == L"GBUFFER_COLOR")
                                shaderOutputs[0] = std::string(p.first.cbegin(), p.first.cend());
                            else if (p.second == L"GBUFFER_NORMAL")
                                shaderOutputs[1] = std::string(p.first.cbegin(), p.first.cend());
                            else
                                HE_ASSERT(false, "unknow semantic");
                        });
                    }
                }

                // [inPerVertex]
                gfx::ShaderLayout shaderLayout;
                uint count(0);
                uint offset(0);
                const std::map<std::wstring, std::wstring>& inNodes(shaderReader.getNodes(L"inPerVertex"));
                std::for_each(inNodes.cbegin(), inNodes.cend(), [&](const std::pair<std::wstring, std::wstring>& p)
                {
                    if (p.second == L"POSITION")
                    {
                        vertexLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec3, gfx::BufferElement::Usage_Position, sizeof(vec3), offset));
                        offset += sizeof(vec3);
                    }
                    else if (p.second == L"TEXCOORD")
                    {
                        vertexLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec2, gfx::BufferElement::Usage_TextureCoordinate, sizeof(vec2), offset));
                        offset += sizeof(vec2);
                    }
                    else if (p.second == L"NORMAL")
                    {
                        vertexLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec3, gfx::BufferElement::Usage_Normal, sizeof(vec3), offset));
                        offset += sizeof(vec3);
                    }
                    else if (p.second == L"TANGENT")
                    {
                        vertexLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec3, gfx::BufferElement::Usage_Tangent, sizeof(vec3), offset));
                        offset += sizeof(vec3);
                    }
                    else if (p.second == L"BONEIDS")
                    {
                        vertexLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec4, gfx::BufferElement::Usage_BoneIDs, sizeof(vec4), offset));
                        offset += sizeof(vec4);
                    }
                    else if (p.second == L"BONEWEIGHTS")
                    {
                        HE_ASSERT(gfx::Bone::MAX_BONEWEIGHTS == 4, "layout incompatible");
                        vertexLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec4, gfx::BufferElement::Usage_BoneWeights, sizeof(vec4), offset));
                        offset += sizeof(vec4);
                    }
                    else
                    {
                        HE_ERROR("Material: unkown attribute " + std::string(p.second.cbegin(), p.second.cend()));
                    } 
                    shaderLayout.addElement(gfx::ShaderLayoutElement(shaderLayout.getElements().size(), std::string(p.first.cbegin(), p.first.cend())));
                }); 

                gfx::BufferLayout instancingLayout;
                offset = 0;
                count = 0;
                if (shaderReader.containsRoot(L"inPerInstance"))
                {
                    const std::map<std::wstring, std::wstring>& inNodes(shaderReader.getNodes(L"inPerInstance"));
                    std::for_each(inNodes.cbegin(), inNodes.cend(), [&](const std::pair<std::wstring, std::wstring>& p)
                    {
                        if (p.second == L"MAT44")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec4, gfx::BufferElement::Usage_Instancing, sizeof(vec4), offset));
                            offset += sizeof(vec4);
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec4, gfx::BufferElement::Usage_Instancing, sizeof(vec4), offset));
                            offset += sizeof(vec4);
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec4, gfx::BufferElement::Usage_Instancing, sizeof(vec4), offset));
                            offset += sizeof(vec4);
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec4, gfx::BufferElement::Usage_Instancing, sizeof(vec4), offset));
                            offset += sizeof(vec4);
                        }
                        else if (p.second == L"FLOAT")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Float, gfx::BufferElement::Usage_Instancing, sizeof(float), offset));
                            offset += sizeof(float);
                        }
                        else if (p.second == L"VEC2")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec2, gfx::BufferElement::Usage_Instancing, sizeof(vec2), offset));
                            offset += sizeof(vec2);
                        }
                        else if (p.second == L"VEC3")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec3, gfx::BufferElement::Usage_Instancing, sizeof(vec3), offset));
                            offset += sizeof(vec3);
                        }
                        else if (p.second == L"VEC4")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Vec4, gfx::BufferElement::Usage_Instancing, sizeof(vec4), offset));
                            offset += sizeof(vec4);
                        }
                        else if (p.second == L"INT")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_Int, gfx::BufferElement::Usage_Instancing, sizeof(int), offset));
                            offset += sizeof(int);
                        }
                        else if (p.second == L"UINT")
                        {
                            instancingLayout.addElement(gfx::BufferElement(count++, gfx::BufferElement::Type_UInt, gfx::BufferElement::Usage_Instancing, sizeof(uint), offset));
                            offset += sizeof(uint);
                        }
                        else
                        {
                            HE_ERROR("Material: instancing unkown type " + std::string(p.second.cbegin(), p.second.cend()));
                        }
                        shaderLayout.addElement(gfx::ShaderLayoutElement(shaderLayout.getElements().size(), std::string(p.first.cbegin(), p.first.cend())));  
                    });
                }

                // [Shader]
                gfx::Shader::pointer pShader(CONTENT->loadShader(shaderReader.readString(L"Shader", L"vsPath", ""),
                                                                 shaderReader.readString(L"Shader", L"fsPath", ""),
                                                                 shaderLayout,
                                                                 shaderOutputs));

                // [info]
                bool isBlended(false);
                BlendEquation blendEq(BlendEquation_Add);
                BlendFunc srcBlend(BlendFunc_One), destBlend(BlendFunc_Zero);
                bool post(true);
                if (reader.containsRoot(L"info"))
                {
                    isBlended = reader.readBool(L"info", L"blending", false);
                    if (isBlended)
                    {
                        blendEq = blendEquationFromString(reader.readString(L"info", L"blendFunc", "ADD"));
                        srcBlend = blendFuncFromString(reader.readString(L"info", L"srcBlend", "ONE"));
                        destBlend = blendFuncFromString(reader.readString(L"info", L"destBlend", "ZERO"));
                    }
                    post = reader.readBool(L"info", L"post", true);
                }

                material.setIsBlended(isBlended, blendEq, srcBlend, destBlend);
                material.setNoPost(!post);
                material.setShader(pShader, vertexLayout, instancingLayout);


                // [uniform]
                if (shaderReader.containsRoot(L"uniform"))
                {
                    const std::map<std::wstring, std::wstring>& uniformNodes(shaderReader.getNodes(L"uniform"));
                    std::for_each(uniformNodes.cbegin(), uniformNodes.cend(), [&](const std::pair<std::wstring, std::wstring> node)
                    {
                        std::string name = std::string(node.first.cbegin(), node.first.cend());
                        // Camera
                        if (node.second == L"WORLDVIEWPROJECTION")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_WorldViewProjection)));
                        }
                        else if (node.second == L"WORLDVIEW")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_WorldView)));
                        }
                        else if (node.second == L"VIEWPROJECTION")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_ViewProjection)));
                        }
                        else if (node.second == L"VIEW")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_View)));
                        }
                        else if (node.second == L"WORLD")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_World)));
                        }
                        else if (node.second == L"WORLDPOSITION")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_WorldPosition)));
                        }

                        // Light
                        else if (node.second == L"AMBIENT_COLOR")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_AmbientColor)));
                        }
                        else if (node.second == L"DIRECTIONAL_COLOR")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_DirectionalColor)));
                        }
                        else if (node.second == L"DIRECTIONAL_DIRECTION")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_DirectionalDirection)));
                        }

                        // Shadow
                        else if (node.second == L"SHADOW_CASCADE_MATRIX0")
                        {
                            if (m_RenderSettings.enableShadows)
                                material.addVar(gfx::ShaderVar::pointer(
                                    NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_ShadowCascadeMatrix0)));
                        }
                        else if (node.second == L"SHADOW_CASCADE_MATRIX1")
                        {
                            if (m_RenderSettings.enableShadows)
                                material.addVar(gfx::ShaderVar::pointer(
                                    NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_ShadowCascadeMatrix1)));
                        }
                        else if (node.second == L"SHADOW_CASCADE_MATRIX2")
                        {
                            if (m_RenderSettings.enableShadows)
                                material.addVar(gfx::ShaderVar::pointer(
                                    NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_ShadowCascadeMatrix2)));
                        }
                        else if (node.second == L"SHADOW_CASCADE_MATRIX3")
                        {
                            if (m_RenderSettings.enableShadows)
                                material.addVar(gfx::ShaderVar::pointer(
                                    NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_ShadowCascadeMatrix3)));
                        }

                        else if (node.second == L"SHADOW_CASCADE0")
                        {
                            if (m_RenderSettings.enableShadows)
                                material.addVar(gfx::ShaderVar::pointer(
                                    NEW gfx::ShaderGlobalVar(pShader->getShaderSamplerId(name), name, gfx::ShaderVarType_ShadowCascade0)));
                        }
                        else if (node.second == L"SHADOW_CASCADE1")
                        {
                            if (m_RenderSettings.enableShadows)
                                material.addVar(gfx::ShaderVar::pointer(
                                    NEW gfx::ShaderGlobalVar(pShader->getShaderSamplerId(name), name, gfx::ShaderVarType_ShadowCascade1)));
                        }
                        else if (node.second == L"SHADOW_CASCADE2")
                        {
                            if (m_RenderSettings.enableShadows)
                                material.addVar(gfx::ShaderVar::pointer(
                                    NEW gfx::ShaderGlobalVar(pShader->getShaderSamplerId(name), name, gfx::ShaderVarType_ShadowCascade2)));
                        }
                        else if (node.second == L"SHADOW_CASCADE3")
                        {
                            if (m_RenderSettings.enableShadows)
                                material.addVar(gfx::ShaderVar::pointer(
                                    NEW gfx::ShaderGlobalVar(pShader->getShaderSamplerId(name), name, gfx::ShaderVarType_ShadowCascade3)));
                        }

                        // Skinning
                        else if (node.second == L"BONETRANSFORMS")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderGlobalVar(pShader->getShaderVarId(name), name, gfx::ShaderVarType_BoneTransforms)));
                        }

                        // Texture
                        else if (node.second == L"TEXTURE2D")
                        {
                            gfx::Texture2D::pointer tex; 
                            vec4 testColorMap(reader.readVector4(L"TEXTURE2D", node.first, vec4(FLT_MIN, FLT_MIN, FLT_MIN, FLT_MIN)));
                            if (testColorMap == vec4(FLT_MIN, FLT_MIN, FLT_MIN, FLT_MIN))
                                tex = CONTENT->asyncLoadTexture(reader.readString(L"TEXTURE2D", node.first, ""));
                            else
                                tex = CONTENT->asyncMakeTexture(Color(testColorMap));
                            gfx::ShaderVar::pointer var(
                                NEW gfx::ShaderUserVar<gfx::Texture2D::pointer>(
                                    pShader->getShaderSamplerId(name), name, tex));
                            material.addVar(var);
                        }

                        // User
                        else if (node.second == L"FLOAT")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderUserVar<float>(pShader->getShaderVarId(name), name, 0)));
                        }
                        else if (node.second == L"VEC2")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderUserVar<vec2>(pShader->getShaderVarId(name), name, vec2(0, 0))));
                        }
                        else if (node.second == L"VEC3")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderUserVar<vec3>(pShader->getShaderVarId(name), name, vec3(0, 0, 0))));
                        }
                        else if (node.second == L"VEC4")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderUserVar<vec4>(pShader->getShaderVarId(name), name, vec4(0, 0, 0, 0))));
                        }
                        else if (node.second == L"INT")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderUserVar<int>(pShader->getShaderVarId(name), name, 0)));
                        }
                        else if (node.second == L"UINT")
                        {
                            material.addVar(gfx::ShaderVar::pointer(
                                NEW gfx::ShaderUserVar<uint>(pShader->getShaderVarId(name), name, 0)));
                        }
                        else
                        {
                            HE_ERROR("Material: unkown semantic " + std::string(node.second.cbegin(), node.second.cend()));
                        }
                    });
                }
            }
        }
        m_pAssetContainer->addAsset(path, material);
        return material;
    }
}

void MaterialLoader::setRenderSettings( const gfx::RenderSettings& settings )
{
    m_RenderSettings = settings;
}

} } //end namespace