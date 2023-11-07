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

void ShaderProgram::attachShader(Shader& shader)
{
    glAttachShader(*_id, shader);
    _shaders.push_back(shader);

    DEBUG_ONLY(_built = false);
}

bool ShaderProgram::attachSource(ShaderType type, const std::string& src, const std::string& label)
{
    Shader shader(type, label);
    
    if (!shader.setSource(src))
    {
        TRACE("Attaching of " << utils::name(type) << " shader failed :\n" << shader.getLog());
        return false;
    }
    
    attachShader(shader);

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

    location unit = -1;
    if(autobind)
    {
        unit = findUniform(name);
        if (unit > -1)
            uniform(name, unit);
    }
    _textures[name] = ProgramTextureInfo{ tex, unit, autobind };
}

location ShaderProgram::findUniform(const std::string &name)
{
    ASSERT(_built);

    auto it = _uniforms.find(name);
    if (it != _uniforms.end())
        return it->second.loc;

    return glGetUniformLocation(*_id, name.c_str());
}

location ShaderProgram::findAttribute(const std::string &name)
{
    ASSERT(_built);

    auto it = _attributes.find(name);
    if(it != _attributes.end())
        return it->second;
       
    location loc = glGetAttribLocation(*_id, name.c_str());
    if (loc > -1)
        _attributes[name] = loc;
    return loc;
}
