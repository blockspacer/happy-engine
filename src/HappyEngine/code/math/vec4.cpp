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
//Created: 10/08/2011
#include "HappyPCH.h" 

#include "vec4.h"
#include "vec3.h"
#include "vec2.h"
#include "foundation/PxVec4.h"

namespace he {

const vec4 vec4::one(1, 1, 1, 1);
const vec4 vec4::zero(0, 0, 0, 0);

vec4::vec4(): x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
}
vec4::vec4(const physx::PxVec4& vec) : x(vec.x), y(vec.y), z(vec.z), w(vec.w)
{
}
vec4::vec4(const float val): x(val), y(val), z(val), w(val)
{
}
vec4::vec4(const vec3& vec, float w_): x(vec.x), y(vec.y), z(vec.z), w(w_)
{
}
vec4::vec4( const vec2& xy, float z_, float w_ ): x(xy.x), y(xy.y), z(z_), w(w_)
{

}
vec4::vec4( const vec2& xy, const vec2& zw ): x(xy.x), y(xy.y), z(zw.x), w(zw.y)
{

}

vec4::vec4(float x_, float y_, float z_, float w_): x(x_), y(y_), z(z_), w(w_)
{
}
vec4::~vec4()
{
}

vec4::vec4(const vec4& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
}
vec4& vec4::operator=(const vec4& other)
{
    x = other.x;
    y = other.y;
    z = other.z;
    w = other.w;
    return *this;
}

//>---------Getters----------------------->
vec3 vec4::xyz() const
{
    return vec3(x, y, z);
}
//<---------------------------------------<

//>---------Operators--------------------->
vec4 vec4::operator-() const
{
    return vec4(-x, -y, -z, -w);
}

vec4 vec4::operator*(float a) const
{
    return vec4(x*a, y*a, z*a, w*a);
}
vec4 vec4::operator/(float a) const
{
    return vec4(x/a, y/a, z/a, w/a);
}

vec4 vec4::operator+(const vec4& v) const
{
    return vec4(x + v.x, y + v.y, z + v.z, w + v.w);
}
vec4 vec4::operator-(const vec4& v) const
{
    return vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

vec4& vec4::operator+=(const vec4& v)
{
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
    return *this;
}
vec4& vec4::operator-=(const vec4& v)
{
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
    return *this;
}
vec4& vec4::operator*=(float a)
{
    x *= a;
    y *= a;
    z *= a;
    w *= a;
    return *this;
}
vec4& vec4::operator/=(float a)
{
    x /= a;
    y /= a;
    z /= a;
    w /= a;
    return *this;
}

bool vec4::operator==(const vec4& v) const
{
    return x == v.x && y == v.y && z == v.z && w == v.w;
}
bool vec4::operator!=(const vec4& v) const
{
    return x != v.x || y != v.y || z != v.z || w != v.w;
}
//<----------------------------------------<

} //end namespace
