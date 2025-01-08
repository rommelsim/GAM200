#version 450 core

layout(location = 0) out vec4 fFragColor;
layout(location = 1) in vec2  vTextureCords;

uniform sampler2D uTex2D;

void main()
{
	fFragColor = texture(uTex2D, vTextureCords);
}