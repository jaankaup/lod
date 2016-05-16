#version 430 core
layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 tex;
layout(location = 2) in vec3 nor;

//out vec4 in_Position_CS;
//out vec3 in_Normal_CS;
//out vec2 in_TextCoord_CS;

out vec3 WorldPos_CS_in;
//out vec2 TexCoord_CS_in;
//out vec3 Normal_CS_in;

void main(void) {

     WorldPos_CS_in = pos;
     //TexCoord_CS_in = in_TextCoord_VS;
     //Normal_CS_in = in_Normal_VS;

     //in_Position_CS = vec4(in_Position_CS,1.0f);
     //in_Normal_CS = in_Normal_VS;
     //in_TextCoord_CS = in_TextCoord_VS;
}