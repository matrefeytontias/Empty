#pragma once

#include "Empty/render/gl/GLEnums.hpp"

namespace render::gl
{
	// ########################
	// # General enum structs #
	// ########################

	template <DataType type>
	struct _BaseType
	{
		typedef void Type;
	};

#define DEFINE_TYPE(pt, t)\
	template <>           \
	struct _BaseType<pt>  \
	{                     \
		typedef t Type;   \
	}

	DEFINE_TYPE(DataType::UByte, GLubyte);
	DEFINE_TYPE(DataType::Byte, GLbyte);
	DEFINE_TYPE(DataType::UShort, GLushort);
	DEFINE_TYPE(DataType::Short, GLshort);
	DEFINE_TYPE(DataType::UInt, GLuint);
	DEFINE_TYPE(DataType::Int, GLint);
	DEFINE_TYPE(DataType::Half, GLhalf);
	DEFINE_TYPE(DataType::Float, GLfloat);
	DEFINE_TYPE(DataType::UByte332, GLubyte);
	DEFINE_TYPE(DataType::UByte332Rev, GLubyte);
	DEFINE_TYPE(DataType::UShort565, GLushort);
	DEFINE_TYPE(DataType::UShort565Rev, GLushort);
	DEFINE_TYPE(DataType::UShort4, GLushort);
	DEFINE_TYPE(DataType::UShort4Rev, GLushort);
	DEFINE_TYPE(DataType::UShort5551, GLushort);
	DEFINE_TYPE(DataType::UShort5551Rev, GLushort);
	DEFINE_TYPE(DataType::UInt8, GLuint);
	DEFINE_TYPE(DataType::UInt8Rev, GLuint);
	DEFINE_TYPE(DataType::UIntAAA2, GLuint);
	DEFINE_TYPE(DataType::UIntAAA2Rev, GLuint);
#undef DEFINE_TYPE

	/**
	 * Get the base data type corresponding to a DataType enum value.
	 */
	template <DataType type>
	using BaseType = typename _BaseType<type>::Type;

	// ################################
	// # Texture-related enum structs #
	// ################################

	/**
	 * Returns the dimensionality of a texture with a given texture target.
	 */
	constexpr int dimensionsFromTarget(TextureTarget target)
	{
		if (target == TextureTarget::Dynamic)
			return 0;
		else if (target == TextureTarget::Texture1D
				 || target == TextureTarget::Proxy1D
				 || target == TextureTarget::TextureBuffer)
			return 1;
		else if (target == TextureTarget::Texture3D
				 || target == TextureTarget::Texture2DArray
				 || target == TextureTarget::Texture2DMultisampleArray
				 || target == TextureTarget::TextureCubemapArray
				 || target == TextureTarget::Proxy3D
				 || target == TextureTarget::Proxy2DArray
				 || target == TextureTarget::Proxy2DMultisampleArray
				 || target == TextureTarget::ProxyCubemapArray)
			return 3;
		else
			return 2;
	}

	/**
	 * Returns whether a given texture target references a cubemap or cubemap array texture target.
	 */
	constexpr bool isTargetCubemap(TextureTarget target)
	{
		return target == TextureTarget::TextureCubemap || target == TextureTarget::ProxyCubemap
			|| target == TextureTarget::TextureCubemapArray || target == TextureTarget::ProxyCubemapArray;
	}

	/**
	 * Returns whether a given texture target references a layered texture type.
	 */
	constexpr bool isTargetLayered(TextureTarget target)
	{
		return target == TextureTarget::Texture1DArray || target == TextureTarget::Proxy1DArray
			|| dimensionsFromTarget(target) == 3;
	}

	/**
	 * Returns whether a given texture target references a proxy target.
	 */
	constexpr bool isTargetProxy(TextureTarget target)
	{
		return target == TextureTarget::Proxy1D || target == TextureTarget::Proxy1DArray
			|| target == TextureTarget::Proxy2D || target == TextureTarget::Proxy2DArray
			|| target == TextureTarget::Proxy2DMultisample || target == TextureTarget::Proxy2DMultisampleArray
			|| target == TextureTarget::Proxy3D || target == TextureTarget::ProxyRectangle
			|| target == TextureTarget::ProxyCubemap || target == TextureTarget::ProxyCubemapArray;
	}

	/**
	 * Returns whether a given texture target is a special texture target. Those include
	 * `TextureRectangle` and `TextureBuffer`, and behave differently from other textures
	 * in some cases.
	 */
	constexpr bool isTargetSpecial(TextureTarget target)
	{
		return target == TextureTarget::TextureRectangle || target == TextureTarget::ProxyRectangle
			|| target == TextureTarget::TextureBuffer;
	}

	/**
	 * Returns whether a given texture target is multisampled.
	 */
	constexpr bool isTargetMultisampled(TextureTarget target)
	{
		return target == TextureTarget::Texture2DMultisample || target == TextureTarget::Proxy2DMultisample
			|| target == TextureTarget::Texture2DMultisampleArray || target == TextureTarget::Proxy2DMultisampleArray;
	}

	/**
	 * Returns whether a given texture target is the array equivalent of another texture target.
	 */
	constexpr bool isTargetArrayOf(TextureTarget test, TextureTarget src)
	{
		return src == TextureTarget::Texture1D && test == TextureTarget::Texture1DArray ||
			src == TextureTarget::Texture2D && test == TextureTarget::Texture2DArray ||
			src == TextureTarget::Texture2DMultisample && test == TextureTarget::Texture2DMultisampleArray ||
			src == TextureTarget::TextureCubemap && test == TextureTarget::TextureCubemapArray ||
			src == TextureTarget::Proxy1D && test == TextureTarget::Proxy1DArray ||
			src == TextureTarget::Proxy2D && test == TextureTarget::Proxy2DArray ||
			src == TextureTarget::Proxy2DMultisample && test == TextureTarget::Proxy2DMultisampleArray ||
			src == TextureTarget::ProxyCubemap && test == TextureTarget::ProxyCubemapArray;
	}

	/**
	 * Returns the index of a cubemap face.
	 */
	constexpr int cubemapFaceIndex(CubemapFace face)
	{
		return static_cast<int>(utils::value(face)) - static_cast<int>(utils::value(CubemapFace::PlusX));
	}
}
