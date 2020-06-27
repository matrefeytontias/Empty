in vec3 position;

uniform float uTime;
uniform mat4 uCamera;
uniform mat4 uP;

out vec4 vPosition;

void main()
{ 
	vPosition = inverse(uCamera) * vec4(position, 1.);
	gl_Position = uP * vPosition;
}
