#pragma once

#include <string>
#include <type_traits>

#include "Empty/render/gl/GLObject.h"
#include "Empty/utils/utils.hpp"

namespace Empty::render::gl
{
	/**
	 * General-purpose shader object class. Does not map one-to-one with OpenGL shader objects, but holds a shared reference
	 * to one.
	 */
	struct Shader : public GLObject<ShaderId>
	{
		/**
		 * Constructs a shader object for a given shader stage.
		 */
		Shader(ShaderType type) : GLObject(type) {}

		/**
		 * Sets the source string of a shader and attempts to compile it,
		 * returning whether it succeeded.
		 */
		inline bool setSource(const std::string& src) const
		{
			auto ind = src.c_str();
			glShaderSource(*_id, 1, &ind, NULL);
			glCompileShader(*_id);
			return getParameter<ShaderParam::CompileStatus>();
		}

		/**
		 * Same as `setSource`, but fetches the source string from
		 * a given file.
		 */
		inline bool setSourceFromFile(const std::string& path) const
		{
			return setSource(utils::getFileContents(path));
		}

		/**
		 * Returns the current information log for this shader.
		 */
		std::string getLog() const
		{
			int logSize = getParameter<ShaderParam::InfoLogLength>();
			std::string log;
			if (logSize > 0)
			{
				log.resize(logSize);
				glGetShaderInfoLog(*_id, logSize, &logSize, log.data());
			}
			return log;
		}

		template <ShaderParam CTParam,
			std::enable_if_t<CTParam == ShaderParam::CompileStatus || CTParam == ShaderParam::DeleteStatus, int> = 0>
			inline bool getParameter() const
		{
			int p;
			glGetShaderiv(*_id, utils::value(CTParam), &p);
			return p;
		}

		template <ShaderParam CTParam,
			std::enable_if_t<CTParam == ShaderParam::InfoLogLength|| CTParam == ShaderParam::SourceLength, int> = 0>
			inline int getParameter() const
		{
			int p;
			glGetShaderiv(*_id, utils::value(CTParam), &p);
			return p;
		}

		template <ShaderParam CTParam,
			std::enable_if_t<CTParam == ShaderParam::Type, int> = 0>
			inline ShaderType getParameter() const
		{
			int p;
			glGetShaderiv(*_id, utils::value(CTParam), &p);
			return static_cast<ShaderType>(p);
		}

		inline operator GLuint() const { return *_id; }
	};
}