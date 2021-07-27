#pragma once

#include <type_traits>

#include "glad/glad.h"
#include "Empty/render/gl/GLEnums.hpp"
#include "Empty/render/gl/GLObject.h"
#include "Empty/utils/macros.h"

namespace render::gl
{
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

	/**
	 * This struct holds everything one needs to bind a Texture. Useful in case
	 * one wants to bind the Texture without having access to the entire object.
	 */
	struct TextureInfo
	{
		std::shared_ptr<TextureId> id;
		TextureTarget target;
	};

	/**
	 * General-purpose texture class. Does not map one-to-one with OpenGL texture objects, but holds a shared reference
	 * to one. It is possible to instantiate textures with targets and formats both known and unknown at compile-time.
	 * All textures are of immutable format ; while their content may be changed at will, their dimension and storage
	 * requirements may only be set once.
	 * 
	 * The class also exposes methods to generate texture views over textures or other texture views. Texture views
	 * are identical to texture objects in all aspects, with the one quirk that instead of having their own data store,
	 * they use that of an existing texture object, possibly reinterpreted in a different format (no conversion happens).
	 * As such, their memory usage is virtually zero, and they allow for the isolation of parts of a texture object's
	 * data store, like specific mipmap levels, layers of an array or faces of a cubemap.
	 * Overloads of the single `makeView` method ensures that only valid combinations of targets are usable.
	 */
	template <TextureTarget CTTarget = TextureTarget::Dynamic, TextureFormat CTFormat = TextureFormat::Dynamic>
	struct Texture : public GLObject<TextureId>
	{
#define COPY_CTPARAMS TextureTarget t = CTTarget, TextureFormat f = CTFormat

		template <COPY_CTPARAMS,
			std::enable_if_t<t == TextureTarget::Dynamic && f == TextureFormat::Dynamic, int> = 0>
			Texture(TextureTarget target, TextureFormat format) : GLObject(target), _target(target), _format(format) {}

		template <COPY_CTPARAMS,
			std::enable_if_t<t != TextureTarget::Dynamic && f == TextureFormat::Dynamic, int> = 0>
			Texture(TextureFormat format) : GLObject(t), _target(t), _format(format) {}

		template <COPY_CTPARAMS,
			std::enable_if_t<t == TextureTarget::Dynamic && f != TextureFormat::Dynamic, int> = 0>
			Texture(TextureTarget target) : GLObject(target), _target(target), _format(f) {}

		template <COPY_CTPARAMS,
			std::enable_if_t<t != TextureTarget::Dynamic && f != TextureFormat::Dynamic, int> = 0>
			Texture() : GLObject(t), _target(t), _format(f) {}

#undef COPY_CTPARAMS

#define COPY_CTPARAMS TextureTarget t = CTTarget

		/**
		 * Allocate storage for a 1-dimensional texture.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<dimensionsFromTarget(t) == 1, int> = 0>
			inline void setStorage(int levels, int width)
		{
			ASSERT(!_requirementsSet && "Storage requirements were already set for this Texture");
			glTextureStorage1D(*_id, levels, utils::value(_format), width);
			DEBUG_ONLY(_requirementsSet = true);
		}

		/**
		 * Allocate storage for a 2-dimensional texture. For 1D array textures, `height` is the number
		 * of layers.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<dimensionsFromTarget(t) == 2, int> = 0>
			inline void setStorage(int levels, int width, int height)
		{
			ASSERT(!_requirementsSet && "Storage requirements were already set for this Texture");
			glTextureStorage2D(*_id, levels, utils::value(_format), width, height);
			DEBUG_ONLY(_requirementsSet = true);
		}

		/**
		 * Allocate storage for a 3-dimensional texture. For array textures, `depth` is the number of
		 * layers. For cubemap array textures, `depth` is also the number of layers and not layer-faces
		 * (no need to multiply it by 6).
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<dimensionsFromTarget(t) == 3, int> = 0>
			inline void setStorage(int levels, int width, int height, int depth)
		{
			ASSERT(!_requirementsSet && "Storage requirements were already set for this Texture");
			if constexpr (isTargetCubemap(t))
				depth *= 6;
			glTextureStorage3D(*_id, levels, utils::value(_format), width, height, depth);
			DEBUG_ONLY(_requirementsSet = true);
		}

		/**
		 * Allocate storage for a texture with target unknown at compile-time. Parameters
		 * that are not relevant to the actual texture target are ignored. For cubemap array
		 * textures, `depth` is the number of layers and not layer-faces (no need to
		 * multiply it by 6).
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<t == TextureTarget::Dynamic, int> = 0>
			void setStorage(int levels, int width, int height = 1, int depth = 1)
		{
			ASSERT(!_requirementsSet && "Storage requirements were already set for this Texture");

			int dims = dimensionsFromTarget(_target);
			if (dims == 1)
				glTextureStorage1D(*_id, levels, utils::value(_format), width);
			else if (dims == 2)
				glTextureStorage2D(*_id, levels, utils::value(_format), width, height);
			else // if(dims == 3)
				glTextureStorage3D(*_id, levels, utils::value(_format), width, height, isTargetCubemap(_target) ? depth * 6 : depth);

			DEBUG_ONLY(_requirementsSet = true);
		}

		/**
		 * Upload data to a 1-dimensional texture.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<dimensionsFromTarget(t) == 1, int> = 0>
			inline void uploadData(int level, int x, int w, PixelFormat format, PixelType type, const void* data) const
		{
			ASSERT(_requirementsSet);
			glTextureSubImage1D(*_id, level, x, w, utils::value(format), utils::value(type), data);
		}

		/**
		 * Upload data to a 2-dimensional texture that is not a cubemap.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<dimensionsFromTarget(t) == 2 && !isTargetCubemap(t), int> = 0>
			inline void uploadData(int level, int x, int y, int w, int h, PixelFormat format, PixelType type, const void* data) const
		{
			ASSERT(_requirementsSet);
			glTextureSubImage2D(*_id, level, x, y, w, h, utils::value(format), utils::value(type), data);
		}

		/**
		 * Upload data to a 3-dimensional texture that is not a cubemap array.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<dimensionsFromTarget(t) == 3 && !isTargetCubemap(t), int> = 0>
			inline void uploadData(int level, int x, int y, int z, int w, int h, int d, PixelFormat format, PixelType type, const void* data) const
		{
			ASSERT(_requirementsSet);
			glTextureSubImage3D(*_id, level, x, y, z, w, h, d, utils::value(format), utils::value(type), data);
		}

		/**
		 * Upload data to a cubemap texture. `face` is the starting face of the upload operation,
		 * and `faces` how many should be filled.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<dimensionsFromTarget(t) == 2 && isTargetCubemap(t), int> = 0>
			inline void uploadData(CubemapFace face, int level, int x, int y, int w, int h, int faces, PixelFormat format, PixelType type, const void* data) const
		{
			ASSERT(_requirementsSet);
			glTextureSubImage3D(*_id, level, x, y, cubemapFaceIndex(face), w, h, faces, utils::value(format), utils::value(type), data);
		}

		/**
		 * Upload data to a cubemap array texture. `face` and `layer` are the starting face and layer
		 * of the upload operation, and `faces` is how many faces should be filled. If the number of
		 * faces overflows the number of faces that a cubemap has, the filling operation continues
		 * on the cubemap of the next layer. For example, using `CubeMapFace::PositiveX`, `0` and `12`
		 * respectively will fill all 6 faces of the first two cubemaps.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<dimensionsFromTarget(t) == 3 && isTargetCubemap(t), int> = 0>
			inline void uploadData(CubemapFace face, int level, int x, int y, int layer, int w, int h, int faces, PixelFormat format, PixelType type, const void* data) const
		{
			ASSERT(_requirementsSet);
			glTextureSubImage3D(*_id, level, x, y, layer * 6 + cubemapFaceIndex(face), w, h, faces, utils::value(format), utils::value(type), data);
		}

		/**
		 * Upload data to a non-cubemap texture with target unknown at compile-time. Parameters
		 * that are not relevant to the actual texture target are ignored.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<t == TextureTarget::Dynamic, int> = 0>
			void uploadData(int level, int x, int y, int z, int w, int h, int d, PixelFormat format, PixelType type, const void* data) const
		{
			ASSERT(_requirementsSet);
			ASSERT(!isTargetCubemap(_target) && "cannot upload data directly to a cubemap ; use dedicated method instead");

			int dims = dimensionsFromTarget(_target);
			if (dims == 1)
				glTextureSubImage1D(*_id, level, x, w, utils::value(format), utils::value(type), data);
			else if (dims == 2)
				glTextureSubImage2D(*_id, level, x, y, w, h, utils::value(format), utils::value(type), data);
			else // if(dims == 3)
				glTextureSubImage3D(*_id, level, x, y, z, w, h, d, utils::value(format), utils::value(type), data);
		}

		/**
		 * Upload data to a cubemap or cubemap array texture with target unknown at compile-time. Parameters
		 * that are not relevant to the actual texture target are ignored.
		 */
		template <COPY_CTPARAMS,
			std::enable_if_t<t == TextureTarget::Dynamic, int> = 0>
			inline void uploadData(CubemapFace face, int level, int x, int y, int layer, int w, int h, int faces, PixelFormat format, PixelType type, const void* data) const
		{
			ASSERT(_requirementsSet);
			ASSERT(isTargetCubemap(_target) && "can only upload data to a cubemap ; use general-purpose method instead");

			if (dimensionsFromTarget(_target) == 2)
				glTextureSubImage3D(*_id, level, x, y, cubemapFaceIndex(face), w, h, faces, utils::value(format), utils::value(type), data);
			else // if (dimensionsFromTarget(_target) == 3)
				glTextureSubImage3D(*_id, level, x, y, layer * 6 + cubemapFaceIndex(face), w, h, faces, utils::value(format), utils::value(type), data);
		}

#undef COPY_CTPARAMS

		/**
		 * Creates a rectangle texture view over a rectangle texture.
		 */
		template <TextureTarget CTNewTarget, TextureFormat CTNewFormat,
			std::enable_if_t<
				CTNewTarget == CTTarget && CTNewTarget == TextureTarget::TextureRectangle,
				int> = 0
		>
		auto makeView()
		{
			return Texture<CTNewTarget, CTNewFormat>(*this, 0, 1, 0, 1);
		}

#define COPY_CTPARAMS(c) TextureTarget CTNewTarget, TextureFormat CTNewFormat, \
			std::enable_if_t<!isTargetSpecial(CTNewTarget) && !isTargetSpecial(CTTarget) && !isTargetProxy(CTNewTarget) && !isTargetProxy(CTTarget) && (c), int> = 0

		/**
		 * Creates a texture view (layered or not) over a non-layered texture.
		 */
		template <COPY_CTPARAMS(CTNewTarget == CTTarget && !isTargetLayered(CTNewTarget) || isTargetArrayOf(CTNewTarget, CTTarget))>
		auto makeView(int minLevel, int numLevels)
		{
			if constexpr (CTTarget == TextureTarget::TextureCubemap)
				return Texture<CTNewTarget, CTNewFormat>(*this, minLevel, numLevels, 0, 6);
			// else
			return Texture<CTNewTarget, CTNewFormat>(*this, minLevel, numLevels, 0, 1);
		}

		/**
		 * Creates a non-layered texture view over a layered texture. For cubemap
		 * arrays, a single "layer" is an entire cubemap, not a layer-face.
		 */
		template <COPY_CTPARAMS(isTargetArrayOf(CTTarget, CTNewTarget))>
		auto makeView(int minLevel, int numLevels, int layer)
		{
			if constexpr (CTTarget == TextureTarget::TextureCubemapArray)
				return Texture<CTNewTarget, CTNewFormat>(*this, minLevel, numLevels, layer * 6, 6);
			// else
			return Texture<CTNewTarget, CTNewFormat>(*this, minLevel, numLevels, layer, 1);
		}

		/**
		 * Creates a layered texture view over a layered texture. For cubemap array
		 * views over cubemap arrays, a single "layer" is an entire cubemap, not a
		 * layer-face. In all other cases, a single "layer" is a layer-face.
		 */
		template <COPY_CTPARAMS(CTNewTarget == CTTarget && isTargetLayered(CTNewTarget) || CTNewTarget == TextureTarget::Texture2DArray && CTTarget == TextureTarget::TextureCubemapArray)>
		auto makeView(int minLevel, int numLevels, int minLayer, int numLayers)
		{
			if constexpr (CTNewTarget == TextureTarget::TextureCubemapArray)
				return Texture<CTNewTarget, CTNewFormat>(*this, minLevel, numLevels, minLayer * 6, numLayers * 6);
			// else
			return Texture<CTNewTarget, CTNewFormat>(*this, minLevel, numLevels, minLayer, numLayers);
		}

		/**
		 * Creates a 2D texture view over a cubemap.
		 */
		template <COPY_CTPARAMS(CTNewTarget == TextureTarget::Texture2D && CTTarget == TextureTarget::TextureCubemap)>
		auto makeView(int minLevel, int numLevels, CubemapFace face)
		{
			return Texture<CTNewTarget, CTNewFormat>(*this, minLevel, numLevels, cubemapFaceIndex(face), 1);
		}

		/**
		 * Creates a 2D texture view over a cubemap array. A single "layer" is an
		 * entire cubemap.
		 */
		template <COPY_CTPARAMS(CTNewTarget == TextureTarget::Texture2D && CTTarget == TextureTarget::TextureCubemapArray)>
		auto makeView(int minLevel, int numLevels, int layer, CubemapFace face)
		{
			return Texture<CTNewTarget, CTNewFormat>(*this, minLevel, numLevels, layer * 6 + cubemapFaceIndex(face), 1);
		}

		/**
		 * Creates a 2D array texture view over a cubemap.
		 */
		template <COPY_CTPARAMS(CTNewTarget == TextureTarget::Texture2DArray && CTTarget == TextureTarget::TextureCubemap)>
		auto makeView(int minLevel, int numLevels, CubemapFace startFace, int faces)
		{
			return Texture<CTNewTarget, CTNewFormat>(*this, minLevel, numLevels, cubemapFaceIndex(startFace), faces);
		}
#undef COPY_CTPARAMS

		/**
		 * Populate the texture's levels with mipmaps. Only usable on texture targets that do have mipmaps.
		 */
		template <std::enable_if_t<!isTargetSpecial(CTTarget) && !isTargetMultisampled(CTTarget) && !isTargetProxy(CTTarget), int> = 0>
		void generateMipmaps()
		{
			glGenerateTextureMipmap(*_id);
		}

		template <TextureParam CTParam, std::enable_if_t<
			CTParam == TextureParam::BaseLevel || CTParam == TextureParam::MaxLevel
			|| CTParam == TextureParam::ViewMinLayer || CTParam == TextureParam::ViewNumLayers
			|| CTParam == TextureParam::ViewMinLevel || CTParam == TextureParam::ViewNumLevels
			|| CTParam == TextureParam::ViewImmutableLevels || CTParam == TextureParam::HasImmutableFormat
			, int> = 0>
			int getParameter() const
		{
			int p;
			glGetTextureParameteriv(*_id, utils::value(CTParam), &p);
			return p;
		}

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
			glGetTextureParameteriv(*_id, utils::value(CTParam), &p);
			return static_cast<TextureParamValue>(p);
		}

		template <TextureParam CTParam, std::enable_if_t<
			CTParam == TextureParam::LODBias
			|| CTParam == TextureParam::MinLOD || CTParam == TextureParam::MaxLOD
			, int> = 0>
			float getParameter() const
		{
			float v;
			glGetTextureParameterfv(*_id, utils::value(CTParam), &v);
			return v;
		}

		template <TextureParam CTParam, std::enable_if_t<
			CTParam == TextureParam::BaseLevel || CTParam == TextureParam::MaxLevel
			, int> = 0>
			void setParameter(int v) const { glTextureParameteri(*_id, utils::value(CTParam), v); }

		template <TextureParam CTParam, std::enable_if_t<
			CTParam == TextureParam::LODBias
			|| CTParam == TextureParam::MinLOD || CTParam == TextureParam::MaxLOD
			, int> = 0>
			void setParameter(float v) const { glTextureParameterf(*_id, utils::value(CTParam), v); }

		template <TextureParam CTParam, std::enable_if_t<
			CTParam == TextureParam::DepthStencilMode
			|| CTParam == TextureParam::CompareFunc || CTParam == TextureParam::CompareMode
			|| CTParam == TextureParam::MinFilter || CTParam == TextureParam::MagFilter
			|| CTParam == TextureParam::SwizzleR || CTParam == TextureParam::SwizzleG
			|| CTParam == TextureParam::SwizzleB || CTParam == TextureParam::SwizzleA
			|| CTParam == TextureParam::WrapS || CTParam == TextureParam::WrapT
			|| CTParam == TextureParam::WrapR
			, int> = 0>
			void setParameter(TextureParamValue v) const { glTextureParameteri(*_id, utils::value(CTParam), utils::value(v)); }

		template <TextureLevelParam CTParam, std::enable_if_t<
			!isTargetSpecial(CTTarget) && !isTargetMultisampled(CTTarget) &&
			(CTParam == TextureLevelParam::RedType || CTParam == TextureLevelParam::GreenType
			 || CTParam == TextureLevelParam::BlueType || CTParam == TextureLevelParam::AlphaType
			 || CTParam == TextureLevelParam::DepthType)
			, int> = 0>
			TextureComponentType getLevelParameter(int level) const
		{
			int v;
			glGetTextureLevelParameteriv(*_id, level, utils::value(CTParam), &v);
			return static_cast<TextureComponentType>(v);
		}

		template <TextureLevelParam CTParam, std::enable_if_t<
			(isTargetSpecial(CTTarget) || isTargetMultisampled(CTTarget)) &&
			(CTParam == TextureLevelParam::RedType || CTParam == TextureLevelParam::GreenType
			 || CTParam == TextureLevelParam::BlueType || CTParam == TextureLevelParam::AlphaType
			 || CTParam == TextureLevelParam::DepthType)
			, int> = 0>
			TextureComponentType getLevelParameter() const
		{
			int v;
			glGetTextureLevelParameteriv(*_id, 0, utils::value(CTParam), &v);
			return static_cast<TextureComponentType>(v);
		}

		template <TextureLevelParam CTParam, std::enable_if_t<
			!isTargetSpecial(CTTarget) && !isTargetMultisampled(CTTarget) &&
			(CTParam == TextureLevelParam::Width || CTParam == TextureLevelParam::Height
			 || CTParam == TextureLevelParam::Depth || CTParam == TextureLevelParam::RedSize
			 || CTParam == TextureLevelParam::GreenSize || CTParam == TextureLevelParam::BlueSize
			 || CTParam == TextureLevelParam::AlphaSize || CTParam == TextureLevelParam::DepthSize
			 || CTParam == TextureLevelParam::CompressedSize)
			, int> = 0>
			int getLevelParameter(int level) const
		{
			int v;
			glGetTextureLevelParameteriv(*_id, level, utils::value(CTParam), &v);
			return v;
		}

		template <TextureLevelParam CTParam, std::enable_if_t<
			((isTargetSpecial(CTTarget) || isTargetMultisampled(CTTarget)) &&
			(CTParam == TextureLevelParam::Width || CTParam == TextureLevelParam::Height
			 || CTParam == TextureLevelParam::Depth || CTParam == TextureLevelParam::RedSize
			 || CTParam == TextureLevelParam::GreenSize || CTParam == TextureLevelParam::BlueSize
			 || CTParam == TextureLevelParam::AlphaSize || CTParam == TextureLevelParam::DepthSize
			 || CTParam == TextureLevelParam::CompressedSize))
			|| (CTTarget == TextureTarget::TextureBuffer &&
				(CTParam == TextureLevelParam::BufferOffset || CTParam == TextureLevelParam::BufferSize))
			, int> = 0>
			int getLevelParameter() const
		{
			int v;
			glGetTextureLevelParameteriv(*_id, 0, utils::value(CTParam), &v);
			return v;
		}

		template <TextureLevelParam CTParam, std::enable_if_t<
			!isTargetSpecial(CTTarget) && !isTargetMultisampled(CTTarget) &&
			CTParam == TextureLevelParam::Compressed
			, int> = 0>
		bool getLevelParameter(int level) const
		{
			int v;
			glGetTextureLevelParameteriv(*_id, level, utils::value(CTParam), &v);
			return v;
		}

		template <TextureLevelParam CTParam, std::enable_if_t<
			(isTargetSpecial(CTTarget) || isTargetMultisampled(CTTarget)) &&
			CTParam == TextureLevelParam::Compressed
			, int> = 0>
			bool getLevelParameter(int level) const
		{
			int v;
			glGetTextureLevelParameteriv(*_id, level, utils::value(CTParam), &v);
			return v;
		}

		operator const TextureInfo() const { return TextureInfo{ _id, _target }; }
		const TextureInfo getInfo() const { return TextureInfo{ _id, _target }; }
		TextureTarget getTarget() const { return _target; }
		TextureFormat getFormat() const { return _format; }

	protected:
		/**
		 * Registered target of the texture object.
		 */
		const TextureTarget _target;
		/**
		 * Registered internal format of the texture object.
		 */
		const TextureFormat _format;
		/**
		 * Whether storage requirements were set.
		 */
		DEBUG_ONLY(bool _requirementsSet = false);

		friend struct TextureView;

		/**
		 * Create a texture view of a different texture.
		 */
		template <TextureTarget CTorigTarget, TextureFormat CTorigFormat>
		Texture(const Texture<CTorigTarget, CTorigFormat>& origTex, int minLevel, int numLevels, int minLayer, int numLayers) : GLObject(), _target(CTTarget), _format(CTFormat)
		{
			glTextureView(*_id, utils::value(_target), *origTex.getInfo().id, utils::value(_format), minLevel, numLevels, minLayer, numLayers);
			DEBUG_ONLY(_requirementsSet = true);
		}
	};
}
