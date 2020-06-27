out vec4 colorOut;

uniform float uTime;

in vec4 vPosition;

void main()
{
	vec3 normal = normalize(cross(dFdx(vPosition.xyz), dFdy(vPosition.xyz)));
	colorOut = vec4(dot(normal, -normalize(vPosition.xyz)).xxx, 1.);
}
