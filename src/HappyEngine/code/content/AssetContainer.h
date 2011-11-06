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
//Author:	Bastian Damman
//Modified:	Sebastiaan Sprengers

#ifndef _HE_ASSET_CONTAINER_P_H_
#define _HE_ASSET_CONTAINER_P_H_
#pragma once

#include <map>
#include <string>
#include <iostream>
#include <functional>

#include "HappyEngine.h"
#include "Console.h"

namespace he {
namespace ct {
    
template<typename T>
class AssetContainer
{
public:
    AssetContainer(): f_DestroyAction(nullptr) { }
    AssetContainer(const std::function<void (const T&)>& destroyAction): f_DestroyAction(destroyAction) { }
    virtual ~AssetContainer(void) { removeAllAssets(); }

	bool isAssetPresent(const std::string &key) const
    {
	    return m_Map.find(key) != m_Map.end();
    }
    void addAsset(const std::string &key, const T& asset)
    {        
		CONSOLE->addMessage("adding asset: " + key, CMSG_TYPE_ENGINE);

	    m_Map[key] = asset;
    }
	void removeAsset(const std::string &key)
    {
	    CONSOLE->addMessage("releasing asset: " + key, CMSG_TYPE_ENGINE);

        if (f_DestroyAction != nullptr)
            f_DestroyAction(m_Map[key]);

	    m_Map.erase(key);
    }
	void removeAllAssets()
    {
	    std::for_each(m_Map.cbegin(), m_Map.cend(), [&](const std::pair<std::string, T>& obj)
	    {
	        CONSOLE->addMessage("releasing asset: " + obj.first, CMSG_TYPE_ENGINE);
            
            if (f_DestroyAction != nullptr)
                f_DestroyAction(obj.second);
	    });

	    m_Map.clear();
    }
    
    const T& getAsset(const std::string& key)
    {
        return m_Map[key];
    }

private:
    std::map<std::string, T> m_Map;
    std::function<void (const T&)> f_DestroyAction;	

    //Disable default copy constructor and assignment operator
    AssetContainer(const AssetContainer& copy);
    AssetContainer& operator=(const AssetContainer& other);
};

} } //end namespace

#endif