#version 430 core

in vec3 WorldPos_FS_in;
in vec2 TexCoord_FS_in;
in vec3 Normal_FS_in;

// Ouput data
out vec3 color;

uniform sampler2D myTextureSampler;
uniform mat4 V;
uniform mat4 M;
uniform vec3 LightPosition_worldspace;
uniform vec3 gEyeWorldPos;
uniform mat3 normalMatrix;

void main(){

	vec3 vertexPosition_cameraspace = (V * M * vec4(WorldPos_FS_in,1)).xyz; // OK.
	vec3 EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace; // OK.

	vec3 LightPosition_cameraspace = (V * M * vec4(gEyeWorldPos,1)).xyz; // OK.
	vec3 LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace; // OK.

	//vec3 Normal_cameraspace = (V * vec4(Normal_FS_in,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
        vec3 Normal_cameraspace = normalMatrix * Normal_FS_in;

	vec3 LightColor = vec3(1,1,1);
	float LightPower = 800.0f;

	vec3 MaterialDiffuseColor = texture( myTextureSampler, TexCoord_FS_in).rgb;
	vec3 MaterialAmbientColor = vec3(0.3,0.3,0.3) * MaterialDiffuseColor;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	float distance = length(LightPosition_worldspace - (M * vec4(WorldPos_FS_in,1.0)).xyz);

	vec3 n = normalize(Normal_cameraspace);
	vec3 l = normalize(LightDirection_cameraspace);
	float cosTheta = clamp(dot(n,l), 0, 1);

	vec3 E = normalize(EyeDirection_cameraspace);
	vec3 R = reflect(-l,n);
	float cosAlpha = clamp( dot( E,R ), 0,1 );

	color =
		// Ambient : simulates indirect lighting
		MaterialAmbientColor +
		// Diffuse : "color" of the object
		MaterialDiffuseColor * LightColor * LightPower * cosTheta / (distance*distance) +
		// Specular : reflective highlight, like a mirror
		MaterialSpecularColor * LightColor * LightPower * pow(cosAlpha,5) / (distance*distance);
}