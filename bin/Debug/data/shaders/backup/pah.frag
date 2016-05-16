#version 330

in vec2 TexCoord0;
in vec3 Normal0;

out vec4 FragColor;

struct DirectionalLight
{
    vec3 Color;
    float AmbientIntensity;
    vec3 Direction;
    float DiffuseIntensity;
};

uniform DirectionalLight DLight;
uniform sampler2D myTextureSampler;

void main()
{
    vec4 AmbientColor = vec4(DLight.Color * DLight.AmbientIntensity, 1.0f);
	float DiffuseFactor = dot(normalize(Normal0), -DLight.Direction);
	
	vec4 DiffuseColor;

    if (DiffuseFactor > 0) {
        DiffuseColor = vec4(DLight.Color * DLight.DiffuseIntensity * DiffuseFactor, 1.0f);
    }
    else {
        DiffuseColor = vec4(0, 0, 0, 0);
    }
	
    FragColor = texture2D(myTextureSampler, TexCoord0.xy) * (AmbientColor + DiffuseColor);
}