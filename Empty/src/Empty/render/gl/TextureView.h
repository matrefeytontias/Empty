#pragma once

#include "Empty/render/gl/Texture.h"

namespace render::gl
{
	/**
	 * Helper class to generate texture views over textures or other texture views. Texture views
	 * are identical to texture objects in all aspects, with the one quirk that instead
	 * of having their own data store, they use that of an existing texture object, possibly reinterpreted
	 * in a different format (no conversion happens). As such, their memory usage is virtually zero, and
	 * they allow for the isolation of parts of a texture object's data store, like specific mipmap levels,
	 * layers of an array or faces of a cubemap.
	 * 
	 * Overloads of the single `make` method ensures that only valid combinations of targets are usable.
	 */
	struct TextureView
	{
		TextureView() = delete;

		/**
		 * Creates a rectangle texture view over a rectangle texture.
		 */
		template <TextureTarget CTTarget, TextureFormat CTFormat,
			TextureTarget CTOrigTarget, TextureFormat CTOrigFormat,
			std::enable_if_t<
				CTTarget == CTOrigTarget && CTTarget == TextureTarget::TextureRectangle,
				int> = 0
		>
		static auto make(const Texture<CTOrigTarget, CTOrigFormat>& tex)
		{
			return Texture<CTTarget, CTFormat>(tex, 0, 1, 0, 1);
		}

#define COPY_CTPARAMS(c) TextureTarget CTTarget, TextureFormat CTFormat, TextureTarget CTOrigTarget, TextureFormat CTOrigFormat, \
			std::enable_if_t<!isTargetSpecial(CTTarget) && !isTargetSpecial(CTOrigTarget) && !isTargetProxy(CTTarget) && !isTargetProxy(CTOrigTarget) && (c), int> = 0
#define COPY_PARAMS const Texture<CTOrigTarget, CTOrigFormat>& tex, int minLevel, int numLevels

		/**
		 * Creates a texture view (layered or not) over a non-layered texture.
		 */
		template <COPY_CTPARAMS(CTTarget == CTOrigTarget && !isTargetLayered(CTTarget) || isTargetArrayOf(CTTarget, CTOrigTarget))>
		static auto make(COPY_PARAMS)
		{
			if constexpr (CTOrigTarget == TextureTarget::TextureCubemap)
				return Texture<CTTarget, CTFormat>(tex, minLevel, numLevels, 0, 6);
			// else
			return Texture<CTTarget, CTFormat>(tex, minLevel, numLevels, 0, 1);
		}

		/**
		 * Creates a non-layered texture view over a layered texture. For cubemap
		 * arrays, a single "layer" is an entire cubemap, not a layer-face.
		 */
		template <COPY_CTPARAMS(isTargetArrayOf(CTOrigTarget, CTTarget))>
		static auto make(COPY_PARAMS, int layer)
		{
			if constexpr (CTOrigTarget == TextureTarget::TextureCubemapArray)
				return Texture<CTTarget, CTFormat>(tex, minLevel, numLevels, layer * 6, 6);
			// else
			return Texture<CTTarget, CTFormat>(tex, minLevel, numLevels, layer, 1);
		}

		/**
		 * Creates a layered texture view over a layered texture. For cubemap array
		 * views over cubemap arrays, a single "layer" is an entire cubemap, not a
		 * layer-face. In all other cases, a single "layer" is a layer-face.
		 */
		template <COPY_CTPARAMS(CTTarget == CTOrigTarget && isTargetLayered(CTTarget) || CTTarget == TextureTarget::Texture2DArray && CTOrigTarget == TextureTarget::TextureCubemapArray)>
		static auto make(COPY_PARAMS, int minLayer, int numLayers)
		{
			if constexpr (CTTarget == TextureTarget::TextureCubemapArray)
				return Texture<CTTarget, CTFormat>(tex, minLevel, numLevels, minLayer * 6, numLayers * 6);
			// else
			return Texture<CTTarget, CTFormat>(tex, minLevel, numLevels, minLayer, numLayers);
		}

		/**
		 * Creates a 2D texture view over a cubemap.
		 */
		template <COPY_CTPARAMS(CTTarget == TextureTarget::Texture2D && CTOrigTarget == TextureTarget::TextureCubemap)>
		static auto make(COPY_PARAMS, CubemapFace face)
		{
			return Texture<CTTarget, CTFormat>(tex, minLevel, numLevels, cubemapFaceIndex(face), 1);
		}

		/**
		 * Creates a 2D texture view over a cubemap array. A single "layer" is an
		 * entire cubemap.
		 */
		template <COPY_CTPARAMS(CTTarget == TextureTarget::Texture2D && CTOrigTarget == TextureTarget::TextureCubemapArray)>
		static auto make(COPY_PARAMS, int layer, CubemapFace face)
		{
			return Texture<CTTarget, CTFormat>(tex, minLevel, numLevels, layer * 6 + cubemapFaceIndex(face), 1);
		}

		/**
		 * Creates a 2D array texture view over a cubemap.
		 */
		template <COPY_CTPARAMS(CTTarget == TextureTarget::Texture2DArray && CTOrigTarget == TextureTarget::TextureCubemap)>
		static auto make(COPY_PARAMS, CubemapFace startFace, int faces)
		{
			return Texture<CTTarget, CTFormat>(tex, minLevel, numLevels, cubemapFaceIndex(startFace), faces);
		}
	};
}
