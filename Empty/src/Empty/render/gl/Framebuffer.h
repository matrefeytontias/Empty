#pragma once

#include "Empty/render/gl/GLObject.h"
#include "Empty/render/gl/GLEnums.hpp"
#include "Empty/render/gl/Renderbuffer.h"
#include "Empty/render/gl/Texture.h"
#include "Empty/utils/macros.h"

namespace render::gl
{
	/**
	 * This struct holds everything one needs to bind a Framebuffer. Useful in case
	 * one wants to bind the Framebuffer without having access to the entire object.
	 */
	struct FramebufferInfo
	{
		std::shared_ptr<FramebufferId> id;
	};

	/**
	 * General-purpose framebuffer class. Does not map one-to-one with OpenGL framebuffer objects, but holds a shared reference
	 * to one.
	 */
	struct Framebuffer : public GLObject<FramebufferId>
	{
#define COPY_CTPARAMS FramebufferAttachment CTAttachment, TextureTarget CTTarget, TextureFormat CTFormat
#define CTCONDITION(c) COPY_CTPARAMS, std::enable_if_t<CTAttachment != FramebufferAttachment::Color && !isTargetProxy(CTTarget) && (c), int> = 0
		
		/**
		 * Attach a single level of a single layer of a layered Texture to a non-color framebuffer attachment.
		 */
		template <CTCONDITION(isTargetLayered(CTTarget) && CTTarget != TextureTarget::TextureCubemapArray)>
		void attachTexture(const Texture<CTTarget, CTFormat>& tex, int level, int layer)
		{
			glNamedFramebufferTextureLayer(*_id, utils::value(CTAttachment), *tex.getInfo().id, level, layer);
		}

		/**
		 * Attach a single level of a non-layered Texture to a non-color framebuffer attachment.
		 */
		template <CTCONDITION(!isTargetLayered(CTTarget) && CTTarget != TextureTarget::TextureCubemap)>
		void attachTexture(const Texture<CTTarget, CTFormat>& tex, int level)
		{
			glNamedFramebufferTexture(*_id, utils::value(CTAttachment), *tex.getInfo().id, level);
		}

		/**
		 * Attach a single level of a single face of a cubemap Texture to a non-color framebuffer attachment.
		 */
		template <COPY_CTPARAMS, std::enable_if_t<CTTarget == TextureTarget::TextureCubemap, int> = 0>
		void attachTexture(const Texture<CTTarget, CTFormat>& tex, int level, CubemapFace face)
		{
			glNamedFramebufferTextureLayer(*_id, utils::value(CTAttachment), *tex.getInfo().id, level, cubemapFaceIndex(face));
		}

		/**
		 * Attach a single level of a single face of a single layer of a cubemap array Texture to a non-color framebuffer attachment.
		 */
		template <COPY_CTPARAMS, std::enable_if_t<CTTarget == TextureTarget::TextureCubemapArray, int> = 0>
		void attachTexture(const Texture<CTTarget, CTFormat>& tex, int level, int layer, CubemapFace face)
		{
			glNamedFramebufferTextureLayer(*_id, utils::value(CTAttachment), *tex.getInfo().id, level, layer * 6 + cubemapFaceIndex(face));
		}

#undef CTCONDITION
#define CTCONDITION(c) COPY_CTPARAMS, std::enable_if_t<CTAttachment == FramebufferAttachment::Color && !isTargetProxy(CTTarget) && (c), int> = 0
		
		/**
		 * Attach a single level of a single layer of a layered Texture to a color framebuffer attachment.
		 */
		template <CTCONDITION(isTargetLayered(CTTarget) && CTTarget != TextureTarget::TextureCubemapArray)>
		void attachTexture(int attachment, const Texture<CTTarget, CTFormat>& tex, int level, int layer)
		{
			glNamedFramebufferTextureLayer(*_id, utils::value(CTAttachment) + attachment, *tex.getInfo().id, level, layer);
		}

		/**
		 * Attach a single level of a non-layered Texture to a color framebuffer attachment.
		 */
		template <CTCONDITION(!isTargetLayered(CTTarget) && CTTarget != TextureTarget::TextureCubemap)>
		void attachTexture(int attachment, const Texture<CTTarget, CTFormat>& tex, int level)
		{
			glNamedFramebufferTexture(*_id, utils::value(CTAttachment) + attachment, *tex.getInfo().id, level);
		}

		/**
		 * Attach a single level of a single face of a cubemap Texture to a color framebuffer attachment.
		 */
		template <COPY_CTPARAMS, std::enable_if_t<CTTarget == TextureTarget::TextureCubemap, int> = 0>
		void attachTexture(int attachment, const Texture<CTTarget, CTFormat>& tex, int level, CubemapFace face)
		{
			glNamedFramebufferTextureLayer(*_id, utils::value(CTAttachment) + attachment, *tex.getInfo().id, level, cubemapFaceIndex(face));
		}

		/**
		 * Attach a single level of a single face of a single layer of a cubemap array Texture to a color framebuffer attachment.
		 */
		template <COPY_CTPARAMS, std::enable_if_t<CTTarget == TextureTarget::TextureCubemapArray, int> = 0>
		void attachTexture(int attachment, const Texture<CTTarget, CTFormat>& tex, int level, int layer, CubemapFace face)
		{
			glNamedFramebufferTextureLayer(*_id, utils::value(CTAttachment) + attachment, *tex.getInfo().id, level, layer * 6 + cubemapFaceIndex(face));
		}

#undef CTCONDITION
#undef COPY_CTPARAMS

		/**
		 * Attach a RenderBuffer to a color framebuffer attachment.
		 */
		template <FramebufferAttachment CTAttachment, std::enable_if_t<CTAttachment == FramebufferAttachment::Color, int> = 0>
		void attachRenderbuffer(const RenderbufferInfo& rb, int attachment)
		{
			glNamedFramebufferRenderbuffer(*_id, utils::value(CTAttachment) + attachment, GL_RENDERBUFFER, *rb.id);
		}

		/**
		 * Attach a RenderBuffer to a non-color framebuffer attachment.
		 */
		template <FramebufferAttachment CTAttachment, std::enable_if_t<CTAttachment != FramebufferAttachment::Color, int> = 0>
		void attachRenderbuffer(const RenderbufferInfo& rb)
		{
			glNamedFramebufferRenderbuffer(*_id, utils::value(CTAttachment), GL_RENDERBUFFER, *rb.id);
		}

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

		/**
		 * Checks the framebuffer's status against a target's requirements.
		 */
		FramebufferStatus checkStatus(FramebufferTarget t) const
		{
			return static_cast<FramebufferStatus>(glCheckNamedFramebufferStatus(*_id, utils::value(t)));
		}

		operator const FramebufferInfo() const { return FramebufferInfo{ _id }; }
		const FramebufferInfo getInfo() const { return FramebufferInfo{ _id }; }
	};
}
