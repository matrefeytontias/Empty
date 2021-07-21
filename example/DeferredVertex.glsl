#version 450

in vec2 aPosition;

out vec2 vTexCoords;

void main()
{
	vTexCoords = aPosition;
	gl_Position = vec4(aPosition * 2 - 1, 0., 1.);
}
