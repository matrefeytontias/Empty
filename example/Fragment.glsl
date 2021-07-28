#version 450

out vec4 colorOut;

uniform float uTime;
uniform sampler2D uTexture;

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoords;

void main()
{
	float factor = max(0, dot(vNormal.xyz, -normalize(vPosition.xyz)));
	float scaledTime = uTime * 15.5;
	// Color modifier for optimal Camellia effect
	vec4 modifier = vec4(cos(scaledTime), sin(scaledTime), sin(scaledTime * 1.5), 1.) * 0.5 + 0.5;
	colorOut = vec4(factor.xxx, 1.) * texture(uTexture, vTexCoords, (1. - pow(factor, 1.5)) * 10.) * modifier;
}
