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
		void bindVertexAttribs(const VertexStructure& attribs)
		{
			for (const auto& attrib : attribs.descriptors)
			{
				if (attrib.index >= 0)
				{
					enableArray(attrib.index);
					glVertexAttribPointer(attrib.index, attrib.elems, utils::value(attrib.type), false, attrib.stride, reinterpret_cast<void*>(attrib.offset));
				}
			}
		}

		const VertexArrayBinding getBindingInfo() const { return VertexArrayBinding(_id); }
	};
}
