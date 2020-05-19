#pragma once

#include <string>

#include "render/gl/Buffer.h"
#include "render/gl/GLObject.h"
#include "render/gl/VertexStructure.h"
#include "utils/utils.hpp"

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

	struct VertexArrayBinding
	{
		VertexArrayBinding() = default;
		VertexArrayBinding(std::shared_ptr<VertexArrayId> id) : _id(id) {}

		std::shared_ptr<VertexArrayId> id() const { return _id; }

		void bind() { glBindVertexArray(*_id); }
		void unbind() { glBindVertexArray(0); }
	private:
		std::shared_ptr<VertexArrayId> _id;
	};

	struct VertexArray : public GLObject<VertexArrayId>
	{
		/**
		 * Binds the vertex array object.
		 */
		void bind() const { glBindVertexArray(*_id); }
		/**
		 * Unbinds the vertex array object.
		 */
		void unbind() const { glBindVertexArray(0); }

		void enableArray(int index) const { glEnableVertexAttribArray(index); }
		void disableArray(int index) const { glDisableVertexAttribArray(index); }

		/**
		 * Sets the vertex attributes structure of the currently bound buffer.
		 */
		void bindVertexAttribs(const VertexStructure& attribs, ShaderProgram& program)
		{
			size_t offset = 0;
			if (attribs.isInterleaved())
			{
				int stride = 0;
				for (auto& attrib : attribs.descriptor)
				{
					stride += attrib.elems * vertexElementSize(attrib.type);
				}

				for (auto& attrib : attribs.descriptor)
				{
					int index = program.findAttribute(attrib.name);
					if (index >= 0)
					{
						enableArray(index);
						glVertexAttribPointer(index, attrib.elems, utils::value(attrib.type), false, stride, reinterpret_cast<void*>(offset));
					}
					offset += static_cast<size_t>(attrib.elems) * vertexElementSize(attrib.type);
				}
			}

			else {
				for (auto& attrib : attribs.descriptor)
				{
					int index = program.findAttribute(attrib.name);
					if (index >= 0)
					{
						enableArray(index);
						glVertexAttribPointer(index, attrib.elems, utils::value(attrib.type), false, 0, reinterpret_cast<void*>(offset));
					}
					offset += attribs.separateVertices * attrib.elems * vertexElementSize(attrib.type);
				}
			}
		}

		const VertexArrayBinding getBindingInfo() const { return VertexArrayBinding(_id); }
	};
}
