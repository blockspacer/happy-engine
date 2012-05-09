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
//Created: 12/08/2011
//Extended: Sebastiaan Sprengers

#ifndef _HE_FONT_LOADER_H_
#define _HE_FONT_LOADER_H_
#pragma once

#include "Font.h"
#include "AssetContainer.h"

#include "ft2build.h"
#include FT_FREETYPE_H

namespace he {
namespace ct {

class FontLoader
{
public:
	FontLoader();
    virtual ~FontLoader();

    bool load(const std::string& path, ushort size, gfx::Font* pOutFont);

private:

    /* DATAMEMBERS */
    AssetContainer<gfx::Font*>* m_pAssetContainer;

    FT_Library* m_pFTLibrary;

    //Disable default copy constructor and default assignment operator
    FontLoader(const FontLoader&);
    FontLoader& operator=(const FontLoader&);
};

} } //end namespace

#endif
