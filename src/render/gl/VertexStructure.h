#pragma once

#include <glad/glad.h>

#include "render/gl/GLEnums.hpp"

namespace render::gl
{
	struct VertexAttribDescriptor
	{
		const std::string name;
		VertexAttribType type;
		int elems;
	};

	struct VertexStructure
	{
		std::vector<VertexAttribDescriptor> descriptor;
		/**
		 * Number of vertices in the buffer when the attributes are separate. If the buffer attributes are interleaved, separateVertices is set to 0.
		 */
		size_t separateVertices = 0;
		bool isInterleaved() { return separateVertices <= 0; }

		void add(const std::string name, VertexAttribType type, int elems)
		{
			descriptor.push_back(VertexAttribDescriptor{ name, type, elems });
		}
	};
}