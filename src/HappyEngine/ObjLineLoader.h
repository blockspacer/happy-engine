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
//Author: Bastian Damman
//Created: 01/09/2011

#ifndef _OBJ_LINE_LOADER_H_
#define _OBJ_LINE_LOADER_H_
#pragma once

#include <string>
#include <vector>

#include "Vector3.h"
#include "HappyTypes.h"

namespace happyengine {
namespace content {
namespace models {

class ObjLineLoader
{
public:
	ObjLineLoader();
    virtual ~ObjLineLoader();

    virtual void load(const std::string& path);

    virtual const std::vector<math::Vector3>& getPoints() const;

private:
    void read(const std::string& path);

    std::vector<math::Vector3> m_PointData;

    //Disable default copy constructor and default assignment operator
    ObjLineLoader(const ObjLineLoader&);
    ObjLineLoader& operator=(const ObjLineLoader&);
};

} } } //end namespace

#endif