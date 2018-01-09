#version 150 core

uniform sampler2D diffuseTex ;

uniform float brightness;

in Vertex {
vec2 texCoord ;
vec4 colour ;
} IN ;

out Vertex {
vec2 texCoord ;
vec4 colour ;
} OUT;

void main ( void ) {
if (brightness < 1.0) {
	OUT.colour = (0.0, 0.0, 0.0, 0.0);
}
else {
	OUT.colour = colour;
}