#pragma once

#include "Empty/render/gl/GLObject.h"
#include "Empty/render/gl/GLEnums.hpp"
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

		operator const FramebufferInfo() const { return FramebufferInfo{ _id }; }
		const FramebufferInfo getInfo() const { return FramebufferInfo{ _id }; }
	};
}
