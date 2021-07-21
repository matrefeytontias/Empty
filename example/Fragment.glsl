#version 450

out vec4 colorOut;

uniform float uTime;
uniform sampler2D uTexture;

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoords;

void main()
{
	// vec3 normal = normalize(cross(dFdx(vPosition.xyz), dFdy(vPosition.xyz)));
	vec4 modifier = vec4(cos(uTime), sin(uTime), cos(uTime)*sin(uTime), 1.);
	modifier.rgb = modifier.rgb * 0.5 + 0.5;
	// vec4 modifier = vec4(1.);
	colorOut = vec4(dot(vNormal.xyz, -normalize(vPosition.xyz)).xxx, 1.) * texture(uTexture, vTexCoords) * modifier;
}
