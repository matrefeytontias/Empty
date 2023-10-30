#pragma once

#include <type_traits>

#include "glad/glad.h"
#include "Empty/gl/GLEnumsUtils.h"
#include "Empty/gl/GLObject.h"
#include "Empty/math/mathutils.hpp"
#include "Empty/utils/macros.h"

namespace Empty::gl
{
	/**
	 * This struct holds everything one needs to bind a Texture. Useful in case
	 * one wants to bind the Texture without having access to the entire object.
	 */
	struct TextureInfo
	{
		std::shared_ptr<TextureId> id;

		operator GLuint() const { return *id; }
	};

	struct TextureLevelInfo
	{
		std::shared_ptr<TextureId> id;
		int level = -1;
		bool layered = false;
		int layer = -1;
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

		template <COPY_CTPARAMS>
		Texture(std::enable_if_t<t == TextureTarget::Dynamic && f == TextureFormat::Dynamic, TextureTarget> target, TextureFormat format, const std::string& label) : GLObject(target, label), _target(target), _format(format) {}

		template <COPY_CTPARAMS>
		Texture(std::enable_if_t<t != TextureTarget::Dynamic && f == TextureFormat::Dynamic, TextureFormat> format, const std::string& label) : GLObject(t, label), _target(t), _format(format) {}

		template <COPY_CTPARAMS>
		Texture(std::enable_if_t<t == TextureTarget::Dynamic && f != TextureFormat::Dynamic, TextureTarget> target, const std::string& label) : GLObject(target, label), _target(target), _format(f) {}

		template <COPY_CTPARAMS, std::enable_if_t<t != TextureTarget::Dynamic && f != TextureFormat::Dynamic, int> = 0>
		Texture(const std::string& label) : GLObject(t, label), _target(t), _format(f) {}

#undef COPY_CTPARAMS

#define COPY_CTPARAMS TextureTarget t = CTTarget

		/**
		 * Allocate storage for a 1-dimensional texture.
		 */
		template <COPY_CTPARAMS>
		inline std::enable_if_t<dimensionsFromTarget(t) == 1> setStorage(int levels, int width)
		{
			ASSERT(!_requirementsSet && "Storage requirements were already set for this Texture");
			glTextureStorage1D(*_id, levels, utils::value(_format), width);
			DEBUG_ONLY(_requirementsSet = true);
		}

		/**
		 * Allocate storage for a 2-dimensional texture. For 1D array textures, `height` is the number
		 * of layers.
		 */
		template <COPY_CTPARAMS>
		inline std::enable_if_t<dimensionsFromTarget(t) == 2> setStorage(int levels, int width, int height)
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
		template <COPY_CTPARAMS>
		inline std::enable_if_t<dimensionsFromTarget(t) == 3> setStorage(int levels, int width, int height, int depth)
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
		template <COPY_CTPARAMS>
		std::enable_if_t<t == TextureTarget::Dynamic> setStorage(int levels, int width, int height = 1, int depth = 1)
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
		template <COPY_CTPARAMS>
		inline std::enable_if_t<dimensionsFromTarget(t) == 1> uploadData(int level, int x, int w, DataFormat format, DataType type, const void* data) const
		{
			ASSERT(_requirementsSet);
			glTextureSubImage1D(*_id, level, x, w, utils::value(format), utils::value(type), data);
		}

		/**
		 * Upload data to a 2-dimensional texture that is not a cubemap.
		 */
		template <COPY_CTPARAMS>
		inline std::enable_if_t<dimensionsFromTarget(t) == 2 && !isTargetCubemap(t)> uploadData(int level, int x, int y, int w, int h, DataFormat format, DataType type, const void* data) const
		{
			ASSERT(_requirementsSet);
			glTextureSubImage2D(*_id, level, x, y, w, h, utils::value(format), utils::value(type), data);
		}

		/**
		 * Upload data to a 3-dimensional texture that is not a cubemap array.
		 */
		template <COPY_CTPARAMS>
		inline std::enable_if_t<dimensionsFromTarget(t) == 3 && !isTargetCubemap(t)> uploadData(int level, int x, int y, int z, int w, int h, int d, DataFormat format, DataType type, const void* data) const
		{
			ASSERT(_requirementsSet);
			glTextureSubImage3D(*_id, level, x, y, z, w, h, d, utils::value(format), utils::value(type), data);
		}

		/**
		 * Upload data to a cubemap texture. `face` is the starting face of the upload operation,
		 * and `faces` how many should be filled.
		 */
		template <COPY_CTPARAMS>
		inline std::enable_if_t<dimensionsFromTarget(t) == 2 && isTargetCubemap(t)> uploadData(CubemapFace face, int level, int x, int y, int w, int h, int faces, DataFormat format, DataType type, const void* data) const
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
		template <COPY_CTPARAMS>
		inline std::enable_if_t<dimensionsFromTarget(t) == 3 && isTargetCubemap(t)> uploadData(CubemapFace face, int level, int x, int y, int layer, int w, int h, int faces, DataFormat format, DataType type, const void* data) const
		{
			ASSERT(_requirementsSet);
			glTextureSubImage3D(*_id, level, x, y, layer * 6 + cubemapFaceIndex(face), w, h, faces, utils::value(format), utils::value(type), data);
		}

		/**
		 * Upload data to a non-cubemap texture with target unknown at compile-time. Parameters
		 * that are not relevant to the actual texture target are ignored.
		 */
		template <COPY_CTPARAMS>
		std::enable_if_t<t == TextureTarget::Dynamic> uploadData(int level, int x, int y, int z, int w, int h, int d, DataFormat format, DataType type, const void* data) const
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
		template <COPY_CTPARAMS>
		inline std::enable_if_t<t == TextureTarget::Dynamic> uploadData(CubemapFace face, int level, int x, int y, int layer, int w, int h, int faces, DataFormat format, DataType type, const void* data) const
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
		 * Fills a texture level with zeroes.
		 * TODO: filter out the combinations of DataFormat, DataType and TextureFormat that end up in a GL error.
		 */
		template <DataFormat format, DataType type>
		inline std::enable_if_t<CTTarget != TextureTarget::Dynamic && !isTargetProxy(CTTarget) && !isTargetSpecial(CTTarget)>
			clearLevel(int level) const
		{
			ASSERT(_requirementsSet);

			static constexpr int dataComponents = componentsInTextureFormat(CTFormat);
			using Data = typename Empty::math::FormVecType<BaseType<TextureFormat, CTFormat>, dataComponents>;

			if constexpr (dataComponents == 1)
			{
				Data zero(0);
				glClearTexImage(*_id, level, utils::value(format), utils::value(type), &zero);
			}
			else
				glClearTexImage(*_id, level, utils::value(format), utils::value(type), Data::zero);
		}
		
		/**
		 * Fills a texture level with a value. The type of the value depends on what the texture target and format holds.
		 * TODO: filter out the combinations of DataFormat, DataType and TextureFormat that end up in a GL error.
		 */
		template <DataFormat format, DataType type>
		inline std::enable_if_t<CTTarget != TextureTarget::Dynamic && !isTargetProxy(CTTarget) && !isTargetSpecial(CTTarget)>
			clearLevel(int level, typename Empty::math::FormVecType<BaseType<TextureFormat, CTFormat>, componentsInTextureFormat(CTFormat)> data) const
		{
			ASSERT(_requirementsSet);

			glClearTexImage(*_id, level, utils::value(format), utils::value(type), &data);
		}

		/**
		 * Creates a rectangle texture view over a rectangle texture.
		 */
		template <TextureTarget CTNewTarget, TextureFormat CTNewFormat>
		std::enable_if_t<CTNewTarget == CTTarget && CTNewTarget == TextureTarget::TextureRectangle, Texture<CTNewTarget, CTNewFormat>> makeView(const std::string& label)
		{
			return Texture<CTNewTarget, CTNewFormat>(*this, 0, 1, 0, 1, label);
		}

#define SIGNATURE(c) template <TextureTarget CTNewTarget, TextureFormat CTNewFormat> \
			std::enable_if_t<!isTargetSpecial(CTNewTarget) && !isTargetSpecial(CTTarget) && !isTargetProxy(CTNewTarget) && !isTargetProxy(CTTarget) && (c), \
			Texture<CTNewTarget, CTNewFormat>>

		/**
		 * Creates a texture view (layered or not) over a non-layered texture.
		 */
		SIGNATURE(CTNewTarget == CTTarget && !isTargetLayered(CTNewTarget) || isTargetArrayOf(CTNewTarget, CTTarget))
			makeView(int minLevel, int numLevels, const std::string& label)
		{
			if constexpr (CTTarget == TextureTarget::TextureCubemap)
				return Texture<CTNewTarget, CTNewFormat>(*this, minLevel, numLevels, 0, 6, label);
			// else
			return Texture<CTNewTarget, CTNewFormat>(*this, minLevel, numLevels, 0, 1, label);
		}

		/**
		 * Creates a non-layered texture view over a layered texture. For cubemap
		 * arrays, a single "layer" is an entire cubemap, not a layer-face.
		 */
		SIGNATURE(isTargetArrayOf(CTTarget, CTNewTarget))
			makeView(int minLevel, int numLevels, int layer, const std::string& label)
		{
			if constexpr (CTTarget == TextureTarget::TextureCubemapArray)
				return Texture<CTNewTarget, CTNewFormat>(*this, minLevel, numLevels, layer * 6, 6, label);
			// else
			return Texture<CTNewTarget, CTNewFormat>(*this, minLevel, numLevels, layer, 1, label);
		}

		/**
		 * Creates a layered texture view over a layered texture. For cubemap array
		 * views over cubemap arrays, a single "layer" is an entire cubemap, not a
		 * layer-face. In all other cases, a single "layer" is a layer-face.
		 */
		SIGNATURE(CTNewTarget == CTTarget && isTargetLayered(CTNewTarget) || CTNewTarget == TextureTarget::Texture2DArray && CTTarget == TextureTarget::TextureCubemapArray)
			makeView(int minLevel, int numLevels, int minLayer, int numLayers, const std::string& label)
		{
			if constexpr (CTNewTarget == TextureTarget::TextureCubemapArray)
				return Texture<CTNewTarget, CTNewFormat>(*this, minLevel, numLevels, minLayer * 6, numLayers * 6, label);
			// else
			return Texture<CTNewTarget, CTNewFormat>(*this, minLevel, numLevels, minLayer, numLayers, label);
		}

		/**
		 * Creates a 2D texture view over a cubemap.
		 */
		SIGNATURE(CTNewTarget == TextureTarget::Texture2D && CTTarget == TextureTarget::TextureCubemap)
			makeView(int minLevel, int numLevels, CubemapFace face, const std::string& label)
		{
			return Texture<CTNewTarget, CTNewFormat>(*this, minLevel, numLevels, cubemapFaceIndex(face), 1, label);
		}

		/**
		 * Creates a 2D texture view over a cubemap array. A single "layer" is an
		 * entire cubemap.
		 */
		SIGNATURE(CTNewTarget == TextureTarget::Texture2D && CTTarget == TextureTarget::TextureCubemapArray)
			makeView(int minLevel, int numLevels, int layer, CubemapFace face, const std::string& label)
		{
			return Texture<CTNewTarget, CTNewFormat>(*this, minLevel, numLevels, layer * 6 + cubemapFaceIndex(face), 1, label);
		}

		/**
		 * Creates a 2D array texture view over a cubemap.
		 */
		SIGNATURE(CTNewTarget == TextureTarget::Texture2DArray && CTTarget == TextureTarget::TextureCubemap)
			makeView(int minLevel, int numLevels, CubemapFace startFace, int faces, const std::string& label)
		{
			return Texture<CTNewTarget, CTNewFormat>(*this, minLevel, numLevels, cubemapFaceIndex(startFace), faces, label);
		}
#undef SIGNATURE

		/**
		 * Populate the texture's levels with mipmaps. Only usable on texture targets that do have mipmaps.
		 */
		template <typename = std::enable_if_t<!isTargetSpecial(CTTarget) && !isTargetMultisampled(CTTarget) && !isTargetProxy(CTTarget)>>
		void generateMipmaps()
		{
			glGenerateTextureMipmap(*_id);
		}

#define COPY_CTPARAMS TextureTarget t = CTTarget

		/**
		 * Get information about one of a non-layered texture's levels.
		 */
		template <COPY_CTPARAMS>
		std::enable_if_t<!isTargetLayered(t), TextureLevelInfo> getLevel(int level)
		{
			ASSERT(_requirementsSet);
			return TextureLevelInfo{ _id, level, false, 0 };
		}

		/**
		 * Get information about one of a layered texture's levels. The whole
		 * level is selected as opposed to a single layer of it.
		 */
		template <COPY_CTPARAMS>
		std::enable_if_t<isTargetLayered(t), TextureLevelInfo> getLevel(int level)
		{
			ASSERT(_requirementsSet);
			return TextureLevelInfo{ _id, level, true, -1 };
		}

		/**
		 * Get information about one layer of a layered texture's levels.
		 */
		template <COPY_CTPARAMS>
		std::enable_if_t<isTargetLayered(t), TextureLevelInfo> getLevel(int level, int layer)
		{
			ASSERT(_requirementsSet);
			return TextureLevelInfo{ _id, level, false, layer };
		}

#undef COPY_CTPARAMS

		template <TextureParam CTParam>
		std::enable_if_t<utils::isOneOf(CTParam, TextureParam::BaseLevel, TextureParam::MaxLevel, TextureParam::ViewMinLayer, TextureParam::ViewNumLayers,
			TextureParam::ViewMinLevel, TextureParam::ViewNumLevels, TextureParam::ViewImmutableLevels, TextureParam::HasImmutableFormat)
			, int> getParameter() const
		{
			int p;
			glGetTextureParameteriv(*_id, utils::value(CTParam), &p);
			return p;
		}

		template <TextureParam CTParam>
		std::enable_if_t<utils::isOneOf(CTParam, TextureParam::DepthStencilMode, TextureParam::CompareFunc, TextureParam::CompareMode,
			TextureParam::MinFilter, TextureParam::MagFilter, TextureParam::SwizzleR, TextureParam::SwizzleG,
			TextureParam::SwizzleB, TextureParam::SwizzleA, TextureParam::WrapS,
			TextureParam::WrapT, TextureParam::WrapR, TextureParam::ImageFormatCompatibilityType)
			, TextureParamValue> getParameter() const
		{
			int p;
			glGetTextureParameteriv(*_id, utils::value(CTParam), &p);
			return static_cast<TextureParamValue>(p);
		}

		template <TextureParam CTParam>
		std::enable_if_t<utils::isOneOf(CTParam, TextureParam::LODBias, TextureParam::MinLOD, TextureParam::MaxLOD)
			, float> getParameter() const
		{
			float v;
			glGetTextureParameterfv(*_id, utils::value(CTParam), &v);
			return v;
		}

		template <TextureParam CTParam>
		std::enable_if_t<utils::isOneOf(CTParam, TextureParam::BaseLevel, TextureParam::MaxLevel)>
			setParameter(int v) const { glTextureParameteri(*_id, utils::value(CTParam), v); }

		template <TextureParam CTParam>
		std::enable_if_t<utils::isOneOf(CTParam, TextureParam::LODBias, TextureParam::MinLOD, TextureParam::MaxLOD)>
			setParameter(float v) const { glTextureParameterf(*_id, utils::value(CTParam), v); }

		template <TextureParam CTParam>
		std::enable_if_t<utils::isOneOf(CTParam, TextureParam::DepthStencilMode, TextureParam::CompareFunc, TextureParam::CompareMode,
			TextureParam::MinFilter, TextureParam::MagFilter, TextureParam::SwizzleR,
			TextureParam::SwizzleG, TextureParam::SwizzleB, TextureParam::SwizzleA,
			TextureParam::WrapS, TextureParam::WrapT, TextureParam::WrapR)>
			setParameter(TextureParamValue v) const { glTextureParameteri(*_id, utils::value(CTParam), utils::value(v)); }

		template <TextureLevelParam CTParam>
		std::enable_if_t<
			!isTargetSpecial(CTTarget) && !isTargetMultisampled(CTTarget) &&
			utils::isOneOf(CTParam, TextureLevelParam::RedType, TextureLevelParam::GreenType,
				TextureLevelParam::BlueType, TextureLevelParam::AlphaType, TextureLevelParam::DepthType)
			, TextureComponentType> getLevelParameter(int level) const
		{
			int v;
			glGetTextureLevelParameteriv(*_id, level, utils::value(CTParam), &v);
			return static_cast<TextureComponentType>(v);
		}

		template <TextureLevelParam CTParam>
		std::enable_if_t<
			(isTargetSpecial(CTTarget) || isTargetMultisampled(CTTarget)) &&
			utils::isOneOf(CTParam, TextureLevelParam::RedType, TextureLevelParam::GreenType,
				TextureLevelParam::BlueType, TextureLevelParam::AlphaType, TextureLevelParam::DepthType)
			, TextureComponentType> getLevelParameter() const
		{
			int v;
			glGetTextureLevelParameteriv(*_id, 0, utils::value(CTParam), &v);
			return static_cast<TextureComponentType>(v);
		}

		template <TextureLevelParam CTParam>
		std::enable_if_t<
			!isTargetSpecial(CTTarget) && !isTargetMultisampled(CTTarget) &&
			utils::isOneOf(CTParam, TextureLevelParam::Width, TextureLevelParam::Height,
				TextureLevelParam::Depth, TextureLevelParam::RedSize, TextureLevelParam::GreenSize,
				TextureLevelParam::BlueSize, TextureLevelParam::AlphaSize, TextureLevelParam::DepthSize,
				TextureLevelParam::CompressedSize)
			, int> getLevelParameter(int level) const
		{
			int v;
			glGetTextureLevelParameteriv(*_id, level, utils::value(CTParam), &v);
			return v;
		}

		template <TextureLevelParam CTParam>
		std::enable_if_t<
			((isTargetSpecial(CTTarget) || isTargetMultisampled(CTTarget)) &&
				utils::isOneOf(CTParam, TextureLevelParam::Width, TextureLevelParam::Height,
					TextureLevelParam::Depth, TextureLevelParam::RedSize, TextureLevelParam::GreenSize,
					TextureLevelParam::BlueSize, TextureLevelParam::AlphaSize, TextureLevelParam::DepthSize,
					TextureLevelParam::CompressedSize))
			|| (CTTarget == TextureTarget::TextureBuffer &&
				utils::isOneOf(CTParam, TextureLevelParam::BufferDataStoreBinding, TextureLevelParam::BufferOffset, TextureLevelParam::BufferSize))
			, int> getLevelParameter() const
		{
			int v;
			glGetTextureLevelParameteriv(*_id, 0, utils::value(CTParam), &v);
			return v;
		}

		template <TextureLevelParam CTParam>
		std::enable_if_t<
			!isTargetSpecial(CTTarget) && !isTargetMultisampled(CTTarget) &&
			CTParam == TextureLevelParam::Compressed
			, bool> getLevelParameter(int level) const
		{
			int v;
			glGetTextureLevelParameteriv(*_id, level, utils::value(CTParam), &v);
			return v;
		}

		template <TextureLevelParam CTParam>
		std::enable_if_t<
			(isTargetSpecial(CTTarget) || isTargetMultisampled(CTTarget)) &&
			CTParam == TextureLevelParam::Compressed
			, bool> getLevelParameter(int level) const
		{
			int v;
			glGetTextureLevelParameteriv(*_id, level, utils::value(CTParam), &v);
			return v;
		}

		operator const TextureInfo() const { return TextureInfo{ _id }; }
		const TextureInfo getInfo() const { return TextureInfo{ _id }; }
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
		Texture(const Texture<CTorigTarget, CTorigFormat>& origTex, int minLevel, int numLevels, int minLayer, int numLayers, const std::string& label) : GLObject(), _target(CTTarget), _format(CTFormat)
		{
			glTextureView(*_id, utils::value(_target), *origTex.getInfo().id, utils::value(_format), minLevel, numLevels, minLayer, numLayers);
			glObjectLabel(utils::value(ObjectNamespace::Texture), *_id, -1, label.c_str());
			DEBUG_ONLY(_requirementsSet = true);
		}
	};
}
