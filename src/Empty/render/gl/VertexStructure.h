#pragma once

#include "Empty/glad/glad.h"
#include "Empty/render/gl/GLEnums.hpp"

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
		size_t offset;
		location index;
		inline size_t size() const { return elems * vertexElementSize(type); }
	};

	struct VertexStructure
	{
		std::vector<VertexAttribDescriptor> descriptors;
		
		/**
		 * separateVertices is the number of vertices in the buffer when the attributes are separate.
		 * If the buffer attributes are interleaved, separateVertices should be set to 0.
		 */
		VertexStructure(size_t separateVertices = 0) : _separateVertices(separateVertices) {}

		inline bool isInterleaved() const { return _separateVertices == 0; }

		/**
		 * Adds a new attribute to the structure.
		 */
		void add(const std::string& name, VertexAttribType type, int elems, location loc = -1)
		{
			size_t offset = 0;

			if (!descriptors.empty())
			{
				const auto& last = descriptors.back();
				if (isInterleaved())
					offset = last.size() + last.offset;
				else
					offset = _separateVertices * last.size() + last.offset;
			}

			descriptors.push_back(VertexAttribDescriptor{ name, type, elems, offset, loc });
		}

		inline size_t bytesPerVertex() const
		{
			size_t result = 0;
			for (const auto& d : descriptors)
				result += d.size();
			return result;
		}

	private:
		/**
		 * Number of vertices in the buffer when the attributes are separate. If the buffer attributes are interleaved, separateVertices is set to 0.
		 */
		size_t _separateVertices = 0;
	};
}