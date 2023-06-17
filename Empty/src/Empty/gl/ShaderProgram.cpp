#include <iostream>
#include <unordered_map>

#include "Empty/gl/ShaderProgram.hpp"
#include "Empty/utils/utils.hpp"

using namespace Empty::gl;

ShaderProgram::~ShaderProgram()
{
    for (const auto& shader : _shaders)
        glDetachShader(*_id, shader);
}

bool ShaderProgram::attachSource(ShaderType type, const std::string& src, const std::string& label)
{
    Shader shader(type, label);
    
    if (!shader.setSource(src))
    {
        TRACE("Attaching of " << utils::name(type) << " shader failed :\n" << shader.getLog());
        return false;
    }
    glAttachShader(*_id, shader);
    _shaders.push_back(shader);

    DEBUG_ONLY(_built = false);

    return true;
}

void ShaderProgram::locateAttributes(VertexStructure& vStruct)
{
    ASSERT(_built);

    for (auto& d : vStruct.descriptors)
        d.index = findAttribute(d.name);
}

std::vector<ProgramUniform> ShaderProgram::dumpUniforms() const
{
    ASSERT(_built);

    std::vector<ProgramUniform> r;
    r.reserve(_uniforms.size());
    for (const auto& u : _uniforms)
        r.push_back(u.second);
    return r;
}

std::vector<ProgramTextureInfo> ShaderProgram::dumpTextures() const
{
    ASSERT(_built);

    std::vector<ProgramTextureInfo> r;
    r.reserve(_textures.size());
    for (const auto& t : _textures)
        r.push_back(t.second);
    return r;
}

void ShaderProgram::registerTexture(const std::string &name, const TextureInfo& tex, bool autobind)
{
    ASSERT(_built);

    location unit = glGetUniformLocation(*_id, name.c_str());
    uniform(name, unit);
    _textures[name] = ProgramTextureInfo{ tex, unit, autobind };
}

location ShaderProgram::findUniform(const std::string &name)
{
    ASSERT(_built);

    auto uniform = _uniforms.find(name);
    if (uniform == _uniforms.end())
        return -1;
    else if(uniform->second.loc < 0)
        return uniform->second.loc = glGetUniformLocation(*_id, name.c_str());
    return uniform->second.loc;
}

location ShaderProgram::findAttribute(const std::string &name)
{
    ASSERT(_built);

    if(_attributes.find(name) == _attributes.end())
        _attributes[name] = glGetAttribLocation(*_id, name.c_str());
    return _attributes[name];
}
