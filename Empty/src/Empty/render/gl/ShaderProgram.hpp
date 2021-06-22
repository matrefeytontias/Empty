#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <type_traits>

#include "Empty/math/vec.h"
#include "Empty/render/gl/GLObject.h"
#include "Empty/render/gl/Shader.h"
#include "Empty/render/gl/Texture.h"
#include "Empty/render/gl/Uniform.h"
#include "Empty/render/gl/VertexStructure.h"
#include "Empty/utils/utils.hpp"

namespace render::gl
{
	struct ShaderProgramInfo
	{
		std::shared_ptr<ProgramId> id;
	};

	/**
	 * Holds information about a uniform in the context of
	 * a shader program.
	 */
	struct ProgramUniform
	{
		std::shared_ptr<UniformBase> uniform;
		location loc = -1;
	};

	/**
	 * Holds information about a texture in the context of
	 * a shader program.
	 */
	struct ProgramTextureInfo
	{
		TextureInfo textureInfo;
		int unit = -1;
	};

	struct ShaderProgram : public GLObject<ProgramId>
	{
	public:
		ShaderProgram() {}
		~ShaderProgram();

		bool attachSource(ShaderType type, const std::string& src);
		inline bool attachFile(ShaderType type, const std::string& path)
		{
			return attachSource(type, utils::getFileContents(path));
		}

		bool build()
		{
			glLinkProgram(*_id);
			return DEBUG_ONLY(_built = ) getParameter<ProgramParam::LinkStatus>();
		}

		/**
		 * Finds and stores attribute locations in the provided vertex structure in-place.
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
		 * Sets a uniform's value. The type is deduced from the value's
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

		std::vector<ProgramUniform> dumpUniforms() const;
		std::vector<ProgramTextureInfo> dumpTextures() const;

		void registerTexture(const std::string& name, const TextureInfo& tex);
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
