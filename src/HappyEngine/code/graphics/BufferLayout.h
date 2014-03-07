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

#ifndef _HE_BUFFERLAYOUT_H_
#define _HE_BUFFERLAYOUT_H_
#pragma once


namespace he {
namespace gfx {

class VertexElement
{
public:
    enum Type
    {
        Type_Vec2,
        Type_Vec3,
        Type_Vec4,
        Type_Float,
        Type_Int,
        Type_IVec4,
        Type_UInt,
    };
    enum Usage
    {
        Usage_Position,
        Usage_TextureCoordinate,
        Usage_Normal,
        Usage_Tangent,
        Usage_BoneIDs,
        Usage_BoneWeights,
        Usage_Instancing,
        Usage_Color
    };
    VertexElement();
    VertexElement(const Type type, const Usage usage, const uint32 size, const uint32 byteOffset);
    virtual ~VertexElement() {}
    //default copy constructor and assignment operator are fine

    uint32 getSize() const; 
    uint32 getByteOffset() const; 
    Type getType() const;
    Usage getAttribute() const;

private:
    Type m_Type;
    Usage m_Usage;
    uint32 m_Size;
    uint32 m_ByteOffset;
};

class VertexLayout
{
public:
    typedef he::ObjectList<VertexElement> layout;

    VertexLayout();
    VertexLayout(const VertexLayout& other);
    VertexLayout& operator=(const VertexLayout& other);
    virtual ~VertexLayout();


    void addElement(const VertexElement& element);

    const layout& getElements() const;
    uint32 getSize() const;


private:
    layout m_Layout;
    uint32 m_Size;
};

} } //end namespace

#endif
