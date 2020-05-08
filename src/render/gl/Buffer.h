#pragma once

#include <glad/glad.h>

#include "render/gl/GLEnums.hpp"
#include "render/gl/GLObject.h"
#include "utils/macros.h"

namespace render::gl
{
	/**
	 * This struct holds everything one needs to bind a buffer. Useful in case
	 * one wants to bind the buffer without having access to the entire Buffer object.
	 */
	struct BufferBinding
	{
		const BufferTarget target = BufferTarget::Dynamic;
		const std::shared_ptr<BufferId> id;

		inline void bind() const
		{
			ASSERT(target != BufferTarget::Dynamic && id && "Invalid buffer binding");
			glBindBuffer(utils::value(target), *id);
		}

		inline void unbind() const
		{
			ASSERT(target != BufferTarget::Dynamic && "Invalid buffer binding");
			glBindBuffer(utils::value(target), 0);
		}
	};

	/**
	 * General-purpose buffer class. Does not map one-to-one with OpenGL buffer objects, but holds a shared reference
	 * to one. It is possible to instantiate buffers with target known or unknown at compile-time.
	 */
	template <BufferTarget CTTarget = BufferTarget::Dynamic>
	struct Buffer : public GLObject<BufferId>
	{
		template <BufferTarget t = CTTarget,
			std::enable_if_t<t == BufferTarget::Dynamic, int> = 0>
			Buffer(BufferTarget target) : _target(target) {}

		template <BufferTarget t = CTTarget,
			std::enable_if_t<t != BufferTarget::Dynamic, int> = 0>
			Buffer() : _target(t) {}

		/**
		 * Binds the buffer. This is necessary before any operation on the buffer is performed.
		 */
		inline void bind() const
		{
			glBindBuffer(utils::value(_target), *_id);
		}

		/**
		 * Unbinds the buffer, making sure it will not be modified by any further operation.
		 */
		inline void unbind() const
		{
			glBindBuffer(utils::value(_target), 0);
		}

		/**
		 * Allocate storage for and upload data to a buffer.
		 */
		inline void uploadData(size_t size, BufferUsage usage, const void* data = nullptr) const
		{
			glBufferData(utils::value(_target), size, data, utils::value(usage));
		}

		template <BufferParam CTParam, std::enable_if_t <
			CTParam == BufferParam::ImmutableStorage
			|| CTParam == BufferParam::Mapped
			, int> = 0>
			bool getParameter() const
		{
			int p;
			glGetBufferParameteriv(utils::value(_target), utils::value(CTParam), &p);
			return p;
		}

		template <BufferParam CTParam, std::enable_if_t <
			CTParam == BufferParam::MapLength || CTParam == BufferParam::MapOffset
			|| CTParam == BufferParam::Size
			, int> = 0>
			int64_t getParameter() const
		{
			int64_t p;
			glGetBufferParameteri64v(utils::value(_target), utils::value(CTParam), &p);
			return p;
		}

		template <BufferParam CTParam, std::enable_if_t <
			CTParam == BufferParam::Access || CTParam == BufferParam::Usage
			, int> = 0>
			BufferParamValue getParameter() const
		{
			int p;
			glGetBufferParameteriv(utils::value(_target), utils::value(CTParam), &p);
			return static_cast<BufferParamValue>(p);
		}

		BufferBinding getBindingInfo() const { return BufferBinding{ _target, _id}; }
		BufferTarget getTarget() const { return _target; }

	protected:
		/**
		 * Registered target of the buffer object.
		 */
		const BufferTarget _target;
	};
}