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
//Created: 09/10/2011
#include "HappyPCH.h" 

#include "Text.h"
#include "ContentManager.h"

namespace he {
namespace gui {

/* CONSTRUCTOR - DESTRUCTOR */
Text::Text(	gfx::Font* font,
			OverFlowType overflow) :	m_pFont(font),
										m_OverFlowType(overflow),
										m_HAlignment(HAlignment_Left),
										m_VAlignment(VAlignment_Top),
                                        m_Bounds(),
                                        m_HasBounds(false)
{
}
Text::Text(	const std::string& text,
			gfx::Font* font,
			OverFlowType overflow) :	m_pFont(font),
										m_OverFlowType(overflow),
										m_HAlignment(HAlignment_Left),
										m_VAlignment(VAlignment_Top),
                                        m_Bounds(),
                                        m_HasBounds(false)
{
	addLine(text);
}

Text::Text() :	m_OverFlowType(OverFlowType_Clip),
				m_HAlignment(HAlignment_Left),
				m_VAlignment(VAlignment_Top),
                m_Bounds(),
                m_HasBounds(false),
                m_pFont(nullptr)
{

}

Text::~Text()
{
}

/* GENERAL */
void Text::addLine(const std::string& string)
{
	m_Text.push_back(string);
}

void Text::clear()
{
	m_Text.clear();
}

/* SETTERS */
void Text::setLine(const std::string& string, uint lineNumber)
{
	if (lineNumber < m_Text.size())
	{
		m_Text[lineNumber] = string;
	}
}

void Text::setHorizontalAlignment(HAlignment alignment)
{
	m_HAlignment = alignment;
}

void Text::setVerticalAlignment(VAlignment alignment)
{
	m_VAlignment = alignment;
}

void Text::setBounds(const vec2& bounds)
{
    if (bounds != vec2())
    {
        m_Bounds = bounds;
        m_HasBounds = true;
    }
    else
    {
        m_HasBounds = false;
    }
}

/* GETTERS */
const std::string& Text::getLine(uint lineNumber) const
{
	return m_Text[lineNumber];
}

const std::vector<std::string>& Text::getText() const
{
	return m_Text;
}

bool Text::isEmpty() const
{
	return m_Text.empty();
}

bool Text::hasBounds() const
{
    return m_HasBounds;
}

Text::OverFlowType Text::getOverFlowType() const
{
	return m_OverFlowType;
}

Text::HAlignment Text::getHorizontalAlignment() const
{
	return m_HAlignment;
}

Text::VAlignment Text::getVerticalAlignment() const
{
	return m_VAlignment;
}

gfx::Font* Text::getFont() const
{
	return m_pFont;
}

const vec2& Text::getBounds() const
{
    return m_Bounds;
}

} } //end namespace