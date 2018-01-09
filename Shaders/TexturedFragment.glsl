#version 150 core
uniform sampler2D diffuseTex;

in Vertex {
	vec2 texCoord;
	vec4 colour;
} IN;

out vec4 gl_FragColor;

void main(void){
	gl_FragColor = texture(diffuseTex, IN.texCoord);
	//gl_FragColor = vec4(IN.texCoord.x, IN.texCoord.y, 0, 1);
}
