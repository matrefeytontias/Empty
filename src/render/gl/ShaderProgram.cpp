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
            glActiveTexture(GL_TEXTURE0 + i);
            tex.second.bind();
            glUniform1i(ensureUniform(tex.first), i++);
        }
    }
    _dirty = false;
    _currentProgram = this;
}

void ShaderProgram::uniform1f(const std::string &name, float v)
{
    glUniform1f(ensureUniform(name), v);
}

void ShaderProgram::uniform2f(const std::string &name, float v1, float v2)
{
    glUniform2f(ensureUniform(name), v1, v2);
}

void ShaderProgram::uniform3f(const std::string &name, float v1, float v2, float v3)
{
    glUniform3f(ensureUniform(name), v1, v2, v3);
}

void ShaderProgram::uniform4f(const std::string &name, float v1, float v2, float v3, float v4)
{
    glUniform4f(ensureUniform(name), v1, v2, v3, v4);
}

void ShaderProgram::uniform1i(const std::string &name, int i)
{
    glUniform1i(ensureUniform(name), i);
}

void ShaderProgram::uniform2i(const std::string &name, int i1, int i2)
{
    glUniform2i(ensureUniform(name), i1, i2);
}

void ShaderProgram::uniformMatrix4fv(const std::string &name, GLuint count, const GLfloat *v)
{
    glUniformMatrix4fv(ensureUniform(name), count, GL_FALSE, v);
}

void ShaderProgram::uniformMatrix3fv(const std::string &name, GLuint count, const GLfloat *v)
{
    glUniformMatrix3fv(ensureUniform(name), count, GL_FALSE, v);
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
