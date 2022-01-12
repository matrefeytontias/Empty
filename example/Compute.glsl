#version 450

layout(local_size_x = 4, local_size_y = 4) in;

layout(r8ui, binding = 0) writeonly uniform uimage2D uProcTex;

uniform uint uResolution;

void main()
{
	uvec2 pixel = gl_WorkGroupID.xy * gl_WorkGroupSize.xy + gl_LocalInvocationID.xy;

	imageStore(uProcTex, ivec2(pixel), uvec4(((pixel.x ^ pixel.y) >> (7 - uResolution)) & 1));
}
