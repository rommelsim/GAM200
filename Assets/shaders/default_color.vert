#version 450 core
layout (location=0) in vec3 aVertexPosition;		
layout (location=1) in vec3 aVertexColor;

layout (location=0) out vec3 vColor;
uniform mat3 uModelToNDC;
void main()
{
	//gl_Position = vec4(vec3(uModelToNDC * aVertexPosition), 1.0);
	gl_Position = vec4(aVertexPosition, 1.0);
	vColor = aVertexColor;
}