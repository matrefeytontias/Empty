#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <type_traits>

#include "Empty/math/vec.h"
#include "Empty/gl/GLObject.h"
#include "Empty/gl/Shader.h"
#include "Empty/gl/Texture.h"
#include "Empty/gl/Uniform.h"
#include "Empty/gl/VertexStructure.h"
#include "Empty/utils/utils.hpp"

namespace Empty::gl
{
	/**
	 * This struct holds everything one needs to bind a ShaderProgram. Useful in case
	 * one wants to bind the ShaderProgram without having access to the entire object.
	 */
	struct ShaderProgramInfo
	{
		std::shared_ptr<ProgramId> id;

		operator GLuint() const { return *id; }
	};

	/**
	 * Holds information about a uniform in the context of
	 * a ShaderProgram.
	 */
	struct ProgramUniform
	{
		std::shared_ptr<UniformBase> uniform;
		location loc = -1;
	};

	/**
	 * Holds information about a texture in the context of
	 * a ShaderProgram.
	 */
	struct ProgramTextureInfo
	{
		TextureInfo textureInfo;
		int unit = -1;
		bool autobind = false;
	};

	/**
	 * General-purpose shader program class. Does not map one-to-one with OpenGL shader
	 * program objects, but holds a shared reference to one. This class offers extra
	 * functionality to handle Shader objects, Uniform objects and vertex attributes.
	 */
	struct ShaderProgram : public GLObject<ProgramId>
	{
	public:
		ShaderProgram() {}
		~ShaderProgram();

		/**
		 * Shorthand for creating a Shader object of a given type,
		 * setting its source string and compiling it, returning
		 * whether it succeeded.
		 */
		bool attachSource(ShaderType type, const std::string& src);
		/**
		 * Same as `attachSource` but fetches the source string from
		 * a given file.
		 */
		inline bool attachFile(ShaderType type, const std::string& path)
		{
			return attachSource(type, utils::getFileContents(path));
		}

		/**
		 * Attemps to link the program, returning whether it succeeded or not.
		 */
		bool build()
		{
			glLinkProgram(*_id);
			return DEBUG_ONLY(_built = ) getParameter<ProgramParam::LinkStatus>();
		}

		/**
		 * Finds and stores attribute locations in the provided VertexStructure in-place.
		 */
		void locateAttributes(VertexStructure& vStruct);

		/**
		 * Looks up or computes and caches an attribute's location.
		 */
		location findAttribute(const std::string& name);
		/**
		 * Looks up or computes and caches a uniform's location.
		 */
		location findUniform(const std::string& name);

		/**
		 * Sets a Uniform's value. The type is deduced from the value's
		 * type or given explicitely through templating.
		 */
		template <typename T>
		void uniform(const std::string& name, const T& value)
		{
			ASSERT(_built);

			// TODO : we don't need to look for the uniform in the map immediately after putting it there

			auto entry = _uniforms.find(name);
			if (entry == _uniforms.end())
			{
				auto& u = _uniforms[name] = ProgramUniform{ std::make_shared<Uniform<T>>(name, value), -1 };
				u.uniform->upload(_id, findUniform(name));
			}
			else if (auto v = dynamic_cast<Uniform<T>*>(entry->second.uniform.get()))
			{
				v->value = value;
				entry->second.uniform->upload(_id, entry->second.loc);
			}
			else
				FATAL("Tried to set existing uniform '" << name << "' with value of the wrong type " << value);
		}

		/**
		 * Returns an array of all known uniforms and their location (includes textures).
		 */
		std::vector<ProgramUniform> dumpUniforms() const;
		/**
		 * Returns an array of all registered textures and their location.
		 */
		std::vector<ProgramTextureInfo> dumpTextures() const;

		/**
		 * Registers a Texture to the shader pogram - this also creates a Uniform
		 * for the sampler. Registered textures can be set to be automatically bound
		 * when a shader program is set as active by the Context.
		 */
		void registerTexture(const std::string& name, const TextureInfo& tex, bool autobind = true);
		size_t getTexturesAmount() const { return _textures.size(); }

		template <ProgramParam CTParam,
			std::enable_if_t<CTParam == ProgramParam::DeleteStatus || CTParam == ProgramParam::LinkStatus
			|| CTParam == ProgramParam::ValidateStatus, int> = 0>
			inline bool getParameter() const
		{
			int p;
			glGetProgramiv(*_id, utils::value(CTParam), &p);
			return p;
		}

		template <ProgramParam CTParam,
			std::enable_if_t<CTParam == ProgramParam::InfoLogLength || CTParam == ProgramParam::AttachedShaders
			|| CTParam == ProgramParam::ActiveAtomicCounterBuffers || CTParam == ProgramParam::ActiveAttributes
			|| CTParam == ProgramParam::ActiveAttributeMaxNameLength || CTParam == ProgramParam::ActiveUniforms
			|| CTParam == ProgramParam::ActiveUniformMaxNameLength || CTParam == ProgramParam::BinaryLength
			|| CTParam == ProgramParam::TransformFeedbackVaryings || CTParam == ProgramParam::TransformFeedbackVaryingMaxNameLength
			|| CTParam == ProgramParam::GeometryVerticesOut
			, int> = 0>
			inline int getParameter() const
		{
			int p;
			glGetProgramiv(*_id, utils::value(CTParam), &p);
			return p;
		}

		template <ProgramParam CTParam,
			std::enable_if_t<CTParam == ProgramParam::ComputeWorkGroupSize, int> = 0>
			inline math::ivec3 getParameter() const
		{
			math::Vector3i p;
			glGetProgramiv(*_id, utils::value(CTParam), p);
			return p;
		}

		template <ProgramParam CTParam,
			std::enable_if_t<CTParam == ProgramParam::RetrievableBinaryHint || CTParam == ProgramParam::Separable, int> = 0>
			inline void setParameter(bool v) const
		{
			glProgramParameteri(*_id, utils::value(CTParam), v);
		}

		operator const ShaderProgramInfo() const { return ShaderProgramInfo{ _id }; }
		const ShaderProgramInfo getInfo() const { return ShaderProgramInfo{ _id }; }

	protected:
		std::vector<Shader> _shaders;
		std::unordered_map<std::string, ProgramUniform> _uniforms;
		std::unordered_map<std::string, location> _attributes;
		std::unordered_map<std::string, ProgramTextureInfo> _textures;

		DEBUG_ONLY(bool _built = false);
	};
}
