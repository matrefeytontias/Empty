in vec3 position;

uniform float uTime;

void main()
{ 
	gl_Position = vec4(position * cos(uTime), 1.);
}