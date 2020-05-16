#pragma once

#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include <type_traits>

#include <glad/glad.h>

#include "math/Vector.hpp"
#include "render/gl/GLObject.h"
#include "render/gl/Shader.h"
#include "render/gl/Texture.h"
#include "render/gl/Uniform.h"
#include "utils/utils.hpp"

namespace render::gl
{
	typedef GLint location;

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
		void use();

		template <typename T>
		void uniform(const std::string& name, const T& value)
		{
			if (_uniforms.find(name) == _uniforms.end())
				_uniforms.emplace(name, std::make_shared<Uniform<T>>(name, value));
			_uniforms[name]->upload(findUniform(name));
		}
		std::vector<std::shared_ptr<UniformBase>> dumpUniforms() const;

		void registerTexture(const std::string& name, const TextureBinding& tex);
		size_t getTexturesAmount() const { return _textures.size(); }

		location findUniform(const std::string& name);
		location findAttribute(const std::string& name);

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
			inline math::Vector3i getParameter() const
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

	protected:

		bool _dirty = true;
		std::vector<Shader> _shaders;
		std::unordered_map<std::string, location> _uniformLocations;
		std::unordered_map<std::string, std::shared_ptr<UniformBase>> _uniforms;
		std::unordered_map<std::string, location> _attributes;
		std::unordered_map<std::string, TextureBinding> _textures;
		static ShaderProgram* _currentProgram;
	};
}
