#version 450 core

// Attributes
layout (location=0) in vec2 aVertexPosition;
layout (location=1) in vec2 aVertexTextureCords;

layout (location=1) out vec2 vTextureCords;
uniform mat3 uModelToNDC;


void main()
{
	vec3 pos_xform = uModelToNDC * vec3(aVertexPosition, 1.0f);
	gl_Position = vec4(pos_xform, 1.0);
	
	vTextureCords = aVertexTextureCords;
}