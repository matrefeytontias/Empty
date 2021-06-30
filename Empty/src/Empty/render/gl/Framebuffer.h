#pragma once

#include "Empty/render/gl/GLObject.h"
#include "Empty/render/gl/GLEnums.hpp"
#include "Empty/render/gl/Texture.h"
#include "Empty/utils/macros.h"

namespace render::gl
{
	/**
	 * This struct holds everything one needs to bind a Framebuffer. Useful in case
	 * one wants to bind the Framebuffer without having access to the entire Framebuffer object.
	 */
	struct FramebufferInfo
	{
		std::shared_ptr<FramebufferId> id;
	};

	/**
	 * General-purpose framebuffer class. Does not map one-to-one with OpenGL buffer objects, but holds a shared reference
	 * to one.
	 */
	struct Framebuffer : public GLObject<FramebufferId>
	{
#define COPY_CTPARAMS DrawBufferType b, TextureTarget t, TextureFormat f
#define CTCONDITION(c) COPY_CTPARAMS, std::enable_if_t<b != DrawBufferType::Color && !isTargetProxy(t) && (c), int> = 0
		
		template <CTCONDITION(isTargetLayered(t) && t != TextureTarget::TextureCubeMapArray)>
		void attachTexture(const Texture<t, f>& tex, int level, int layer)
		{
			glNamedFramebufferTextureLayer(*_id, utils::value(b), *tex.getInfo().id, level, layer);
		}

		template <CTCONDITION(!isTargetLayered(t) && t != TextureTarget::TextureCubeMap)>
		void attachTexture(const Texture<t, f>& tex, int level)
		{
			glNamedFramebufferTexture(*_id, utils::value(b), *tex.getInfo().id, level);
		}

		template <COPY_CTPARAMS, std::enable_if_t<t == TextureTarget::TextureCubeMap, int> = 0>
		void attachTexture(const Texture<t, f>& tex, int level, CubeMapFace face)
		{
			glNamedFramebufferTextureLayer(*_id, utils::value(b), *tex.getInfo().id, level, cubemapFaceIndex(face));
		}

		template <COPY_CTPARAMS, std::enable_if_t<t == TextureTarget::TextureCubeMapArray, int> = 0>
		void attachTexture(const Texture<t, f>& tex, int level, int layer, CubeMapFace face)
		{
			glNamedFramebufferTextureLayer(*_id, utils::value(b), *tex.getInfo().id, level, layer * 6 + cubemapFaceIndex(face));
		}

#undef CTCONDITION
#define CTCONDITION(c) COPY_CTPARAMS, std::enable_if_t<b == DrawBufferType::Color && !isTargetProxy(t) && (c), int> = 0

		template <CTCONDITION(isTargetLayered(t) && t != TextureTarget::TextureCubeMapArray)>
		void attachTexture(int attachment, const Texture<t, f>& tex, int level, int layer)
		{
			glNamedFramebufferTextureLayer(*_id, utils::value(b) + attachment, *tex.getInfo().id, level, layer);
		}

		template <CTCONDITION(!isTargetLayered(t) && t != TextureTarget::TextureCubeMap)>
		void attachTexture(int attachment, const Texture<t, f>& tex, int level)
		{
			glNamedFramebufferTexture(*_id, utils::value(b) + attachment, *tex.getInfo().id, level);
		}

		template <COPY_CTPARAMS, std::enable_if_t<t == TextureTarget::TextureCubeMap, int> = 0>
		void attachTexture(int attachment, const Texture<t, f>& tex, int level, CubeMapFace face)
		{
			glNamedFramebufferTextureLayer(*_id, utils::value(b) + attachment, *tex.getInfo().id, level, cubemapFaceIndex(face));
		}

		template <COPY_CTPARAMS, std::enable_if_t<t == TextureTarget::TextureCubeMapArray, int> = 0>
		void attachTexture(int attachment, const Texture<t, f>& tex, int level, int layer, CubeMapFace face)
		{
			glNamedFramebufferTextureLayer(*_id, utils::value(b) + attachment, *tex.getInfo().id, level, layer * 6 + cubemapFaceIndex(face));
		}

#undef CTCONDITION
#undef COPY_CTPARAMS

		template <FramebufferParam CTParam, std::enable_if_t <
			CTParam == FramebufferParam::DefaultFixedSampleLocations
			, int> = 0>
		void setParameter(bool v) const
		{
			glNamedFramebufferParameteri(*_id, utils::value(CTParam), v);
		}

		template <FramebufferParam CTParam, std::enable_if_t <
			CTParam != FramebufferParam::DefaultFixedSampleLocations
			, int> = 0>
		void setParameter(unsigned int v) const
		{
			glNamedFramebufferParameteri(*_id, utils::value(CTParam), v);
		}

		FramebufferStatus checkStatus(FramebufferTarget t) const
		{
			return static_cast<FramebufferStatus>(glCheckNamedFramebufferStatus(*_id, utils::value(t)));
		}

		operator const FramebufferInfo() const { return FramebufferInfo{ _id }; }
		const FramebufferInfo getInfo() const { return FramebufferInfo{ _id }; }
	};
}
