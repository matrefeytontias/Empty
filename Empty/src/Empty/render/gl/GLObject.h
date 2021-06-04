#pragma once

#include <memory>

#include "glad/glad.h"
#include "Empty/render/gl/GLEnums.hpp"
#include "Empty/utils/noncopyable.h"
#include "Empty/utils/utils.hpp"

namespace render::gl
{
    using location = GLint;

    struct BufferId : public utils::noncopyable
    {
        BufferId() { glCreateBuffers(1, &_id); }
        ~BufferId() { glDeleteBuffers(1, &_id); }
        operator GLuint() const { return _id; }
        bool operator==(const BufferId& a) const { return _id == a._id; }
    private:
        GLuint _id;
    };

    struct ProgramId : public utils::noncopyable
    {
        ProgramId() { _id = glCreateProgram(); }
        ~ProgramId() { glDeleteProgram(_id); }
        operator GLuint() const { return _id; }
        bool operator==(const ProgramId& a) const { return _id == a._id; }
    private:
        GLuint _id;
    };

    struct TextureId : public utils::noncopyable
    {
        TextureId(TextureTarget t) { glCreateTextures(utils::value(t), 1, &_id); }
        ~TextureId() { glDeleteTextures(1, &_id); }
        operator GLuint() const { return _id; }
        bool operator==(const TextureId& a) const { return _id == a._id; }
    private:
        GLuint _id;
    };

    struct VertexArrayId : public utils::noncopyable
    {
        VertexArrayId() { glCreateVertexArrays(1, &_id); }
        ~VertexArrayId() { glDeleteVertexArrays(1, &_id); }
        operator GLuint() const { return _id; }
        bool operator==(const VertexArrayId& a) const { return _id == a._id; }
    private:
        GLuint _id;
    };

    struct ShaderId : public utils::noncopyable
    {
        ShaderId(ShaderType type) { _id = glCreateShader(utils::value(type)); }
        ~ShaderId() { glDeleteShader(_id); }
        operator GLuint() const { return _id; }
        bool operator==(const ShaderId& a) { return _id == a._id; }
    private:
        GLuint _id;
    };

    struct FramebufferId : public utils::noncopyable
    {
        FramebufferId() { glCreateFramebuffers(1, &_id); }
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
	protected:
		std::shared_ptr<Id> _id;
	};
}
