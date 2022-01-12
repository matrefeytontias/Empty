#version 450

out vec4 colorOut;

uniform float uTime;
uniform sampler2D uTexture;
uniform usampler2D uProcTex;

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoords;

void main()
{
	float factor = max(0, dot(vNormal.xyz, -normalize(vPosition.xyz)));
	float scaledTime = uTime * 15.5;
	// Color modifier for optimal Camellia effect
	vec3 modifier = vec3(cos(scaledTime), sin(scaledTime), sin(scaledTime * 1.5)) * 0.5 + 0.5;
	vec4 tex = texture(uTexture, vTexCoords, (1. - pow(factor, 1.5)) * 10.);
	colorOut = vec4(factor * tex.rgb * texture(uProcTex, vTexCoords).r * modifier, tex.a);
}
