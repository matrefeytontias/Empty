#include <iostream>
#include <unordered_map>

#include "Empty/render/gl/ShaderProgram.hpp"
#include "Empty/utils/utils.hpp"

using namespace render::gl;

ShaderProgram* render::gl::ShaderProgram::_currentProgram = nullptr;

ShaderProgram::~ShaderProgram()
{
    for (const auto& shader : _shaders)
        glDetachShader(*_id, shader);
}

bool ShaderProgram::attachSource(ShaderType type, const std::string& src)
{
    Shader shader(type);
    if (!shader.setSource(src))
        return false;
    glAttachShader(*_id, shader);
    _shaders.push_back(shader);
    _dirty = true;
    return true;
}

void ShaderProgram::use()
{
    if(_dirty)
        glLinkProgram(*_id);
    if(_currentProgram != this)
        glUseProgram(*_id);
    if (_dirty || _currentProgram != this)
        updateTextureUniforms();
    _dirty = false;
    _currentProgram = this;
}

void ShaderProgram::locateAttributes(VertexStructure& vStruct)
{
    for (auto& d : vStruct.descriptors)
        d.index = findAttribute(d.name);
}

std::vector<std::shared_ptr<UniformBase>> ShaderProgram::dumpUniforms() const
{
    std::vector<std::shared_ptr<UniformBase>> r;
    r.reserve(_uniforms.size());
    for (const auto& u : _uniforms)
        r.push_back(u.second);
    return r;
}

void ShaderProgram::registerTexture(const std::string &name, const TextureBinding& tex)
{
    _dirty = true;
    _textures[name] = tex;
    if (_currentProgram == this)
        updateTextureUniforms();
    // if not, this will be called by the next call of use()
}

GLint ShaderProgram::findUniform(const std::string &name)
{
    if(_uniformLocations.find(name) == _uniformLocations.end())
        _uniformLocations[name] = glGetUniformLocation(*_id, name.c_str());
    return _uniformLocations[name];
}

GLint ShaderProgram::findAttribute(const std::string &name)
{
    if(_attributes.find(name) == _attributes.end())
        _attributes[name] = glGetAttribLocation(*_id, name.c_str());
    return _attributes[name];
}

void ShaderProgram::updateTextureUniforms()
{
    int i = 0;
    for (const auto& tex : _textures)
    {
        tex.second.bind(i);
        glUniform1i(findUniform(tex.first), i++);
    }
}
