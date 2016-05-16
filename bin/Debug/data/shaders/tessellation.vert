#version 430 core
layout(location = 0) in vec3 in_Position_VS;
layout(location = 1) in vec2 in_TextCoord_VS;
layout(location = 2) in vec3 in_Normal_VS;

// uniform mat4 M;

out vec3 WorldPos_CS_in;
out vec2 TexCoord_CS_in;
out vec3 Normal_CS_in;

void main(void) {

     //WorldPos_CS_in = (M * vec4(in_Position_VS, 1.0f)).xyz;
     //TexCoord_CS_in = in_TextCoord_VS;
     //Normal_CS_in = (M * vec4(in_Normal_VS, 0.0)).xyz;

     WorldPos_CS_in = in_Position_VS;
     TexCoord_CS_in = in_TextCoord_VS;
     Normal_CS_in = in_Normal_VS;
}