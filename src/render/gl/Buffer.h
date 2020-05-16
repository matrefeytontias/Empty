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
		BufferBinding() = default;
		BufferBinding(BufferTarget target, std::shared_ptr<BufferId> id)
			: _target(target), _id(id)
		{}

		BufferTarget target() const { return _target; }
		std::shared_ptr<BufferId> id() const { return _id; }

		inline void bind() const
		{
			ASSERT(target != BufferTarget::Dynamic && id && "Invalid buffer binding");
			glBindBuffer(utils::value(_target), *_id);
		}

		inline void unbind() const
		{
			ASSERT(target != BufferTarget::Dynamic && "Invalid buffer binding");
			glBindBuffer(utils::value(_target), 0);
		}
	private:
		BufferTarget _target = BufferTarget::Dynamic;
		std::shared_ptr<BufferId> _id;
	};

	template<BufferTarget> struct Buffer;
	/**
	 * This struct is used when mapping a buffer.
	 */
	struct BufferMapping
	{
		BufferMapping() {}

		/**
		 * Unmaps the buffer and deletes the access to the pointer.
		 */
		inline bool unmap()
		{
			_data = nullptr;
			_binding.bind();
			return glUnmapBuffer(utils::value(_binding.target()));
		}

		inline bool isMapped() const { return _data != nullptr; }

	private:
		template<BufferTarget> friend struct Buffer;
		void* _data = nullptr;
		BufferBinding _binding;

		/**
		 * Maps the buffer.
		 */
		inline void map(BufferBinding binding, BufferAccess access)
		{
			_binding = binding;
			_data = glMapBuffer(utils::value(_binding.target()), utils::value(access));
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
		 * Binding a buffer modifies the currently bound VAO (and thus requires one), except for
		 * the `Array` target.
		 */
		inline void bind() const
		{
			glBindBuffer(utils::value(_target), *_id);
		}

		/**
		 * Unbinds the buffer, making sure it will not be modified by any further operation.
		 * Binding a buffer modifies the currently bound VAO (and thus requires one), except for
		 * the `Array` target.
		 */
		inline void unbind() const
		{
			glBindBuffer(utils::value(_target), 0);
		}

		/**
		 * Maps the buffer to a pointer that contains the buffer's data.
		 */
		inline BufferMapping map(BufferAccess access)
		{
			ASSERT(!mapping.isMapped());
			mapping.map(getBindingInfo(), access);
			return mapping;
		}

		/**
		 * Allocate storage for and upload data to a buffer.
		 */
		inline void uploadData(size_t size, BufferUsage usage, const void* data = nullptr) const
		{
			ASSERT(!mapping.isMapped());
			glBufferData(utils::value(_target), size, data, utils::value(usage));
		}

		template <BufferParam CTParam, std::enable_if_t <
			CTParam == BufferParam::ImmutableStorage
			|| CTParam == BufferParam::Mapped
			, int> = 0>
			bool getParameter() const
		{
			ASSERT(!mapping.isMapped());
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
			ASSERT(!mapping.isMapped());
			int64_t p;
			glGetBufferParameteri64v(utils::value(_target), utils::value(CTParam), &p);
			return p;
		}

		template <BufferParam CTParam, std::enable_if_t <
			CTParam == BufferParam::Access, int> = 0>
			BufferAccess getParameter() const
		{
			ASSERT(!mapping.isMapped());
			int p;
			glGetBufferParameteriv(utils::value(_target), utils::value(CTParam), &p);
			return static_cast<BufferAccess>(p);
		}

		template <BufferParam CTParam, std::enable_if_t <
			CTParam == BufferParam::Usage, int> = 0>
			BufferUsage getParameter() const
		{
			ASSERT(!mapping.isMapped());
			int p;
			glGetBufferParameteriv(utils::value(_target), utils::value(CTParam), &p);
			return static_cast<BufferUsage>(p);
		}

		BufferBinding getBindingInfo() const { return BufferBinding(_target, _id); }
		BufferTarget getTarget() const { return _target; }

	protected:
		/**
		 * Registered target of the buffer object.
		 */
		const BufferTarget _target;
		BufferMapping mapping;
	};
}