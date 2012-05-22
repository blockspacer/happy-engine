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
//Created: 17/03/2012

#ifndef _HE_OBJECT_FACTORY_H_
#define _HE_OBJECT_FACTORY_H_
#pragma once

namespace he {

template<typename T>
class ObjectFactory
{
public:
    ObjectFactory(): m_IncreaseSize(32), m_DisplayName("unknown")
    {
    }
    void init(size_t startSize, size_t increaseSize, const std::string& displayName)
    {
        m_IncreaseSize = increaseSize;
        m_DisplayName = displayName;
        resize(startSize);
    }
    virtual ~ObjectFactory()
    {
        destroyAll();
    }

    virtual void destroyAll()
    {
        for (ObjectHandle::Type i(0); i < m_Pool.size(); ++i)
        {
            if (m_Pool[i] != nullptr)
            {
                delete m_Pool[i];
                m_FreeHandles.push(i);
            }
            ++m_Salt[i];
            HE_ASSERT(m_Salt[i] + 1 < OBJECTHANDLE_MAX, "ObjectFactory (%s): salt is growing out of bounds", m_DisplayName.c_str());
        }
    }

    virtual ObjectHandle create()
    {
        ObjectHandle handle(getFreeHandle());
        m_Pool[handle.index] = NEW T();
        return handle;
    }
    virtual ObjectHandle create(T* obj)
    {
        ObjectHandle handle(getFreeHandle());
        m_Pool[handle.index] = obj;
        return handle;
    }
    virtual void destroyObject(const ObjectHandle& handle)
    {
        HE_ASSERT(handle != ObjectHandle::unassigned, "ObjectFactory (%s): destroying unassigned handle", m_DisplayName.c_str());
        if (m_Salt[handle.index] != handle.salt)
        {
            HE_ERROR("ObjectFactory (%s): salt mismatch when destroying object", m_DisplayName.c_str());
        }
        else
        {
            destroyAt(handle.index);
        }
    }
    virtual void destroyAt(ObjectHandle::Type index)
    {
        HE_ASSERT(m_Pool[index] != nullptr, "ObjectFactory (%s): destroying non existing handle", m_DisplayName.c_str());
        m_FreeHandles.push(index);
        delete m_Pool[index];
        m_Pool[index] = nullptr;
        ++m_Salt[index];
        HE_ASSERT(m_Salt[index] + 1 < OBJECTHANDLE_MAX, "ObjectFactory (%s): salt is growing out of bounds", m_DisplayName.c_str());
    }

    virtual T* get(const ObjectHandle& handle)
    {
        HE_ASSERT(handle != ObjectHandle::unassigned, "ObjectFactory (%s): getting unassigned handle", m_DisplayName.c_str());
        HE_ASSERT(m_Salt[handle.index] == handle.salt, "ObjectFactory (%s): salt mismatch when getting object", m_DisplayName.c_str());
        return m_Pool[handle.index];
    }
    virtual T* getAt(ObjectHandle::Type index)
    {
        return m_Pool[index];
    }

    virtual bool isAlive(const ObjectHandle& handle)
    {
        return m_Salt[handle.index] == handle.salt;
    }
    virtual bool isAliveAt(size_t index)
    {
        return m_Pool[index] != nullptr;
    }

protected:
    virtual void resize(size_t newSize)
    {
        HE_ASSERT(newSize < OBJECTHANDLE_MAX, "ObjectFactory (%s): resize out of range: %d", m_DisplayName.c_str(), (int)newSize);
        HE_WARNING("ObjectFactory (%s): increasing pool to %d", m_DisplayName.c_str(), (int)newSize);
        ObjectHandle::Type oldSize(static_cast<ObjectHandle::Type>(m_Pool.size()));
        m_Pool.resize(newSize);
        m_Salt.resize(newSize);
        for (ObjectHandle::Type i(oldSize); i < newSize; ++i)
        {
            m_FreeHandles.push(i);
            m_Pool[i] = nullptr;
            m_Salt[i] = 0;
        }
    }

    std::string m_DisplayName;

private:
    ObjectHandle getFreeHandle()
    {
        if (m_FreeHandles.size() == 0)
        {
            resize(m_Pool.size() + m_IncreaseSize);
        }

        ObjectHandle::Type index(m_FreeHandles.front());
        m_FreeHandles.pop();
        ObjectHandle handle;
        handle.index = index;
        handle.salt = m_Salt[index];       
        return handle;
    }

    size_t m_IncreaseSize;

    std::vector<T*> m_Pool;
    std::vector<ObjectHandle::Type> m_Salt;
    std::queue<ObjectHandle::Type> m_FreeHandles;

    //Disable default copy constructor and default assignment operator
    ObjectFactory(const ObjectFactory&);
    ObjectFactory& operator=(const ObjectFactory&);
};

} //end namespace

#endif
