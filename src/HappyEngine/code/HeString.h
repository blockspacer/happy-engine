//HappyEngine Copyright (C) 2011 - 2014  Evil Interactive
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
//Created: 2013/03/09

#ifndef _HE_String_H_
#define _HE_String_H_
#pragma once

#include <string>
#include "HappyAllocater.h"

namespace he {

typedef he::StlAllocater<char> THeStringAllocator;
typedef std::basic_string<char, std::char_traits<char>, THeStringAllocator > StdString;
typedef std::basic_stringstream<char, std::char_traits<char>, THeStringAllocator > StringStream;
typedef StdString String;

HAPPY_ENTRY void toLower(char* const buff);
HAPPY_ENTRY uint32 hash(const uint32 oldHash, const he::String& str);

} //end namespace

#endif
