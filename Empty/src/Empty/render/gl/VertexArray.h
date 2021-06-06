#pragma once

#include <string>

#include "Empty/render/gl/Buffer.h"
#include "Empty/render/gl/GLObject.h"
#include "Empty/render/gl/ShaderProgram.hpp"
#include "Empty/render/gl/VertexStructure.h"
#include "Empty/utils/utils.hpp"

namespace render::gl
{
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
		inline void bind() const { glBindVertexArray(*_id); }
		/**
		 * Unbinds the vertex array object.
		 */
		inline void unbind() const { glBindVertexArray(0); }

		inline void enableArray(int index) const { glEnableVertexArrayAttrib(*_id, index); }
		inline void disableArray(int index) const { glDisableVertexArrayAttrib(*_id, index); }

		/**
		 * Sets the vertex attributes structure of the currently bound buffer.
		 */
		void attachVertexBuffer(const BufferBinding& vbo, const VertexStructure& attribs, size_t offset = 0, int bindingIndex = 0)
		{
			// If attributes are interleaved, one single buffer binding is enough to provide for all vertex attribs
			if (attribs.isInterleaved())
			{
				glVertexArrayVertexBuffer(*_id, bindingIndex, *vbo.id(), offset, static_cast<GLsizei>(attribs.bytesPerVertex()));
				for (const auto& attrib : attribs.descriptors)
				{
					if (attrib.index >= 0)
					{
						enableArray(attrib.index);
						glVertexArrayAttribFormat(*_id, attrib.index, attrib.elems, utils::value(attrib.type), false, static_cast<GLuint>(attrib.offset));
						glVertexArrayAttribBinding(*_id, attrib.index, bindingIndex);
					}
				}
			}
			// Otherwise, we'll need one buffer binding per vertex attrib
			else
			{
				int bi = bindingIndex;
				for (const auto& attrib : attribs.descriptors)
				{
					glVertexArrayVertexBuffer(*_id, bi, *vbo.id(), offset + attrib.offset, static_cast<GLsizei>(attrib.size()));
					if (attrib.index >= 0)
					{
						enableArray(attrib.index);
						glVertexArrayAttribFormat(*_id, attrib.index, attrib.elems, utils::value(attrib.type), false, 0);
						glVertexArrayAttribBinding(*_id, attrib.index, bi);
						bi++;
					}
				}
			}
		}

		inline void attachElementBuffer(const BufferBinding& ibo)
		{
			glVertexArrayElementBuffer(*_id, *ibo.id());
		}

		inline const VertexArrayBinding getBindingInfo() const { return VertexArrayBinding(_id); }
	};
}
