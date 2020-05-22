#pragma once

#include <glad/glad.h>

#include "render/gl/GLEnums.hpp"

namespace render::gl
{
	constexpr size_t vertexElementSize(VertexAttribType type)
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
		std::string name;
		VertexAttribType type;
		int elems;
		int stride;
		size_t offset;
	};

	struct VertexStructure
	{
		std::vector<VertexAttribDescriptor> descriptors;
		
		/**
		 * separateVertices is the number of vertices in the buffer when the attributes are separate.
		 * If the buffer attributes are interleaved, separateVertices should be set to 0.
		 */
		VertexStructure(size_t separateVertices = 0) : _separateVertices(separateVertices) {}

		/**
		 * Adds a new attribute to the structure.
		 */
		void add(const std::string& name, VertexAttribType type, int elems)
		{
			int stride = 0;
			size_t offset = 0;

			if (!descriptors.empty())
			{
				if (isInterleaved())
				{
					stride = elems * vertexElementSize(type) + descriptors.front().stride;
					offset = static_cast<size_t>(descriptors.back().elems) * vertexElementSize(descriptors.back().type) + descriptors.back().offset;
					for (auto& attrib : descriptors)
						attrib.stride = stride;
				}
				else
					offset = _separateVertices * descriptors.back().elems * vertexElementSize(descriptors.back().type) + descriptors.back().offset;
			}

			descriptors.push_back(VertexAttribDescriptor{ name, type, elems, stride, offset });
		}

		size_t bytesPerVertex() const
		{
			size_t result = 0;
			for (const auto& d : descriptors)
				result += vertexElementSize(d.type) * d.elems;
			return result;
		}

	private:
		/**
		 * Number of vertices in the buffer when the attributes are separate. If the buffer attributes are interleaved, separateVertices is set to 0.
		 */
		size_t _separateVertices = 0;

		bool isInterleaved() const { return _separateVertices <= 0; }
	};
}