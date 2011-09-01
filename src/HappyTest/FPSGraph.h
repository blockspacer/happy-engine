//HappyTest Copyright (C) 2011  Bastian Damman, Sebastiaan Sprengers
//
//This file is part of HappyTest.
//
//    HappyTest is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    HappyTest is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with HappyTest.  If not, see <http://www.gnu.org/licenses/>.
//
//Author: Sebastiaan Sprengers 
//Created: 30/08/2011

#ifndef _FPS_GRAPH_H_
#define _FPS_GRAPH_H_
#pragma once

#define BOOST_DISABLE_ASSERTS

#include <vector>
#include "HappyTypes.h"

namespace happytest {

class FPSGraph
{
public:
	FPSGraph();
    virtual ~FPSGraph();

	void show(float dTime, float interval = 0.5f);

private:

	std::vector<happyengine::uint> m_FpsHistory;

	float m_GameTime;
	float m_TBase;

    //Disable default copy constructor and default assignment operator
    FPSGraph(const FPSGraph&);
    FPSGraph& operator=(const FPSGraph&);
};

} //end namespace

#endif
