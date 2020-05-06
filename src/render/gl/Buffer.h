#pragma once

#include <glad/glad.h>

#include "render/gl/GLObject.h"
#include "utils/macros.h"

namespace render::gl
{
	/**
	 * Valid targets that a buffer can be bound to and adopt the type of.
	 * `Dynamic` is not a real target and is only present for templating purposes.
	 */
	enum struct BufferTarget : GLenum
	{
		Dynamic = GL_INVALID_ENUM,
		// Buffers
		Array = GL_ARRAY_BUFFER,
		AtomicCounter = GL_ATOMIC_COUNTER_BUFFER,
		CopyRead = GL_COPY_READ_BUFFER,
		CopyWrite = GL_COPY_WRITE_BUFFER,
		DispatchIndirect = GL_DISPATCH_INDIRECT_BUFFER,
		DrawIndirect = GL_DRAW_INDIRECT_BUFFER,
		ElementArray = GL_ELEMENT_ARRAY_BUFFER,
		PixelPack = GL_PIXEL_PACK_BUFFER,
		PixelUnpack = GL_PIXEL_UNPACK_BUFFER,
		Query = GL_QUERY_BUFFER,
		ShaderStorage = GL_SHADER_STORAGE_BUFFER,
		Texture = GL_TEXTURE_BUFFER,
		TransformFeedback = GL_TRANSFORM_FEEDBACK_BUFFER,
		Uniform = GL_UNIFORM_BUFFER
	};

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
	template <BufferTarget CBTarget = BufferTarget::Dynamic>
	struct Buffer : public GLObject<BufferId>
	{
		template <BufferTarget t = CBTarget,
			std::enable_if_t<t == BufferTarget::Dynamic, int> = 0>
			Buffer(BufferTarget target) : _target(target) {}

		template <BufferTarget t = CBTarget,
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

	protected:
		/**
		 * Registered target of the buffer object.
		 */
		const BufferTarget _target;
	};
}