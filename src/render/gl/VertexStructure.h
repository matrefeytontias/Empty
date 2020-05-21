#pragma once

#include <glad/glad.h>

#include "render/gl/GLEnums.hpp"

namespace render::gl
{
	constexpr int vertexElementSize(VertexAttribType type)
	{
		if (type == VertexAttribType::UByte || type == VertexAttribType::Byte)
			return 1;
		else if (type == VertexAttribType::UShort || type == VertexAttribType::Short
			|| type == VertexAttribType::Half)
			return 2;
		else if (type == VertexAttribType::Double)
			return 8;
		else
			return 4;
	}

	struct VertexAttribDescriptor
	{
		const std::string name;
		VertexAttribType type;
		int elems;
		int stride;
		size_t offset;
	};

	struct VertexStructure
	{
		std::vector<VertexAttribDescriptor> descriptor = {};
		
		/**
		 * separateVertices is the number of vertices in the buffer when the attributes are separate.
		 * If the buffer attributes are interleaved, separateVertices should be set to 0.
		 */
		VertexStructure(size_t separateVertices = 0) : _separateVertices(separateVertices)
		{}

		/**
		 * Adds a new attribute to the structure.
		 */
		void add(std::string name, VertexAttribType type, int elems)
		{
			int stride = 0;
			size_t offset = 0;

			if (!descriptor.empty())
			{
				if (isInterleaved())
				{
					stride = elems * vertexElementSize(type) + descriptor.front().stride;
					offset = static_cast<size_t>(descriptor.back().elems) * vertexElementSize(descriptor.back().type) + descriptor.back().offset;
					for (auto& attrib : descriptor)
					{
						attrib.stride = stride;
					}
				}
				else {
					offset = _separateVertices * descriptor.back().elems * vertexElementSize(descriptor.back().type) + descriptor.back().offset;
				}
			}

			descriptor.push_back(VertexAttribDescriptor{ name, type, elems, stride, offset });
		}

	private:
		/**
		 * Number of vertices in the buffer when the attributes are separate. If the buffer attributes are interleaved, separateVertices is set to 0.
		 */
		size_t _separateVertices = 0;

		bool isInterleaved() const { return _separateVertices <= 0; }
	};
}