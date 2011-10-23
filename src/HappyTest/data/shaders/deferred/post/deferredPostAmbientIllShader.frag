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
//Author:  Bastian Damman
//Created: 18/08/2011

#version 150 core

#include "decode.frag"

noperspective in vec2 texCoord;

out vec4 outColor;

struct AmbientLight
{
    vec3 color;
};

struct DirectionalLight
{
	vec3 color;
	vec3 direction;
};

uniform sampler2D colorIllMap;
uniform sampler2D normalMap;
uniform sampler2D sgMap;
uniform sampler2D depthMap;

uniform vec4 projParams;

uniform AmbientLight ambLight;
uniform DirectionalLight dirLight;

uniform mat4 mtxDirLight;
uniform sampler2D shadowMap;

float shadowCheck(vec3 position)
{
	vec4 coord = mtxDirLight * vec4(position, 1.0f);
	coord.xyz /= coord.w;
	if (coord.x < -1 || coord.y < -1 || coord.x > 1 || coord.y > 1 ||
		coord.z < 0)
		return 0.25f;

	//NDC -> texturespace
	coord.x = (coord.x + 1.0f) / 2.0f;
	coord.y = (coord.y + 1.0f) / 2.0f;
	coord.z = (coord.z + 1.0f) / 2.0f;
	
	float shadow = 0;
	//[unroll]
	//for (int tx = -1.3f; tx <= 1.3; tx += 1.3f)
		//[unroll]
		//for (int ty = -1.3f; ty <= 1.3f; ty += 1.3f)
	shadow += (texture2D(shadowMap, coord.xy).r <= coord.z - 0.000025f)? 0.25f : 1.0f;
	
	//shadow /= 9.0f;

	return shadow;
}

void main()
{    
    vec2 ndc = texCoord * 2.0f - 1.0f;
	    
	vec3 position = getPosition( texture2D(depthMap, texCoord).x, ndc, projParams );

	vec3 lightDir = dirLight.direction;
	    
	vec3 normal = decodeNormal(texture2D(normalMap, texCoord).xy);
    
	float dotLightNormal = max(0.0f, dot(lightDir, normal));
	
	vec4 sg = texture2D(sgMap, texCoord);	
	vec3 vCamDir = normalize(-position);
	float spec = max(0, pow(dot(reflect(-lightDir, normal), vCamDir), sg.g * 100.0f) * sg.r);
	
	vec4 color = texture2D(colorIllMap, texCoord);

	vec3 dirColor = (dotLightNormal * color.rgb + vec3(spec, spec, spec) * 5.0f) * dirLight.color;
	dirColor *= shadowCheck(position);
	
	outColor = vec4(color.rgb * ambLight.color + 
					color.rgb * color.a * 10.0f + 
					dirColor, 1.0f);						
}