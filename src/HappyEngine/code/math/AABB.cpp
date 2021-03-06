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
//Created: 28/09/2011
#include "HappyPCH.h" 

#include "AABB.h"

namespace he {

AABB AABB::calculateBoundingAABB( const void* pointCloud, uint32 num, uint32 stride, uint32 posOffset, gfx::EShaderAttributeTypeComponents comp )
{
    if (num < 2)
        return AABB(vec3(0, 0, 0), vec3(0, 0, 0));
    vec3 min(FLT_MAX, comp >= gfx::eShaderAttributeTypeComponents_2? FLT_MAX : 0.0f, comp >= gfx::eShaderAttributeTypeComponents_3? FLT_MAX : 0.0f);
    vec3 max(-FLT_MAX, comp >= gfx::eShaderAttributeTypeComponents_2? -FLT_MAX : 0.0f, comp >= gfx::eShaderAttributeTypeComponents_3? -FLT_MAX : 0.0f);

    const char* charPointCloud = static_cast<const char*>(pointCloud);
    for(uint32 i = 0; i < num; ++i)
    {
        switch (comp)
        {
        case he::gfx::eShaderAttributeTypeComponents_1:
            {
                const float& p(*reinterpret_cast<const float*>(charPointCloud + stride * i + posOffset));
                min.x = std::min(min.x, p);
                max.x = std::max(max.x, p);
            } break;
        case he::gfx::eShaderAttributeTypeComponents_2:
            {
                const vec3 p(*reinterpret_cast<const vec2*>(charPointCloud + stride * i + posOffset), 0);
                he::minPerComponent(min, p, min);
                he::maxPerComponent(max, p, max);
            } break;
        case he::gfx::eShaderAttributeTypeComponents_3:
            {
                const vec3& p(*reinterpret_cast<const vec3*>(charPointCloud + stride * i + posOffset));
                he::minPerComponent(min, p, min);
                he::maxPerComponent(max, p, max);
            } break;
        default:
            break;
        }
    }

    return AABB(min, max);
}

void AABB::generateVertices( he::PrimitiveList<vec3>& outBuffer ) const
{
    vec3 diff(m_BottomBackRight - m_TopFrontLeft);
    outBuffer.add(m_TopFrontLeft + vec3(0, 0, 0));
    outBuffer.add(m_TopFrontLeft + vec3(diff.x, 0, 0));
    outBuffer.add(m_TopFrontLeft + vec3(0, diff.y, 0));
    outBuffer.add(m_TopFrontLeft + vec3(diff.x, diff.y, 0));
    outBuffer.add(m_TopFrontLeft + vec3(0, 0, diff.z));
    outBuffer.add(m_TopFrontLeft + vec3(diff.x, 0, diff.z));
    outBuffer.add(m_TopFrontLeft + vec3(0, diff.y, diff.z));
    outBuffer.add(m_TopFrontLeft + vec3(diff.x, diff.y, diff.z));
}

void AABB::generateIndices( he::PrimitiveList<uint32>& outBuffer, uint32 offset ) const
{
    // Front
    outBuffer.add(offset + 0); outBuffer.add(offset + 1);
    outBuffer.add(offset + 0); outBuffer.add(offset + 2);
    outBuffer.add(offset + 1); outBuffer.add(offset + 3);
    outBuffer.add(offset + 2); outBuffer.add(offset + 3);

    // Back
    outBuffer.add(offset + 4); outBuffer.add(offset + 5);
    outBuffer.add(offset + 4); outBuffer.add(offset + 6);
    outBuffer.add(offset + 5); outBuffer.add(offset + 7);
    outBuffer.add(offset + 6); outBuffer.add(offset + 7);

    // Connection
    outBuffer.add(offset + 0); outBuffer.add(offset + 4);
    outBuffer.add(offset + 1); outBuffer.add(offset + 5);
    outBuffer.add(offset + 2); outBuffer.add(offset + 6);
    outBuffer.add(offset + 3); outBuffer.add(offset + 7);
}

} //end namespace