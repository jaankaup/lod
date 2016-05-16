#version 430 core

layout (location = 0) in vec3 Position_VS_in;
layout (location = 1) in vec2 TexCoord_VS_in;
layout (location = 2) in vec3 Normal_VS_in;

uniform mat4 MVP;

out vec3 WorldPos_FS_in;
out vec2 TexCoord_FS_in;
out vec3 Normal_FS_in;

void main()
{
    gl_Position    = MVP * vec4(Position_VS_in, 1.0f);
    WorldPos_FS_in = Position_VS_in;
    TexCoord_FS_in = TexCoord_VS_in;
    Normal_FS_in   = Normal_VS_in;
}