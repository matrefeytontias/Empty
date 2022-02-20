#pragma once

#include <string>

#include "Empty/render/gl/Buffer.h"
#include "Empty/render/gl/GLObject.h"
#include "Empty/render/gl/ShaderProgram.hpp"
#include "Empty/render/gl/VertexStructure.h"
#include "Empty/utils/utils.hpp"

namespace Empty::render::gl
{
	/**
	 * This struct holds everything one needs to bind a VertexArray. Useful in case
	 * one wants to bind the VertexArray without having access to the entire object.
	 */
	struct VertexArrayInfo
	{
		std::shared_ptr<VertexArrayId> id;
	};

	/**
	 * General-purpose vertex array class. Does not map one-to-one with OpenGL vertey array objects,
	 * but holds a shared reference to one.
	 */
	struct VertexArray : public GLObject<VertexArrayId>
	{
		/**
		 * Enables a generic vertex attribute array.
		 */
		inline void enableArray(int index) const { glEnableVertexArrayAttrib(*_id, index); }
		/**
		 * Disables a generic vertex attribute array.
		 */
		inline void disableArray(int index) const { glDisableVertexArrayAttrib(*_id, index); }

		/**
		 * Attaches a Buffer to the vertex array as a vertex buffer, using a given VertexStructure to
		 * interpret the data starting `offset` bytes into the buffer. This process can use anywhere
		 * from 1 to as many buffer bindings as there are attributes in the vertex structure, starting
		 * at `bindingIndex`.
		 */
		void attachVertexBuffer(const BufferInfo& vbo, const VertexStructure& attribs, size_t offset = 0, int bindingIndex = 0)
		{
			// If attributes are interleaved, one single buffer binding is enough to provide for all vertex attribs
			if (attribs.isInterleaved())
			{
				glVertexArrayVertexBuffer(*_id, bindingIndex, *vbo.id, offset, static_cast<GLsizei>(attribs.bytesPerVertex()));
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
					glVertexArrayVertexBuffer(*_id, bi, *vbo.id, offset + attrib.offset, static_cast<GLsizei>(attrib.size()));
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

		/**
		 * Attaches a Bufffer to the vertex array as an element buffer.
		 */
		inline void attachElementBuffer(const BufferInfo& ibo)
		{
			glVertexArrayElementBuffer(*_id, *ibo.id);
		}

		operator const VertexArrayInfo() const { return VertexArrayInfo{ _id }; }
		const VertexArrayInfo getInfo() const { return VertexArrayInfo{ _id }; }
	};
}
