out vec4 colorOut;

uniform float uTime;

void main()
{
	colorOut = vec4(abs(cos(uTime)), abs(sin(uTime)), 0.0, 1.0);
}