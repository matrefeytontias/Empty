#pragma once

#include "Empty/glad/glad.h"
#include "Empty/render/gl/GLEnums.hpp"
#include "Empty/render/gl/GLObject.h"
#include "Empty/utils/macros.h"

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
			ASSERT(_target != BufferTarget::Dynamic && _id && "Invalid buffer binding");
			glBindBuffer(utils::value(_target), *_id);
		}

		inline void unbind() const
		{
			ASSERT(_target != BufferTarget::Dynamic && "Invalid buffer binding");
			glBindBuffer(utils::value(_target), 0);
		}
	private:
		BufferTarget _target = BufferTarget::Dynamic;
		std::shared_ptr<BufferId> _id;
	};

	template<BufferTarget> struct Buffer;
	/**
	 * Structure used when mapping a buffer. Unmapping is done via
	 * destruction of this object ; as such, its usage should be encapsulated
	 * in a scope so the buffer is unmapped when this object goes out of scope.
	 */
	struct BufferMapping : public utils::noncopyable
	{
		/**
		 * Unmaps the buffer. Use this object in a scope to make sure this is called.
		 */
		~BufferMapping()
		{
			TRACE("Buffer unmapped !");
			_binding.bind();
			glUnmapBuffer(utils::value(_binding.target()));
			*_mapped = false;
		}

		uint8_t& operator[](size_t i) { return _data[i]; }
		uint8_t operator[](size_t i) const { return _data[i]; }

		/**
		 * Returns a reference to a value of the requested type situated at the i-th byte of the mapping.
		 */
		template <typename T = uint8_t>
		T& get(size_t i)
		{
			return *reinterpret_cast<T*>(_data + i);
		}

		/**
		 * Returns the value of the requested type situated at the i-th byte of the mapping.
		 */
		template <typename T = uint8_t>
		T get(size_t i) const
		{
			return *reinterpret_cast<T*>(_data + i);
		}

	private:
		template<BufferTarget> friend struct Buffer;
		BufferBinding _binding;
		uint8_t* _data;
		bool* _mapped;

		/**
		 * Maps the buffer. Use this object in a scope to make sure the buffer is unmapped
		 * when this object goes out of scope.
		 */
		BufferMapping(const BufferBinding& binding, BufferAccess access, bool *mapped) : _binding(binding), _mapped(mapped)
		{
			_data = reinterpret_cast<uint8_t*>(glMapBuffer(utils::value(_binding.target()), utils::value(access)));
			*_mapped = true;
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
		BufferMapping map(BufferAccess access)
		{
			ASSERT(!_mapped);
			return BufferMapping(getBindingInfo(), access, &_mapped);
		}

		/**
		 * Allocate storage for and upload data to a buffer.
		 */
		inline void uploadData(size_t size, BufferUsage usage, const void* data = nullptr) const
		{
			ASSERT(!_mapped);
			glBufferData(utils::value(_target), size, data, utils::value(usage));
		}

		inline void uploadSubData(size_t size, size_t offset , const void* data = nullptr) const
		{
			ASSERT(!_mapped);
			glBufferSubData(utils::value(_target), offset, size, data);
		}

		template <BufferParam CTParam, std::enable_if_t <
			CTParam == BufferParam::ImmutableStorage
			|| CTParam == BufferParam::Mapped
			, int> = 0>
			bool getParameter() const
		{
			ASSERT(!_mapped);
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
			ASSERT(!_mapped);
			int64_t p;
			glGetBufferParameteri64v(utils::value(_target), utils::value(CTParam), &p);
			return p;
		}

		template <BufferParam CTParam, std::enable_if_t <
			CTParam == BufferParam::Access, int> = 0>
			BufferAccess getParameter() const
		{
			ASSERT(!_mapped);
			int p;
			glGetBufferParameteriv(utils::value(_target), utils::value(CTParam), &p);
			return static_cast<BufferAccess>(p);
		}

		template <BufferParam CTParam, std::enable_if_t <
			CTParam == BufferParam::Usage, int> = 0>
			BufferUsage getParameter() const
		{
			ASSERT(!_mapped);
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
		bool _mapped = false;
	};
}