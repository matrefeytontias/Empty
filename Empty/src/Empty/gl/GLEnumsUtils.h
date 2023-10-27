#pragma once

#include "Empty/gl/GLEnums.hpp"
#include "Empty/utils/utils.hpp"

namespace Empty::gl
{
	// ########################
	// # General enum structs #
	// ########################

	namespace internal
	{
		template <typename T, T type>
		struct _BaseType
		{
			using Type = void;
		};

#define DEFINE_TYPE(d, t)\
		template <>           \
		struct _BaseType<DataType, d>  \
		{                     \
			using Type = t;   \
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

		template <TextureFormat f>
		struct _BaseType<
			std::enable_if_t<Empty::utils::isOneOf(f,
				TextureFormat::Depth, TextureFormat::Red16f, TextureFormat::RG16f, TextureFormat::RGB16f,
				TextureFormat::RGBA16f, TextureFormat::Red32f, TextureFormat::RG32f, TextureFormat::RGB32f,
				TextureFormat::RGBA32f, TextureFormat::R11G11B10f, TextureFormat::RGB9E5),
			TextureFormat>
			, f>
		{
			using Type = GLfloat;
		};

		template <TextureFormat f>
		struct _BaseType<
			std::enable_if_t<Empty::utils::isOneOf(f,
				TextureFormat::Red, TextureFormat::RG, TextureFormat::RGB, TextureFormat::RGBA,
				TextureFormat::DepthStencil, TextureFormat::Red8, TextureFormat::Red16, TextureFormat::RG8,
				TextureFormat::RG16, TextureFormat::RGB332, TextureFormat::RGB4, TextureFormat::RGB5,
				TextureFormat::RGB8, TextureFormat::RGB10, TextureFormat::RGB12, TextureFormat::RGBA2,
				TextureFormat::RGBA4, TextureFormat::RGB5A1, TextureFormat::RGBA8, TextureFormat::RGB10A2,
				TextureFormat::RGBA12, TextureFormat::RGBA16, TextureFormat::SRGB8, TextureFormat::SRGBA8,
				TextureFormat::RGB10A2ui, TextureFormat::Red8ui, TextureFormat::Red16ui, TextureFormat::Red32ui,
				TextureFormat::RG8ui, TextureFormat::RG16ui, TextureFormat::RG32ui, TextureFormat::RGB8ui,
				TextureFormat::RGB16ui, TextureFormat::RGB32ui, TextureFormat::RGBA8ui, TextureFormat::RGBA16ui,
				TextureFormat::RGBA32ui),
			TextureFormat>
			, f>
		{
			using Type = GLuint;
		};

		template <TextureFormat f>
		struct _BaseType<
			std::enable_if_t<Empty::utils::isOneOf(f,
				TextureFormat::Red8s, TextureFormat::Red16s, TextureFormat::RG8s, TextureFormat::RG16s,
				TextureFormat::RGB8s, TextureFormat::RGB16s, TextureFormat::RGBA8s,
				TextureFormat::Red8i, TextureFormat::Red16i, TextureFormat::Red32i, TextureFormat::RG8i,
				TextureFormat::RG16i, TextureFormat::RG32i, TextureFormat::RGB8i, TextureFormat::RGB16i,
				TextureFormat::RGB32i, TextureFormat::RGBA8i, TextureFormat::RGBA16i, TextureFormat::RGBA32i),
			TextureFormat>
			, f>
		{
			using Type = GLint;
		};
	}

	/**
	 * Get the base data type corresponding to an enum value.
	 * Example usage: `BaseType<DataType, DataType::UByte>`.
	 */
	template <typename T, T t>
	using BaseType = typename internal::_BaseType<T, t>::Type;

	/**
	 * Tells whether a state variable needs an index for its value to be queried.
	 * Some state variables can be read with and without an index parameter, yielding true for both
	 * `isStateVarIndexed` and `isStateVarNonIndexed`.
	 */
	constexpr bool isStateVarIndexed(ContextStateVar var)
	{
		return utils::isOneOf(var, ContextStateVar::DrawBuffer,
			ContextStateVar::MaxComputeWorkGroupCount, ContextStateVar::MaxComputeWorkGroupInvocations,
			ContextStateVar::MaxComputeWorkGroupSize, ContextStateVar::ShaderStorageBufferBinding,
			ContextStateVar::ShaderStorageBufferStart, ContextStateVar::ShaderStorageBufferSize,
			ContextStateVar::TransformFeedbackBufferBinding, ContextStateVar::TransformFeedbackBufferStart,
			ContextStateVar::TransformFeedbackBufferSize, ContextStateVar::UniformBufferBinding,
			ContextStateVar::UniformBufferSize, ContextStateVar::UniformBufferStart,
			ContextStateVar::VertexBindingDivisor, ContextStateVar::VertexBindingOffset,
			ContextStateVar::VertexBindingStride, ContextStateVar::VertexBindingBuffer,
			ContextStateVar::Viewport);
	}

	/**
	 * Tells whether a state variable needs an index for its value to be queried.
	 * Some state variables can be read with and without an index parameter, yielding true for both
	 * `isStateVarIndexed` and `isStateVarNonIndexed`.
	 */
	constexpr bool isStateVarNonIndexed(ContextStateVar var)
	{
		return !isStateVarIndexed(var) || utils::isOneOf(var, ContextStateVar::ShaderStorageBufferBinding,
			ContextStateVar::TransformFeedbackBufferBinding, ContextStateVar::UniformBufferBinding,
			ContextStateVar::Viewport);
	}

	/**
	 * Tells whether a state variable holds float value(s).
	 */
	constexpr bool isStateVarFloatValued(ContextStateVar var)
	{
		return utils::isOneOf(
			var, ContextStateVar::DepthClearValue,
			ContextStateVar::LineWidth, ContextStateVar::PointSize,
			ContextStateVar::PointSizeGranularity, ContextStateVar::PolygonOffsetFactor,
			ContextStateVar::PolygonOffsetUnits, ContextStateVar::SampleCoverageValue,
			ContextStateVar::SmoothLineWidthGranularity);
	}

	/**
	 * Tells whether a state variable holds pair(s) of float values.
	 */
	constexpr bool isStateVarVec2Valued(ContextStateVar var)
	{
		return utils::isOneOf(
			var, ContextStateVar::AliasedLineWidthRange,
			ContextStateVar::DepthRange, ContextStateVar::PointSizeRange,
			ContextStateVar::SmoothLineWidthRange
		);
	}

	/**
	 * Tells whether a state variable holds vec4 value(s).
	 */
	constexpr bool isStateVarVec4Valued(ContextStateVar var)
	{
		return utils::isOneOf(var, ContextStateVar::BlendColor, ContextStateVar::ColorClearValue);
	}

	/**
	 * Tells whether a state variable holds pair(s) of integer value(s).
	 */
	constexpr bool isStateVarIvec2Valued(ContextStateVar var)
	{
		return utils::isOneOf(var, ContextStateVar::MaxViewportDims, ContextStateVar::ViewportBoundsRange);
	}

	/**
	 * Tells whether a state variable holds vec4 value(s).
	 */
	constexpr bool isStateVarIvec4Valued(ContextStateVar var)
	{
		return utils::isOneOf(var, ContextStateVar::ScissorBox, ContextStateVar::Viewport);
	}

	/**
	 * Tells whether a state variable holds boolean value(s).
	 */
	constexpr bool isStateVarBoolValued(ContextStateVar var)
	{
		return utils::isOneOf(
			var, ContextStateVar::Blend,
			ContextStateVar::ColorLogicOp, ContextStateVar::CullFace,
			ContextStateVar::DepthTest, ContextStateVar::DepthWriteMask,
			ContextStateVar::Dither, ContextStateVar::DoubleBuffer,
			ContextStateVar::LineSmooth, ContextStateVar::PackLSBFirst,
			ContextStateVar::PackSwapBytes, ContextStateVar::ProgramPointSize,
			ContextStateVar::PolygonOffsetFill, ContextStateVar::PolygonOffsetLine,
			ContextStateVar::PolygonOffsetPoint, ContextStateVar::PolygonSmooth,
			ContextStateVar::SampleCoverageInvert, ContextStateVar::ScissorTest,
			ContextStateVar::ShaderCompiler, ContextStateVar::StencilTest,
			ContextStateVar::Stereo, ContextStateVar::UnpackLSBFirst,
			ContextStateVar::UnpackSwapBytes);
	}

	/**
	 * Tells whether a state variable holds integer value(s).
	 */
	constexpr bool isStateVarIntValued(ContextStateVar var)
	{
		return !isStateVarBoolValued(var) && !isStateVarFloatValued(var) && !isStateVarIvec2Valued(var)
			&& !isStateVarIvec4Valued(var) && !isStateVarVec2Valued(var) && !isStateVarVec4Valued(var);
	}

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
		else if (utils::isOneOf(target, TextureTarget::Texture1D,
			TextureTarget::Proxy1D, TextureTarget::TextureBuffer))
			return 1;
		else if (utils::isOneOf(target, TextureTarget::Texture3D,
			TextureTarget::Texture2DArray, TextureTarget::Texture2DMultisampleArray,
			TextureTarget::TextureCubemapArray, TextureTarget::Proxy3D,
			TextureTarget::Proxy2DArray, TextureTarget::Proxy2DMultisampleArray,
			TextureTarget::ProxyCubemapArray))
			return 3;
		else
			return 2;
	}

	/**
	 * Returns whether a given texture target references a cubemap or cubemap array texture target.
	 */
	constexpr bool isTargetCubemap(TextureTarget target)
	{
		return utils::isOneOf(target, TextureTarget::TextureCubemap, TextureTarget::ProxyCubemap,
			TextureTarget::TextureCubemapArray, TextureTarget::ProxyCubemapArray);
	}

	/**
	 * Returns whether a given texture target references a layered texture type.
	 */
	constexpr bool isTargetLayered(TextureTarget target)
	{
		return utils::isOneOf(target, TextureTarget::Texture1DArray, TextureTarget::Proxy1DArray)
			|| dimensionsFromTarget(target) == 3;
	}

	/**
	 * Returns whether a given texture target references a proxy target.
	 */
	constexpr bool isTargetProxy(TextureTarget target)
	{
		return utils::isOneOf(target, TextureTarget::Proxy1D, TextureTarget::Proxy1DArray,
			TextureTarget::Proxy2D, TextureTarget::Proxy2DArray,
			TextureTarget::Proxy2DMultisample, TextureTarget::Proxy2DMultisampleArray,
			TextureTarget::Proxy3D, TextureTarget::ProxyRectangle,
			TextureTarget::ProxyCubemap, TextureTarget::ProxyCubemapArray);
	}

	/**
	 * Returns whether a given texture target is a special texture target. Those include
	 * `TextureRectangle` and `TextureBuffer`, and behave differently from other textures
	 * in some cases.
	 */
	constexpr bool isTargetSpecial(TextureTarget target)
	{
		return utils::isOneOf(target, TextureTarget::TextureRectangle, TextureTarget::ProxyRectangle,
			TextureTarget::TextureBuffer);
	}

	/**
	 * Returns whether a given texture target is multisampled.
	 */
	constexpr bool isTargetMultisampled(TextureTarget target)
	{
		return utils::isOneOf(target, TextureTarget::Texture2DMultisample, TextureTarget::Proxy2DMultisample,
			TextureTarget::Texture2DMultisampleArray, TextureTarget::Proxy2DMultisampleArray);
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

	/**
	 * Returns the number of components in a texture format.
	 */
	constexpr int componentsInTextureFormat(TextureFormat format)
	{
		if (format == TextureFormat::Dynamic)
			return 0;
		else if (utils::isOneOf(format, TextureFormat::Red, TextureFormat::Depth,
			TextureFormat::Red, TextureFormat::Red8, TextureFormat::Red8s, TextureFormat::Red16,
			TextureFormat::Red16s, TextureFormat::Red16f, TextureFormat::Red32f, TextureFormat::Red8i,
			TextureFormat::Red8ui, TextureFormat::Red16i, TextureFormat::Red16ui, TextureFormat::Red32i,
			TextureFormat::Red32ui, TextureFormat::RedComp))
			return 1;
		else if (utils::isOneOf(format, TextureFormat::RG, TextureFormat::DepthStencil,
			TextureFormat::RG8, TextureFormat::RG8s, TextureFormat::RG16, TextureFormat::RG16s,
			TextureFormat::RG16f, TextureFormat::RG32f, TextureFormat::RG8i, TextureFormat::RG8ui,
			TextureFormat::RG16i, TextureFormat::RG16ui, TextureFormat::RG32i, TextureFormat::RG32ui))
			return 2;
		else if (utils::isOneOf(format, TextureFormat::RGBA, TextureFormat::RGBA2, TextureFormat::RGBA4,
			TextureFormat::RGB5A1, TextureFormat::RGBA8, TextureFormat::RGBA8s, TextureFormat::RGB10A2,
			TextureFormat::RGB10A2ui, TextureFormat::RGBA12, TextureFormat::RGBA16, TextureFormat::RGBA16f,
			TextureFormat::RGBA32f, TextureFormat::RGBA8i, TextureFormat::RGBA8ui, TextureFormat::RGBA16i,
			TextureFormat::RGBA16ui, TextureFormat::RGBA32i, TextureFormat::RGBA32ui, TextureFormat::RGBAComp,
			TextureFormat::SRGBAComp))
			return 4;
		else
			return 3;
	}
}
