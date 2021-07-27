#version 450

out vec4 colorOut;

uniform float uTime;
uniform sampler2D uTexture;

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoords;

void main()
{
	vec4 modifier = vec4(1.);
	float lod = sin(uTime) * 4 + 4;
	colorOut = vec4(dot(vNormal.xyz, -normalize(vPosition.xyz)).xxx, 1.) * texture(uTexture, vTexCoords, lod) * modifier;
}
