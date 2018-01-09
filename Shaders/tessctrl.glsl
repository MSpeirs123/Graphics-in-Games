#version 400 core

layout(vertices =3) out;//num vertices in patch output

uniform float tessLevelInner;
uniform float tessLevelOuter;

in Vertex {
vec4 colour;
} IN[]; //An array of data from vertex shader

out Vertex {
vec4 colour;
} OUT[]; //Array of data to send to tessellator

patch out Vector4 subColour;

void main() {
gl_TessLevelInner[0] = tessLevelInner;
gl_TessLevelOuter[0] = tessLevelOuter;
gl_TessLevelOuter[1] = tessLevelOuter;
gl_TessLevelOuter[2] = tessLevelOuter;
gl_out[gl_InvocationID].gl_Position =
gl_in[gl_InvocationID].gl_Position;
}