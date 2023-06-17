#pragma once

#include <memory>

#include "glad/glad.h"
#include "Empty/gl/GLEnums.hpp"
#include "Empty/utils/noncopyable.h"
#include "Empty/utils/utils.hpp"

namespace Empty::gl
{
    using location = GLint;

    /**
     * Encapsulates the ID of an OpenGL buffer object.
     */
    struct BufferId : public utils::noncopyable
    {
        BufferId(const std::string& label)
        {
            glCreateBuffers(1, &_id);
            glObjectLabel(utils::value(ObjectNamespace::Buffer), _id, -1, label.c_str());
        }
        ~BufferId() { glDeleteBuffers(1, &_id); }
        operator GLuint() const { return _id; }
        bool operator==(const BufferId& a) const { return _id == a._id; }
        bool operator!=(const BufferId& a) const { return _id != a._id; }
    private:
        GLuint _id;
    };

    /**
     * Encapsulates the ID of an OpenGL shader program object.
     */
    struct ProgramId : public utils::noncopyable
    {
        ProgramId(const std::string& label)
        {
            _id = glCreateProgram();
            glObjectLabel(utils::value(ObjectNamespace::Program), _id, -1, label.c_str());
        }
        ~ProgramId() { glDeleteProgram(_id); }
        operator GLuint() const { return _id; }
        bool operator==(const ProgramId& a) const { return _id == a._id; }
        bool operator!=(const ProgramId& a) const { return _id != a._id; }
    private:
        GLuint _id;
    };

    /**
     * Encapsulates the ID of an OpenGL texture object.
     */
    struct TextureId : public utils::noncopyable
    {
        TextureId(TextureTarget t, const std::string& label)
        {
            glCreateTextures(utils::value(t), 1, &_id);
            glObjectLabel(utils::value(ObjectNamespace::Texture), _id, -1, label.c_str());
        }
        TextureId()
        {
            glGenTextures(1, &_id);
        }
        ~TextureId() { glDeleteTextures(1, &_id); }
        operator GLuint() const { return _id; }
        bool operator==(const TextureId& a) const { return _id == a._id; }
        bool operator!=(const TextureId& a) const { return _id != a._id; }
    private:
        GLuint _id;
    };

    /**
     * Encapsulates the ID of an OpenGL vertex array object.
     */
    struct VertexArrayId : public utils::noncopyable
    {
        VertexArrayId(const std::string& label)
        {
            glCreateVertexArrays(1, &_id);
            glObjectLabel(utils::value(ObjectNamespace::VertexArray), _id, -1, label.c_str());
        }
        ~VertexArrayId() { glDeleteVertexArrays(1, &_id); }
        operator GLuint() const { return _id; }
        bool operator==(const VertexArrayId& a) const { return _id == a._id; }
        bool operator!=(const VertexArrayId& a) const { return _id != a._id; }
    private:
        GLuint _id;
    };

    /**
     * Encapsulates the ID of an OpenGL shader object.
     */
    struct ShaderId : public utils::noncopyable
    {
        ShaderId(ShaderType type, const std::string& label)
        {
            _id = glCreateShader(utils::value(type));
            glObjectLabel(utils::value(ObjectNamespace::Shader), _id, -1, label.c_str());
        }
        ~ShaderId() { glDeleteShader(_id); }
        operator GLuint() const { return _id; }
        bool operator==(const ShaderId& a) { return _id == a._id; }
        bool operator!=(const ShaderId& a) { return _id != a._id; }
    private:
        GLuint _id;
    };

    /**
     * Encapsulates the ID of an OpenGL framebuffer object.
     */
    struct FramebufferId : public utils::noncopyable
    {
        FramebufferId(const std::string& label)
        {
            glCreateFramebuffers(1, &_id);
            glObjectLabel(utils::value(ObjectNamespace::Framebuffer), _id, -1, label.c_str());
        }
        // For existing resources
        FramebufferId(GLuint id)
        {
            _id = id;
        }
        ~FramebufferId() { glDeleteFramebuffers(1, &_id); }
        operator GLuint() const { return _id; }
        bool operator==(const FramebufferId& a) const { return _id == a._id; }
        bool operator!=(const FramebufferId& a) const { return _id != a._id; }
    private:
        GLuint _id;
    };

    /**
     * Encapsulates the ID of an OpenGL renderbuffer object.
     */
    struct RenderbufferId : public utils::noncopyable
    {
        RenderbufferId(const std::string& label)
        {
            glCreateRenderbuffers(1, &_id);
            glObjectLabel(utils::value(ObjectNamespace::Renderbuffer), _id, -1, label.c_str());
        }
        ~RenderbufferId() { glDeleteRenderbuffers(1, &_id); }
        operator GLuint() const { return _id; }
        bool operator==(const RenderbufferId& a) const { return _id == a._id; }
        bool operator!=(const RenderbufferId& a) const { return _id != a._id; }
    private:
        GLuint _id;
    };

	/**
     * Base class for GL objects ; just hold and construct an ID of a given type.
     */
	template <typename Id> struct GLObject
	{
        template <typename ...Ts>
		GLObject(const Ts& ...args) : _id(std::make_shared<Id>(args...)) {}
		bool operator==(const GLObject& a) { return _id == a._id; }
        bool operator!=(const GLObject& a) { return _id != a._id; }
	protected:
		std::shared_ptr<Id> _id;
	};
}
