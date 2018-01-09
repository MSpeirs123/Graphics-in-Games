#version 150 core

uniform sampler2D diffuseTex ;
uniform sampler2D bumpTex ;
uniform sampler2DShadow shadowTex ; // NEW !

uniform vec4 lightColour ;
uniform vec3 lightPos ;
uniform vec3 cameraPos ;
uniform float lightRadius ;

in Vertex {
vec3 colour ;
vec2 texCoord ;
vec3 normal ;
vec3 tangent ;
vec3 binormal ;
vec3 worldPos ;
vec4 shadowProj ; // New !
} IN ;

out vec4 gl_FragColor ;

void main (void) {
	mat3 TBN = mat3 ( IN . tangent , IN . binormal , IN . normal );
	vec3 normal = IN.normal;

	vec4 diffuse = texture2D ( diffuseTex , IN . texCoord );

	vec3 incident = normalize ( lightPos - IN . worldPos );
	float lambert = max (0.0 , dot ( incident , normal )); // Different !

	float dist = length ( lightPos - IN . worldPos );
	float atten = 1.0 - clamp ( dist / lightRadius , 0.0 , 1.0);

	vec3 viewDir = normalize ( cameraPos - IN . worldPos );
	vec3 halfDir = normalize ( incident + viewDir );

	float rFactor = max (0.0 , dot ( halfDir , normal )); // Different !
	float sFactor = pow ( rFactor , 33.0 );
	float shadow = 1.0; // New !

	if( IN . shadowProj . w > 0.0) { // New !
	shadow = textureProj ( shadowTex , IN . shadowProj );
	}

	lambert *= shadow ; // New !

	vec3 colour = ( diffuse . rgb * lightColour . rgb );
	colour += ( lightColour . rgb * sFactor ) * 0.33;
	gl_FragColor = vec4 ( colour * atten * lambert , diffuse . a );
	gl_FragColor . rgb += ( diffuse . rgb * lightColour . rgb ) * 0.1;
	
	
	// vec3 clip_shadow = (IN.shadowProj.xyz/IN.shadowProj.w) * 2.f - 1.f;
	// if (abs(clip_shadow.x) <= 1.0f && abs(clip_shadow.y) <= 1.0f)
	// {
		// clip_shadow = clip_shadow * 0.5f + 0.5f;
		// gl_FragColor . rg = clip_shadow.xy;
	// }
}