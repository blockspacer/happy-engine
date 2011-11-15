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

#ifndef _HE_MATRIX_H_
#define _HE_MATRIX_H_
#pragma once

#include "PxMat44.h"
#include "vec3.h"
#include "vec4.h"

namespace he {

class mat44
{
private:
    physx::pubfnd3::PxMat44 m_Matrix;

public:
	mat44();
	explicit mat44(physx::pubfnd3::PxMat44 mat);
	mat44(const vec4& col0, const vec4& col1, const vec4& col2, const vec4& col3);
	mat44( float _00, float _01, float _02, float _03,
            float _10, float _11, float _12, float _13,
            float _20, float _21, float _22, float _23,
            float _30, float _31, float _32, float _33 );
    ~mat44();
    //default copy constructor and assignment operator are fine

    //static constructors
    static mat44 createPerspectiveLH(float fov, float viewWidth, float viewHeight, float zNear, float zFar);
    static mat44 createPerspectiveLH(float fov, float aspectRatio, float zNear, float zFar);
    static mat44 createPerspectiveRH(float fov, float viewWidth, float viewHeight, float zNear, float zFar);
    static mat44 createOrthoLH(float left, float right, float top, float bottom, float zNear, float zFar);
    static mat44 createOrthoRH(float left, float right, float top, float bottom, float zNear, float zFar);
    static mat44 createLookAtLH(const vec3& eye, const vec3& target, const vec3& up);
    static mat44 createLookAtRH(const vec3& eye, const vec3& target, const vec3& up);
    static mat44 createTranslation(const vec3& translation);
    static mat44 createRotation(const vec3& axis, float radians);
    static mat44 createScale(const vec3& scale);
    static mat44 createScale(float xScale, float yScale, float zScale);
    static mat44 createScale(float scale);

    //operators
    mat44 operator*(const mat44& mat) const;
    mat44 operator*(float scale) const;
    void operator*=(float scale);
    vec3 operator*(const vec3& vec) const;
    vec4 operator*(const vec4& vec) const;
    float operator()(int row, int column) const;
    float& operator()(int row, int column);

    //getters
    const float* toFloatArray() const;
    vec3 getTranslation() const;
    const physx::pubfnd3::PxMat44& getPhyicsMatrix() const;
    mat44 inverse() const;
    float getDeterminant() const;
    
	//Static
    static const mat44 Identity;
};

} //end namespace

#endif
