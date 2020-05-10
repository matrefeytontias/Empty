#include <iostream>
#include <unordered_map>

#include "render/gl/ShaderProgram.hpp"
#include "utils/utils.hpp"

using namespace render::gl;

ShaderProgram* render::gl::ShaderProgram::_currentProgram = nullptr;

ShaderProgram::~ShaderProgram()
{
    for(auto cpl : _attributes)
        glDisableVertexAttribArray(cpl.second);
    
    for(auto& shader : _shaders)
        glDetachShader(*_id, *shader);
}

void ShaderProgram::attachSource(ShaderType type, const std::string& src)
{
    auto shader = std::make_shared<ShaderId>(type);
    auto ind = src.c_str();
    glShaderSource(*shader, 1, &ind, NULL);
    glCompileShader(*shader);
    utils::printShaderLog(*shader);
    glAttachShader(*_id, *shader);
    _shaders.push_back(shader);
    _dirty = true;
}

void ShaderProgram::use()
{
    if(_dirty)
        glLinkProgram(*_id);
    if(_currentProgram != this)
        glUseProgram(*_id);
    if(_dirty || _currentProgram != this)
    {
        int i = 0;
        for(auto tex : _textures)
        {
            tex.second.bind(i);
            glUniform1i(ensureUniform(tex.first), i++);
        }
    }
    _dirty = false;
    _currentProgram = this;
}

void ShaderProgram::vertexAttribPointer(const std::string &name, GLuint size, GLenum type, GLsizei stride, const size_t offset)
{
    glEnableVertexAttribArray(ensureAttrib(name));
    glVertexAttribPointer(_attributes[name], size, type, GL_FALSE, stride, reinterpret_cast<GLvoid*>(offset));
}

void ShaderProgram::registerTexture(const std::string &name, const TextureBinding& tex)
{
    _dirty = true;
    _textures[name] = tex;
}

GLint ShaderProgram::ensureUniform(const std::string &name)
{
    if(_uniforms.find(name) == _uniforms.end())
        _uniforms[name] = glGetUniformLocation(*_id, name.c_str());
    return _uniforms[name];
}

GLint ShaderProgram::ensureAttrib(const std::string &name)
{
    if(_attributes.find(name) == _attributes.end())
        _attributes[name] = glGetAttribLocation(*_id, name.c_str());
    return _attributes[name];
}
