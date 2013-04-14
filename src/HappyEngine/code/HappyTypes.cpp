//HappyEngine Copyright (C) 2011 - 2012  Evil Interactive
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
//Created: 20/05/2012
#include "HappyPCH.h" 

#include "HappyTypes.h"

#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>

namespace he {

const ObjectHandle ObjectHandle::unassigned;
const net::NetworkObjectTypeID net::NetworkObjectTypeID::unassigned;

Guid::Guid(): m_Data1(0), m_Data2(0), m_Data3(0), m_Data4(0), m_Data5(0), m_Data6(0)
{

}

Guid::Guid( const char* const guid )
{
    HE_COMPILE_ASSERT(sizeof(uint32) == 4, "uint32 must be 4 byte big");
    HE_COMPILE_ASSERT(sizeof(uint16) == 2, "uint16 must be 2 byte big");
    HE_COMPILE_ASSERT(sizeof(Guid) == 16, "Error guid must be 16 byte");
    if (strlen(guid) == 36)
    {
        // last value will overflow in m_Data6
        sscanf(guid, "%8x-%4x-%4x-%4x-%4x%8x", &m_Data1, &m_Data2, &m_Data3, &m_Data4, &m_Data5, &m_Data6);
    }
    else
    {
        HE_ERROR("Failed converting guid: %s", guid);
    }
}

Guid::Guid( const Guid& other )
{
    he_memcpy(this, &other, sizeof(Guid));
}

Guid& Guid::operator=( const Guid& other )
{
    he_memcpy(this, &other, sizeof(Guid));
    return *this;
}

he::String Guid::toString() const
{
    char buff[37];
    sprintf(buff, "%08x-%04x-%04x-%04x-%04x%08x", m_Data1, m_Data2, m_Data3, m_Data4, m_Data5, m_Data6);
    return buff;
}

bool Guid::operator==( const Guid& other ) const
{
    return  m_Data1 == other.m_Data1 && 
            m_Data2 == other.m_Data2 && 
            m_Data3 == other.m_Data3 && 
            m_Data4 == other.m_Data4 && 
            m_Data5 == other.m_Data5 && 
            m_Data6 == other.m_Data6;
}

bool Guid::operator!=( const Guid& other ) const
{
    return  m_Data1 != other.m_Data1 || 
            m_Data2 != other.m_Data2 || 
            m_Data3 != other.m_Data3 || 
            m_Data4 != other.m_Data4 || 
            m_Data5 != other.m_Data5 || 
            m_Data6 != other.m_Data6;
}

he::Guid Guid::generateGuid()
{
    Guid g;
    boost::uuids::uuid uuid = boost::uuids::random_generator()();
    he_memcpy(&g, &uuid, sizeof(Guid));
    return g;
}

} //end namespace
