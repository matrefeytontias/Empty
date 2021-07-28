#version 450

in vec3 aPosition;
in vec2 aTexCoords;
in vec3 aNormal;

uniform float uTime;
uniform mat4 uModel;
uniform mat4 uCamera;
uniform mat4 uP;

out vec4 vPosition;
out vec2 vTexCoords;
out vec4 vNormal;

void main()
{
	vPosition = inverse(uCamera) * uModel * vec4(aPosition, 1.);
	vTexCoords = aTexCoords;
	vNormal = inverse(uCamera) * uModel * vec4(aNormal, 0.);
	gl_Position = uP * vPosition;
}
