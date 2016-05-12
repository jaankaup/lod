#version 330
in vec3 LightIntensity;
layout( location = 0 ) out vec4 FragColor;
//out vec3 color;

void main() {
 FragColor = vec4(LightIntensity, 1.0);
 //color = LightIntensity;
}