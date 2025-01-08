#version 450 core
layout (location=0) in vec3 aVertexPosition;	
layout (location=2) in vec2 aTexture;

layout (location=1) out vec2 vTexture;
uniform mat3 uModelToNDC;
void main()
{
	//gl_Position = vec4(vec3(uModelToNDC * aVertexPosition), 1.0);
	gl_Position = vec4(aVertexPosition, 1.0);
	vTexture = aTexture;
}