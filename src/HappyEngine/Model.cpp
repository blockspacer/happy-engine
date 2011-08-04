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

#include "Model.h"

#include "GL/glew.h"
#include <algorithm>
#include "Assert.h"

namespace happyengine {
namespace graphics {

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Model::Model(): m_NumVertices(0), m_NumIndices(0)
{
    glGenVertexArrays(1, &m_VaoID[0]);
}


Model::~Model()
{
    
}
//Calling glBufferData with a NULL pointer before uploading new data can improve performance (tells the driver you don't care about the old contents)
void Model::setVertices(const void* pVertices, unsigned int num, const VertexLayout& vertexLayout)
{
    ASSERT(m_NumVertices == 0, "you can only set the vertices once, use DynamicModel instead");
    m_NumVertices = num;

    glBindVertexArray(m_VaoID[0]);
    glEnableVertexAttribArray(m_VaoID[0]);

    VertexLayout::layout elements(vertexLayout.getElements());

    glGenBuffers(1, m_VertexVboID);
    glBindBuffer(GL_ARRAY_BUFFER, m_VertexVboID[0]);
    glBufferData(GL_ARRAY_BUFFER, vertexLayout.getVertexSize() * num, pVertices, GL_STATIC_DRAW);

    std::for_each(elements.cbegin(), elements.cend(), [&](const VertexElement& e)
    {
        GLint components = 1;
        GLenum type = 0;
        switch (e.getType())
        {
            case VertexElement::Type_Vector2: type = GL_FLOAT; components = 2; break;
            case VertexElement::Type_Vector3: type = GL_FLOAT; components = 3; break;
            case VertexElement::Type_Vector4: type = GL_FLOAT; components = 4; break;
            case VertexElement::Type_Float: type = GL_FLOAT; break;
            case VertexElement::Type_Double: type = GL_DOUBLE; break;

            case VertexElement::Type_Int: type = GL_INT; break;
            case VertexElement::Type_UInt: type = GL_UNSIGNED_INT; break;
                
            case VertexElement::Type_Short: type = GL_SHORT; break;
            case VertexElement::Type_UShort: type = GL_UNSIGNED_SHORT; break;

            case VertexElement::Type_Byte: type = GL_BYTE; break;
            case VertexElement::Type_UByte: type = GL_UNSIGNED_BYTE; break;
            
            default: ASSERT("unknown type"); break; 
        }
        glVertexAttribPointer(e.getElementIndex(), components, type, GL_FALSE, vertexLayout.getVertexSize(), 
            BUFFER_OFFSET(e.getByteOffset())); 
    });

    //unbind
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}
void Model::setIndices(const void* pIndices, unsigned int num, IndexType type)
{
    ASSERT(m_NumIndices == 0, "you can only set the indices once, use DynamicModel instead");
    m_NumIndices = num;
    
    glBindVertexArray(m_VaoID[0]);
    glGenBuffers(1, m_IndexVboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IndexVboID[0]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, type * num, pIndices, GL_STATIC_DRAW);

    switch (type)
    {
        case IndexType_Byte: m_IndexType = GL_UNSIGNED_BYTE; break;
        case IndexType_UShort: m_IndexType = GL_UNSIGNED_SHORT; break;
        case IndexType_UInt: m_IndexType = GL_UNSIGNED_INT; break;
        default: ASSERT("unkown type"); break;
    }
}

unsigned int Model::getVertexArraysID() const
{
    return m_VaoID[0];
}

unsigned int Model::getNumVertices() const
{
    return m_NumVertices;
}
unsigned int Model::getNumIndices() const
{
    return m_NumIndices;
}

unsigned int Model::getIndexType() const
{
    return m_IndexType;
}


} } //end namespace