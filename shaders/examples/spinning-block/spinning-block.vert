#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 texCoord;

uniform mat4 transform;

out vec2 TexCoord;

void main()
{
    gl_Position = transform * vec4(position, 1.0);
    TexCoord = vec2(texCoord.x, texCoord.y);
}
