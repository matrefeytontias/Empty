#pragma once

#include "Empty/render/gl/GLObject.h"
#include "Empty/render/gl/GLEnums.hpp"

namespace render::gl
{
	struct RenderbufferInfo
	{
		std::shared_ptr<RenderbufferId> id;
	};

	/**
	 * General-purpose renderbuffer class. Does not map one-to-one with OpenGL renderbuffer objects, but holds a shared reference
	 * to one.
	 */
	struct Renderbuffer : public GLObject<RenderbufferId>
	{
		void setStorage(RenderbufferFormat format, int width, int height, int samples = 0)
		{
			glNamedRenderbufferStorageMultisample(*_id, samples, utils::value(format), width, height);
		}

		template <RenderbufferParam CTParam, std::enable_if_t<CTParam == RenderbufferParam::InternalFormat, int> = 0>
		RenderbufferFormat getParameter() const
		{
			int p;
			glGetNamedRenderbufferParameteriv(*_id, GL_RENDERBUFFER_INTERNAL_FORMAT, &p);
			return static_cast<RenderbufferFormat>(p);
		}

		template <RenderbufferParam CTParam, std::enable_if_t<CTParam != RenderbufferParam::InternalFormat, int> = 0>
		int getParameter() const
		{
			int p;
			glGetNamedRenderbufferParameteriv(*_id, utils::value(CTParam), &p);
			return p;
		}

		operator const RenderbufferInfo() const { return RenderbufferInfo{ _id }; }
		const RenderbufferInfo getInfo() const { return RenderbufferInfo{ _id }; }
	};
}