#version 450 core
layout (location = 3) in vec4 vertex; // <vec2 pos, vec2 tex>
out vec2 TexCoords;

uniform mat4 projection;

void main()
{
    gl_Position = vec4((projection * vec4(vertex.xy, 1.0, 0.0)).xy, 0.0, 1.0);
    TexCoords = vertex.zw;
}  