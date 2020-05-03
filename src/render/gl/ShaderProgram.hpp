#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <glad/glad.h>

#include "render/gl/Texture.hpp"
#include "utils/utils.hpp"

namespace render::gl
{
    struct ShaderProgram
    {
    public:
        ShaderProgram();
        ~ShaderProgram();
        void attach(GLenum type, const char* path);
        void attachFromSource(GLenum type, const char* src);
        void use();
        void uniform1f(const std::string& name, float v);
        void uniform2f(const std::string& name, float v1, float v2);
        void uniform3f(const std::string& name, float v1, float v2, float v3);
        void uniform4f(const std::string& name, float v1, float v2, float v3, float v4);
        void uniform1i(const std::string& name, int i);
        void uniform2i(const std::string& name, int i1, int i2);
        void uniformMatrix4fv(const std::string& name, GLuint count, const GLfloat* v);
        void uniformMatrix3fv(const std::string& name, GLuint count, const GLfloat* v);
        void vertexAttribPointer(const std::string& name, GLuint size, GLenum type, GLsizei stride, const size_t offset);
        template <TextureTarget target, TextureFormat format>
        void registerTexture(const std::string& name, const Texture<target, format>& tex);
        size_t getTexturesAmount() const { return _textures.size(); }
        GLint ensureUniform(const std::string& name);
        GLint ensureAttrib(const std::string& name);
    private:
        bool _dirty;
        GLuint _vao, _program;
        std::vector<GLenum> _shaders;
        std::unordered_map<std::string, GLint> _uniforms;
        std::unordered_map<std::string, GLint> _attributes;
        std::unordered_map<std::string, TextureBinding> _textures;
        static GLuint _currentProgram;
    };
}
