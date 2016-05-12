#version 400
// *** Insert appropriate uniforms for the Phong model ***
// The mesh line settings

uniform struct LineInfo {
float Width;
vec4 Color;
} Line;

in vec3 GPosition;
in vec3 GNormal;
noperspective in vec3 GEdgeDistance;

layout( location = 0 ) out vec4 FragColor;

vec3 phongModel( vec3 pos, vec3 norm )
{
// *** Phong model evaluation code goes here ***
}
void main() {
// The shaded surface color.
vec4 color = vec4( phongModel(GPosition, GNormal), 1.0 );
// Find the smallest distance
float d = min( GEdgeDistance.x, GEdgeDistance.y );
d = min( d, GEdgeDistance.z );
// Determine the mix factor with the line color
float mixVal = smoothstep( Line.Width – 1,
Line.Width + 1, d );
// Mix the surface color with the line color
FragColor = mix( Line.Color, color, mixVal );
}