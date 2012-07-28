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
//Author:  Sebastiaan Sprengers
//Created: 12/10/2011

#ifndef _HE_SCROLLBAR_H_
#define _HE_SCROLLBAR_H_
#pragma once

#include "vec2.h"
#include <map>
#include "Hitregion.h"
#include "Color.h"
#include "Canvas2D.h"

namespace he {
namespace gui {

class Scrollbar
{
public:

	/* CONSTRUCTOR - DESTRUCTOR */
	Scrollbar(const vec2& pos, const vec2& size, float heightScrollbar);
    virtual ~Scrollbar();

	/* GENERAL */
	void tick();
	void draw(gfx::Canvas2D* canvas);

	/* SETTERS */
	void setBarPos(float barPos);
	void setColors(	const Color& backgroundColor = Color(0.6f,0.6f,0.6f),
					const Color& scrollbarColor = Color(0.6f,0.6f,0.8f),
					const Color& edgeColor = Color(0.1f,0.1f,0.1f));
    void setPosition(const vec2& pos);
    void setSize(const vec2& size);

	/* GETTERS */
	float getBarPos() const;

private:

	/* DATAMEMBERS */
	std::map<std::string, Color> m_Colors;

	Hitregion* m_pHitregion;

	float m_BarPos;
	float m_ScrollbarHeight;

	bool m_bDragging;

	vec2 m_Pos;
	vec2 m_Size;
	vec2 m_PreviousMousePos;

    /* DEFAULT COPY & ASSIGNMENT OPERATOR */
    Scrollbar(const Scrollbar&);
    Scrollbar& operator=(const Scrollbar&);
};

} } //end namespace

#endif
