#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include <glad/glad.h>

#include "render/gl/GLObject.h"
#include "render/gl/Texture.h"
#include "utils/utils.hpp"

namespace render::gl
{
    struct ShaderProgram : public GLObject<ProgramId>
    {
    public:
        ShaderProgram() {}
        ~ShaderProgram();
        void attachSource(ShaderType type, const std::string& src);
        inline void attachFile(ShaderType type, const std::string& path)
        {
            attachSource(type, utils::getFileContents(path));
        }
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
        void registerTexture(const std::string& name, const TextureBinding& tex);
        size_t getTexturesAmount() const { return _textures.size(); }
        GLint ensureUniform(const std::string& name);
        GLint ensureAttrib(const std::string& name);
    private:
        bool _dirty = true;
        std::vector<std::shared_ptr<ShaderId>> _shaders;
        std::unordered_map<std::string, GLint> _uniforms;
        std::unordered_map<std::string, GLint> _attributes;
        std::unordered_map<std::string, TextureBinding> _textures;
        static ShaderProgram* _currentProgram;
    };
}
