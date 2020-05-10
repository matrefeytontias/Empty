#pragma once

#include <string>

#include "render/gl/Buffer.h"
#include "render/gl/GLObject.h"
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
		const std::shared_ptr<VertexArrayId> id;

		void bind() { glBindVertexArray(*id); }
		void unbind() { glBindVertexArray(0); }
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

		VertexArrayBinding getBindingInfo() const { return VertexArrayBinding{_id}; }
	};
}
