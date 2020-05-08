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
	 * Names for a buffer's usage when uploading data.
	 */
	enum struct BufferUsage : GLenum
	{
		StreamDraw = GL_STREAM_DRAW,
		StreamRead = GL_STREAM_READ,
		StreamCopy = GL_STREAM_COPY,
		StaticDraw = GL_STATIC_DRAW,
		StaticRead = GL_STATIC_READ,
		StaticCopy = GL_STATIC_COPY,
		DynamicDraw = GL_DYNAMIC_DRAW,
		DynamicRead = GL_DYNAMIC_READ,
		DynamicCopy = GL_DYNAMIC_COPY
	};

	/**
	 * Names for a buffer's internal parameters.
	 */
	enum struct BufferParam : GLenum
	{
		Access = GL_BUFFER_ACCESS,
		AccessFlags = GL_BUFFER_ACCESS_FLAGS,
		ImmutableStorage = GL_BUFFER_IMMUTABLE_STORAGE,
		Mapped = GL_BUFFER_MAPPED,
		MapLength = GL_BUFFER_MAP_LENGTH,
		MapOffset = GL_BUFFER_MAP_OFFSET,
		Size = GL_BUFFER_SIZE,
		StorageFlags = GL_BUFFER_STORAGE_FLAGS,
		Usage = GL_BUFFER_USAGE
	};

	/**
	 * Possible values of buffer parameters that accept symbolic constants as values.
	 */
	enum struct BufferParamValue : GLenum
	{
		Invalid = GL_INVALID_ENUM,
		// Access & AccessFlags
		ReadOnly = GL_READ_ONLY,
		WriteOnly = GL_WRITE_ONLY,
		ReadWrite = GL_READ_WRITE,
		// StorageFlags from glBufferStorage
		StorageDynamic = GL_DYNAMIC_STORAGE_BIT,
		StorageMapRead = GL_MAP_READ_BIT,
		StorageMapWrite = GL_MAP_WRITE_BIT,
		StorageMapPersistent = GL_MAP_PERSISTENT_BIT,
		StorageMapCoherent = GL_MAP_COHERENT_BIT,
		StorageClient = GL_CLIENT_STORAGE_BIT,
		// StorageFlags from glBufferData
		Data = GL_MAP_READ_BIT | GL_MAP_WRITE_BIT | GL_DYNAMIC_STORAGE_BIT | GL_MAP_WRITE_BIT,
		// Usage
		StreamDraw = GL_STREAM_DRAW,
		StreamRead = GL_STREAM_READ,
		StreamCopy = GL_STREAM_COPY,
		StaticDraw = GL_STATIC_DRAW,
		StaticRead = GL_STATIC_READ,
		StaticCopy = GL_STATIC_COPY,
		DynamicDraw = GL_DYNAMIC_DRAW,
		DynamicRead = GL_DYNAMIC_READ,
		DynamicCopy = GL_DYNAMIC_COPY
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
		inline void uploadData(int size, BufferUsage usage, const void* data = nullptr) const
		{
			glBufferData(utils::value(_target), size, data, utils::value(usage));
		}

		template <BufferParam CTParam, std::enable_if_t <
			CTParam == BufferParam::ImmutableStorage
			|| CTParam == BufferParam::Mapped || CTParam == BufferParam::MapLength
			|| CTParam == BufferParam::MapOffset
			|| CTParam == BufferParam::Size
			, int> = 0>
			int getParameter() const
		{
			int p;
			glGetBufferParameteriv(utils::value(_target), utils::value(CTParam), &p);
			return p;
		}

		template <BufferParam CTParam, std::enable_if_t <
			CTParam == BufferParam::Access || CTParam == BufferParam::AccessFlags
			|| CTParam == BufferParam::StorageFlags
			|| CTParam == BufferParam::Usage
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