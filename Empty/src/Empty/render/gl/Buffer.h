#pragma once

#include "glad/glad.h"
#include "Empty/render/gl/GLEnums.hpp"
#include "Empty/render/gl/GLObject.h"
#include "Empty/render/gl/GLEnumsUtils.h"
#include "Empty/utils/macros.h"

namespace render::gl
{
	/**
	 * This struct holds everything one needs to bind a Buffer. Useful in case
	 * one wants to bind the Buffer without having access to the entire object.
	 */
	struct BufferInfo
	{
		std::shared_ptr<BufferId> id;
	};

	struct Buffer;
	/**
	 * Structure used when mapping a Buffer. Unmapping is done via
	 * destruction of this object ; as such, its usage should be encapsulated
	 * in a scope so the buffer is unmapped when this object goes out of scope.
	 */
	struct BufferMapping : public utils::noncopyable
	{
		/**
		 * Unmaps the Buffer. Use this object in a scope to make sure this is called.
		 */
		~BufferMapping()
		{
			glUnmapNamedBuffer(*_binding.id);
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
		 * Returns a value of the requested type situated at the i-th byte of the mapping.
		 */
		template <typename T = uint8_t>
		T get(size_t i) const
		{
			return *reinterpret_cast<T*>(_data + i);
		}

	private:
		friend struct Buffer;
		BufferInfo _binding;
		uint8_t* _data;
		bool* _mapped;

		/**
		 * Maps the Buffer. Use this object in a scope to make sure the buffer is unmapped
		 * when this object goes out of scope.
		 */
		BufferMapping(const BufferInfo& binding, BufferAccess access, bool *mapped) : _binding(binding), _mapped(mapped)
		{
			_data = reinterpret_cast<uint8_t*>(glMapNamedBuffer(*_binding.id, utils::value(access)));
			*_mapped = true;
		}
	};

	/**
	 * General-purpose buffer class. Does not map one-to-one with OpenGL buffer objects, but holds a shared reference
	 * to one. Buffers have a mutable format, which means their storage requirements can be adjusted several times.
	 */
	struct Buffer : public GLObject<BufferId>
	{
		/**
		 * Maps the buffer to a pointer that contains the buffer's data. Returns a
		 * BufferMapping object ; unmapping is performed when said object is destroyed,
		 * usually by going out of scope.
		 */
		BufferMapping map(BufferAccess access)
		{
			ASSERT(!_mapped);
			return BufferMapping(getInfo(), access, &_mapped);
		}

		/**
		 * Allocate storage and optionally upload data to the buffer.
		 */
		inline void setStorage(size_t size, BufferUsage usage, const void* data = nullptr) const
		{
			ASSERT(!_mapped);
			glNamedBufferData(*_id, size, data, utils::value(usage));
		}

		/**
		 * Upload data to the buffer.
		 */
		inline void uploadData(size_t offset, size_t size, const void* data) const
		{
			ASSERT(!_mapped);
			glNamedBufferSubData(*_id, offset, size, data);
		}

		/**
		 * Clear the contents of the buffer with a given value. The type of the
		 * value depends on the provided DataType enum value.
		 */
		template <DataFormat CTFormat, DataType CTType>
		inline void clearData(BufferDataFormat internalFormat, BaseType<CTType> value)
		{
			glClearNamedBufferData(*_id, utils::value(internalFormat), utils::value(CTFormat), utils::value(CTType), &value);
		}

		template <BufferParam CTParam, std::enable_if_t <
			CTParam == BufferParam::ImmutableStorage
			|| CTParam == BufferParam::Mapped
			, int> = 0>
			bool getParameter() const
		{
			ASSERT(!_mapped);
			int p;
			glGetNamedBufferParameteriv(*_id, utils::value(CTParam), &p);
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
			glGetNamedBufferParameteri64v(*_id, utils::value(CTParam), &p);
			return p;
		}

		template <BufferParam CTParam, std::enable_if_t <
			CTParam == BufferParam::Access, int> = 0>
			BufferAccess getParameter() const
		{
			ASSERT(!_mapped);
			int p;
			glGetNamedBufferParameteriv(*_id, utils::value(CTParam), &p);
			return static_cast<BufferAccess>(p);
		}

		template <BufferParam CTParam, std::enable_if_t <
			CTParam == BufferParam::Usage, int> = 0>
			BufferUsage getParameter() const
		{
			ASSERT(!_mapped);
			int p;
			glGetNamedBufferParameteriv(*_id, utils::value(CTParam), &p);
			return static_cast<BufferUsage>(p);
		}

		operator const BufferInfo() const { return BufferInfo{ _id }; }
		const BufferInfo getInfo() const { return BufferInfo{ _id }; }

	protected:
		bool _mapped = false;
	};
}