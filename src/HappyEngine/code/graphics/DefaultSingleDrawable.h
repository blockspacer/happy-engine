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
//Created: 18/12/2011

#ifndef _HE_DEFAULT_SINGLE_DRAWABLE_H_
#define _HE_DEFAULT_SINGLE_DRAWABLE_H_
#pragma once

#include "IDrawable.h"

namespace he {
namespace gfx {

class DefaultSingleDrawable : public ISingleDrawable
{
public:
    DefaultSingleDrawable();
    virtual ~DefaultSingleDrawable();

    virtual const Material& getMaterial() const = 0;
    virtual void applyMaterial(const ICamera* pCamera) const; 
    virtual void applyMaterial(const Material& customMaterial, const ICamera* pCamera) const;

    virtual const ModelMesh::pointer& getModelMesh() const = 0;
    virtual mat44 getWorldMatrix() const = 0;

    virtual bool getCastsShadow() const;
    virtual void setCastsShadow(bool castShadow);

    virtual bool isVisible() const;
    virtual void setVisible(bool visible);

    virtual bool isInCamera(const ICamera* pCamera) const; 
    virtual float getDrawPriority(const ICamera* pCamera) const;

    virtual void draw();
    virtual void drawShadow();

private:

    bool m_isVisible;
    bool m_castsShadow;

    //Disable default copy constructor and default assignment operator
    DefaultSingleDrawable(const DefaultSingleDrawable&);
    DefaultSingleDrawable& operator=(const DefaultSingleDrawable&);
};

} } //end namespace

#endif