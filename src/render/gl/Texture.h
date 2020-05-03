#pragma once

#include <type_traits>

#include <glad/glad.h>

#include "render/gl/GLObject.h"
#include "utils/macros.h"

namespace render::gl
{
	/**
	 * Valid targets that a texture can be bound to and adopt the type of.
	 * `Dynamic` is not a real target and is only present for templating purposes.
	 */
	enum struct TextureTarget : GLenum
	{
		Dynamic = GL_INVALID_ENUM,
		// Textures
		Texture1D = GL_TEXTURE_1D,
		Texture2D = GL_TEXTURE_2D,
		Texture3D = GL_TEXTURE_3D,
		Texture1DArray = GL_TEXTURE_1D_ARRAY,
		Texture2DArray = GL_TEXTURE_2D_ARRAY,
		TextureRectangle = GL_TEXTURE_RECTANGLE,
		TextureCubeMap = GL_TEXTURE_CUBE_MAP,
		TextureCubeMapArray = GL_TEXTURE_CUBE_MAP_ARRAY,
		Texture2DMultisample = GL_TEXTURE_2D_MULTISAMPLE,
		Texture2DMultisampleArray = GL_TEXTURE_2D_MULTISAMPLE_ARRAY,
		TextureBuffer = GL_TEXTURE_BUFFER,
		// Proxies
		Proxy1D = GL_PROXY_TEXTURE_1D,
		Proxy2D = GL_PROXY_TEXTURE_2D,
		Proxy3D = GL_PROXY_TEXTURE_3D,
		Proxy1DArray = GL_PROXY_TEXTURE_1D_ARRAY,
		Proxy2DArray = GL_PROXY_TEXTURE_2D_ARRAY,
		ProxyRectangle = GL_PROXY_TEXTURE_RECTANGLE,
		ProxyCubeMap = GL_PROXY_TEXTURE_CUBE_MAP,
		ProxyCubeMapArray = GL_PROXY_TEXTURE_CUBE_MAP_ARRAY,
		Proxy2DMultisample = GL_PROXY_TEXTURE_2D_MULTISAMPLE,
		Proxy2DMultisampleArray = GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY,
	};

	/**
	 * Internal formats that a texture can adopt. This is different
	 * from the format of the pixel data that is fed to it.
	 * `Dynamic` is not a real format and is only present for templating purposes.
	 */
	enum struct TextureFormat : GLenum
	{
		Dynamic = GL_INVALID_ENUM,
		// Base internal formats
		Red = GL_RED,
		RG = GL_RG,
		RGB = GL_RGB,
		RGBA = GL_RGBA,
		Depth = GL_DEPTH_COMPONENT,
		DepthStencil = GL_DEPTH_STENCIL,
		// Sized internal formats
		Red8 = GL_R8,
		Red8s = GL_R8_SNORM,
		Red16 = GL_R16,
		Red16s = GL_R16_SNORM,
		RG8 = GL_RG8,
		RG8s = GL_RG8_SNORM,
		RG16 = GL_RG16,
		RG16s = GL_RG16_SNORM,
		RGB332 = GL_R3_G3_B2,
		RGB4 = GL_RGB4,
		RGB5 = GL_RGB5,
		RGB8 = GL_RGB8,
		RGB8s = GL_RGB8_SNORM,
		RGB10 = GL_RGB10,
		RGB12 = GL_RGB12,
		RGB16s = GL_RGB16_SNORM,
		RGBA2 = GL_RGBA2,
		RGBA4 = GL_RGBA4,
		RGB5A1 = GL_RGB5_A1,
		RGBA8 = GL_RGBA8,
		RGBA8s = GL_RGBA8_SNORM,
		RGB10A2 = GL_RGB10_A2,
		RGB10A2ui = GL_RGB10_A2UI,
		RGBA12 = GL_RGBA12,
		RGBA16 = GL_RGBA16,
		SRGB8 = GL_SRGB8,
		SRGBA8 = GL_SRGB8_ALPHA8,
		Red16f = GL_R16F,
		RG16f = GL_RG16F,
		RGB16f = GL_RGB16F,
		RGBA16f = GL_RGBA16F,
		Red32f = GL_R32F,
		RG32f = GL_RG32F,
		RGB32f = GL_RGB32F,
		RGBA32f = GL_RGBA32F,
		R11G11B10f = GL_R11F_G11F_B10F,
		RGB9E5 = GL_RGB9_E5,
		Red8i = GL_R8I,
		Red8ui = GL_R8UI,
		Red16i = GL_R16I,
		Red16ui = GL_R16UI,
		Red32i = GL_R32I,
		Red32ui = GL_R32UI,
		RG8i = GL_RG8I,
		RG8ui = GL_RG8UI,
		RG16i = GL_RG16I,
		RG16ui = GL_RG16UI,
		RG32i = GL_RG32I,
		RG32ui = GL_RG32UI,
		RGB8i = GL_RGB8I,
		RGB8ui = GL_RGB8UI,
		RGB16i = GL_RGB16I,
		RGB16ui = GL_RGB16UI,
		RGB32i = GL_RGB32I,
		RGB32ui = GL_RGB32UI,
		RGBA8i = GL_RGBA8I,
		RGBA8ui = GL_RGBA8UI,
		RGBA16i = GL_RGBA16I,
		RGBA16ui = GL_RGBA16UI,
		RGBA32i = GL_RGBA32I,
		RGBA32ui = GL_RGBA32UI,
		// Compressed internal formats
		RedComp = GL_COMPRESSED_RED,
		RGComp = GL_COMPRESSED_RG,
		RGBComp = GL_COMPRESSED_RGB,
		RGBAComp = GL_COMPRESSED_RGBA,
		SRGBComp = GL_COMPRESSED_SRGB,
		SRGBAComp = GL_COMPRESSED_SRGB_ALPHA,
	};

	/**
	 * Represents each of the six faces of a cube map.
	 */
	enum struct CubeMapFace : GLenum
	{
		PlusX = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		MinusX = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		PlusY = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		MinusY = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		PlusZ = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		MinusZ = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
	};

	/**
	 * Format of pixel data that is fed to a texture. Different from the
	 * texture's own internal format. This is used in conjunction with
	 * the `PixelType` enum and serves to identify what color components
	 * are to be used and their meaning.
	 */
	enum struct PixelFormat : GLenum
	{
		Red = GL_RED,
		RG = GL_RG,
		RGB = GL_RGB,
		BGR = GL_BGR,
		RGBA = GL_RGBA,
		BGRA = GL_BGRA,
		RedInt = GL_RED_INTEGER,
		RGInt = GL_RG_INTEGER,
		RGBInt = GL_RGB_INTEGER,
		BGRInt = GL_BGR_INTEGER,
		RGBAInt = GL_RGBA_INTEGER,
		BGRAInt = GL_BGRA_INTEGER,
		Stencil = GL_STENCIL_INDEX,
		Depth = GL_DEPTH_COMPONENT,
		DepthStencil = GL_DEPTH_STENCIL,
	};

	/**
	 * Type of pixel data that is fed to a texture. Different from the
	 * texture's own internal format. This is used in conjunction with
	 * the `PixelFormat` enum and serves to identify the data type and
	 * size of the color components.
	 */
	enum struct PixelType : GLenum
	{
		UByte = GL_UNSIGNED_BYTE,
		Byte = GL_BYTE,
		UShort = GL_UNSIGNED_SHORT,
		Short = GL_SHORT,
		UInt = GL_UNSIGNED_INT,
		Int = GL_INT,
		Half = GL_HALF_FLOAT,
		Single = GL_FLOAT,
		UByte332 = GL_UNSIGNED_BYTE_3_3_2,
		UByte332Rev = GL_UNSIGNED_BYTE_2_3_3_REV,
		UShort565 = GL_UNSIGNED_SHORT_5_6_5,
		UShort565Rev = GL_UNSIGNED_SHORT_5_6_5_REV,
		UShort4 = GL_UNSIGNED_SHORT_4_4_4_4,
		UShort4Rev = GL_UNSIGNED_SHORT_4_4_4_4_REV,
		UShort5551 = GL_UNSIGNED_SHORT_5_5_5_1,
		UShort5551Rev = GL_UNSIGNED_SHORT_1_5_5_5_REV,
		UInt8 = GL_UNSIGNED_INT_8_8_8_8,
		UInt8Rev = GL_UNSIGNED_INT_8_8_8_8_REV,
		UIntAAA2 = GL_UNSIGNED_INT_10_10_10_2,
		UIntAAA2Rev = GL_UNSIGNED_INT_2_10_10_10_REV,
	};

	/**
	 * Names for a texture's internal parameters.
	 */
	enum struct TextureParam : GLenum
	{
		DepthStencilMode = GL_DEPTH_STENCIL_TEXTURE_MODE,
		BaseLevel = GL_TEXTURE_BASE_LEVEL,
		BorderColor = GL_TEXTURE_BORDER_COLOR,
		CompareFunc = GL_TEXTURE_COMPARE_FUNC,
		CompareMode = GL_TEXTURE_COMPARE_MODE,
		LODBias = GL_TEXTURE_LOD_BIAS,
		MinFilter = GL_TEXTURE_MIN_FILTER,
		MagFilter = GL_TEXTURE_MAG_FILTER,
		MinLOD = GL_TEXTURE_MIN_LOD,
		MaxLOD = GL_TEXTURE_MAX_LOD,
		MaxLevel = GL_TEXTURE_MAX_LEVEL,
		SwizzleR = GL_TEXTURE_SWIZZLE_R,
		SwizzleG = GL_TEXTURE_SWIZZLE_G,
		SwizzleB = GL_TEXTURE_SWIZZLE_B,
		SwizzleA = GL_TEXTURE_SWIZZLE_A,
		SwizzleRGBA = GL_TEXTURE_SWIZZLE_RGBA,
		WrapS = GL_TEXTURE_WRAP_S,
		WrapT = GL_TEXTURE_WRAP_T,
		WrapR = GL_TEXTURE_WRAP_R,
		// Read-only parameters
		ViewImmutableLevels = GL_TEXTURE_IMMUTABLE_LEVELS,
		ViewMinLayer = GL_TEXTURE_VIEW_MIN_LAYER,
		ViewNumLayers = GL_TEXTURE_VIEW_NUM_LAYERS,
		ViewMinLevel = GL_TEXTURE_VIEW_MIN_LEVEL,
		ViewNumLevels = GL_TEXTURE_VIEW_NUM_LEVELS,
		ImageFormatCompatibilityType = GL_IMAGE_FORMAT_COMPATIBILITY_TYPE,
		HasImmutableFormat = GL_TEXTURE_IMMUTABLE_FORMAT,
	};

	/**
	 * Possible values of texture parameters that accept symbolic constants as values.
	 */
	enum struct TextureParamValue : GLenum
	{
		Invalid = GL_INVALID_ENUM,
		// DepthStencilMode
		DepthMode = GL_DEPTH_COMPONENT,
		StencilMode = GL_STENCIL_INDEX,
		// CompareFunc
		FuncLEqual = GL_LEQUAL,
		FuncGEqual = GL_GEQUAL,
		FuncLess = GL_LESS,
		FuncGreater = GL_GREATER,
		FuncEqual = GL_EQUAL,
		FuncNotEqual = GL_NOTEQUAL,
		FuncAlways = GL_ALWAYS,
		FuncNever = GL_NEVER,
		// CompareMode
		CompareRefToTexture = GL_COMPARE_REF_TO_TEXTURE,
		NoCompare = GL_NONE,
		// Min & MagFilter
		FilterNearest = GL_NEAREST,
		FilterLinear = GL_LINEAR,
		FilterNearestMipmapNearest = GL_NEAREST_MIPMAP_NEAREST,
		FilterNearestMipmapLinear = GL_NEAREST_MIPMAP_LINEAR,
		FilterLinearMipmapNearest = GL_LINEAR_MIPMAP_NEAREST,
		FilterLinearMipmapLinear = GL_LINEAR_MIPMAP_LINEAR,
		// Swizzling
		SwizzleRed = GL_RED,
		SwizzleGreen = GL_GREEN,
		SwizzleBlue = GL_BLUE,
		SwizzleAlpha = GL_ALPHA,
		SwizzleZero = GL_ZERO,
		SwizzleOne = GL_ONE,
		// Wrapping
		ClampToEdge = GL_CLAMP_TO_EDGE,
		ClampToBorder = GL_CLAMP_TO_BORDER,
		MirrorRepeat = GL_MIRRORED_REPEAT,
		Repeat = GL_REPEAT,
		MirrorClampToEdge = GL_MIRROR_CLAMP_TO_EDGE,
	};

	/**
	 * This struct holds everything one needs to bind a texture. Useful in case
	 * one wants to bind the texture without having access to the entire Texture object.
	 */
	struct TextureBinding
	{
		const TextureTarget target = TextureTarget::Dynamic;
		const std::shared_ptr<TextureId> id;
		// Texture unit to which the texture is to be bound. -1 means whatever
		// the current active unit is.
		const int unit = -1;

		inline void bind() const
		{
			ASSERT(target != TextureTarget::Dynamic && id && "Invalid texture binding");
			if (unit >= 0)
				glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(utils::value(target), *id);
		}
		inline void unbind() const
		{
			ASSERT(target != TextureTarget::Dynamic && "Invalid texture binding");
			if (unit >= 0)
				glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(utils::value(target), 0);
		}
	};

	/**
	 * General-purpose texture class. Does not map one-to-one with OpenGL texture objects, but holds a shared reference
	 * to one. It is possible to instantiate textures with targets and formats both known and unknown at compile-time.
	 */
	template <TextureTarget CTTarget = TextureTarget::Dynamic, TextureFormat CTFormat = TextureFormat::Dynamic>
	struct Texture : public GLObject<TextureId>
	{
		template <TextureTarget t = CTTarget, TextureFormat f = CTFormat,
			std::enable_if_t<t == TextureTarget::Dynamic && f == TextureFormat::Dynamic, int> = 0>
			Texture(TextureTarget target, TextureFormat format) : _target(target), _format(format) {}

		template <TextureTarget t = CTTarget, TextureFormat f = CTFormat,
			std::enable_if_t<t != TextureTarget::Dynamic && f == TextureFormat::Dynamic, int> = 0>
			Texture(TextureFormat format) : _target(t), _format(format) {}

		template <TextureTarget t = CTTarget, TextureFormat f = CTFormat,
			std::enable_if_t<t == TextureTarget::Dynamic && f != TextureFormat::Dynamic, int> = 0>
			Texture(TextureTarget target) : _target(target), _format(f) {}

		template <TextureTarget t = CTTarget, TextureFormat f = CTFormat,
			std::enable_if_t<t != TextureTarget::Dynamic && f != TextureFormat::Dynamic, int> = 0>
			Texture() : _target(t), _format(f) {}

		/**
		 * Binds the texture to a given texture unit. This is necessary before
		 * any operation on the texture is performed. If no texture unit is given,
		 * whichever one is currently active is used.
		 */
		inline void bind(int unit = -1) const
		{
			if (unit >= 0)
				glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(utils::value(_target), *_id);
		}
		
		/**
		 * Unbinds the texture, making sure it will not be modified by any further operation.
		 */
		inline void unbind(int unit = -1) const
		{
			if (unit >= 0)
				glActiveTexture(GL_TEXTURE0 + unit);
			glBindTexture(utils::value(_target), 0);
		}

		void uploadData(int level, int width, PixelFormat format, PixelType type, const void* data) const
		{
			ASSERT(dimensionsFromTarget(_target) == 1 && "cannot upload 1-dimensional data to a non-1-dimensional texture");

			glTexImage1D(utils::value(_target), level, utils::value(_format), width, 0, utils::value(format), utils::value(type), data);
		}

		void uploadData(int level, int width, int height, PixelFormat format, PixelType type, const void* data) const
		{
			ASSERT(_target != TextureTarget::TextureCubeMap && "cannot upload 2-dimensional data directly to cube map ; upload to faces instead");
			ASSERT(dimensionsFromTarget(_target) == 2 && "cannot upload 2-dimensional data to a non-2-dimensional texture");

			glTexImage2D(utils::value(_target), level, utils::value(_format), width, height, 0, utils::value(format), utils::value(type), data);
		}

		void uploadData(int level, int width, int height, int depth, PixelFormat format, PixelType type, const void* data) const
		{
			ASSERT(dimensionsFromTarget(_target) == 3 && "cannot upload 3-dimensional data to a non-3-dimensional texture");

			glTexImage3D(utils::value(_target), level, utils::value(_format), width, height, depth, 0, utils::value(format), utils::value(type), data);
		}

		void uploadData(CubeMapFace face, int level, int width, int height, PixelFormat format, PixelType type, const void* data) const
		{
			ASSERT(_target == TextureTarget::TextureCubeMap);

			glTexImage2D(utils::value(face), level, utils::value(_format), width, height, 0, utils::value(format), utils::value(type), data);
		}


		void uploadSubData(int level, int xoffset, int width, PixelFormat format, PixelType type, const void* data) const;
		void uploadSubData(int level, int xoffset, int yoffset, int width, int height, PixelFormat format, PixelType type, const void* data) const;
		void uploadSubData(CubeMapFace face, int level, int xoffset, int yoffset, int width, int height, PixelFormat format, PixelType type, const void* data) const;
		void uploadSubData(int level, int xoffset, int yoffset, int zoffset, int width, int height, int depth, PixelFormat format, PixelType type, const void* data) const;

		/**
		 * Gets value of integer-valued texture parameters.
		 */
		template <TextureParam CTParam, std::enable_if_t<
			CTParam == TextureParam::BaseLevel || CTParam == TextureParam::MaxLevel
			|| CTParam == TextureParam::ViewMinLayer || CTParam == TextureParam::ViewNumLayers
			|| CTParam == TextureParam::ViewMinLevel || CTParam == TextureParam::ViewNumLevels
			|| CTParam == TextureParam::ViewImmutableLevels || CTParam == TextureParam::HasImmutableFormat
			, int> = 0>
			int getParameter() const
		{
			int p;
			glGetTexParameteriv(utils::value(_target), utils::value(CTParam), &p);
			return p;
		}

		/**
		 * Gets value of symbolic-valued texture parameter.
		 */
		template <TextureParam CTParam, std::enable_if_t<
			CTParam == TextureParam::DepthStencilMode
			|| CTParam == TextureParam::CompareFunc || CTParam == TextureParam::CompareMode
			|| CTParam == TextureParam::MinFilter || CTParam == TextureParam::MagFilter
			|| CTParam == TextureParam::SwizzleR || CTParam == TextureParam::SwizzleG
			|| CTParam == TextureParam::SwizzleB || CTParam == TextureParam::SwizzleA
			|| CTParam == TextureParam::WrapS || CTParam == TextureParam::WrapT
			|| CTParam == TextureParam::WrapR || CTParam == TextureParam::ImageFormatCompatibilityType
			, int> = 0>
			TextureParamValue getParameter() const
		{
			int p;
			glGetTexParameteriv(utils::value(_target), utils::value(CTParam), &p);
			return static_cast<TextureParamValue>(p);
		}

		/**
		 * Gets value of float-valued texture parameter.
		 */
		template <TextureParam CTParam, std::enable_if_t<
			CTParam == TextureParam::LODBias
			|| CTParam == TextureParam::MinLOD || CTParam == TextureParam::MaxLOD
			, int> = 0>
			float getParameter() const
		{
			float v;
			glGetTexParameterfv(utils::value(_target), utils::value(CTParam), &v);
			return v;
		}

		/**
		 * Sets integer-valued texture parameter.
		 */
		template <TextureParam CTParam, std::enable_if_t<
			CTParam == TextureParam::BaseLevel || CTParam == TextureParam::MaxLevel
			, int> = 0>
			void setParameter(int v) const { glTexParameteri(utils::value(_target), utils::value(CTParam), v); }

		/**
		 * Sets float-valued texture parameter.
		 */
		template <TextureParam CTParam, std::enable_if_t<
			CTParam == TextureParam::LODBias
			|| CTParam == TextureParam::MinLOD || CTParam == TextureParam::MaxLOD
			, int> = 0>
			void setParameter(float v) const { glTexParameterf(utils::value(_target), utils::value(CTParam), v); }

		/**
		 * Sets symbolic-valued texture parameter.
		 */
		template <TextureParam CTParam, std::enable_if_t<
			CTParam == TextureParam::DepthStencilMode
			|| CTParam == TextureParam::CompareFunc || CTParam == TextureParam::CompareMode
			|| CTParam == TextureParam::MinFilter || CTParam == TextureParam::MagFilter
			|| CTParam == TextureParam::SwizzleR || CTParam == TextureParam::SwizzleG
			|| CTParam == TextureParam::SwizzleB || CTParam == TextureParam::SwizzleA
			|| CTParam == TextureParam::WrapS || CTParam == TextureParam::WrapT
			|| CTParam == TextureParam::WrapR
			, int> = 0>
			void setParameter(TextureParamValue v) const { glTexParameteri(utils::value(_target), utils::value(CTParam), utils::value(v)); }

		TextureBinding getBindingInfo() const { return TextureBinding{_target, _id}; }
		TextureTarget target() const { return _target; }
		TextureFormat format() const { return _format; }

	protected:
		/**
		 * Registered target of the texture object.
		 */
		const TextureTarget _target;
		/**
		 * Registered internal format of the texture object.
		 */
		const TextureFormat _format;
	};
}