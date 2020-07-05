out vec4 colorOut;

uniform float uTime;
uniform sampler2D uTexture;

in vec4 vPosition;
in vec4 vNormal;
in vec2 vTexCoords;

void main()
{
	// vec3 normal = normalize(cross(dFdx(vPosition.xyz), dFdy(vPosition.xyz)));
	colorOut = vec4(dot(vNormal.xyz, -normalize(vPosition.xyz)).xxx, 1.) * texture(uTexture, vTexCoords);
	//colorOut = vec4(vTexCoords, 0., 1.);
}
