#version 330 core

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projMatrix;
uniform mat4 textureMatrix;


in  vec3 	position;
in  vec2 	texCoord;
in  vec4 	colour;

out Vertex	{
	vec2 texCoord;
} OUT;

void main(void)	{
	gl_Position		= vec4(position, 1.0);
	OUT.texCoord	= texCoord;
	
	OUT.texCoord.y = 1 - texCoord.y;
}