#version 150 core

uniform sampler2D reflectionTex;
uniform sampler2D refractionTex;

in Vertex {
vec2 texCoords;
} IN;

out vec4 out_Color;

void main(void) {
vec4 reflectColour = texture(reflectionTex, texCoords);
vec4 refractColour = texture(refractionTex, texCoords);
out_Color = vec4 mix(reflectColour, refractColour, 0.5);
}