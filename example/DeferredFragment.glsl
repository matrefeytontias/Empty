#version 450

out vec4 colorOut;

layout(location = 1) uniform sampler2D uTexture;

in vec2 vTexCoords;

void main()
{
	colorOut = texture(uTexture, vTexCoords);
	// colorOut.rgb = 1. - colorOut.rgb;
}
