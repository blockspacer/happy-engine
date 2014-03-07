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
#include "HappyPCH.h" 
#include "VertexLayout.h"

namespace he {
namespace gfx {

#pragma region BufferElement
VertexElement::VertexElement(const Type type, const Usage usage, const uint32 size, const uint32 byteOffset):
        m_Type(type), m_Usage(usage),
        m_Size(size), m_ByteOffset(byteOffset)
{
    HE_ASSERT(m_Size != 0, "element size == 0");
}

VertexElement::VertexElement():
    m_Type(Type_Int), m_Usage(Usage_Other),
    m_Size(0), m_ByteOffset(0)
{

}

uint32 VertexElement::getSize() const
{
    return m_Size;
}
uint32 VertexElement::getByteOffset() const
{
    return m_ByteOffset;
}
VertexElement::Type VertexElement::getType() const
{
    return m_Type;
}
VertexElement::Usage VertexElement::getAttribute() const
{
    return m_Usage;
}
#pragma endregion

#pragma region BufferElement
VertexLayout::VertexLayout(): m_Size(0)
{
}

VertexLayout::VertexLayout(const VertexLayout& other ): m_Size(other.m_Size), m_Layout(other.m_Layout.size())
{
    m_Layout.append(other.m_Layout);
}

VertexLayout& VertexLayout::operator=( const VertexLayout& other )
{
    m_Size = other.m_Size;
    m_Layout.clear();
    m_Layout.reserve(other.m_Layout.size());
    m_Layout.append(other.m_Layout);

    return *this;
}

VertexLayout::~VertexLayout()
{
}

void VertexLayout::addElement(const VertexElement& element)
{
    m_Layout.add(element);
    m_Size += element.getSize();
}

const VertexLayout::layout& VertexLayout::getElements() const
{
    return m_Layout;
}
uint32 VertexLayout::getSize() const
{
    return m_Size;
}
#pragma endregion

} } //end namespace
