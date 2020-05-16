#pragma once

#include <memory>

#include <glad/glad.h>

#include "render/gl/GLEnums.hpp"
#include "utils/noncopyable.h"
#include "utils/utils.hpp"

namespace render::gl
{
	// Generate a non-copyable ID struct for OpenGL resources created with glGen-type functions.
#define GL_ID_STRUCT_GEN(name, suffix)                          \
    struct name : public utils::noncopyable                     \
    {                                                           \
        name() { glGen##suffix(1, &_id); }                      \
        ~name() { glDelete##suffix(1, &_id); }                  \
        operator GLuint() const { return _id; }                 \
        bool operator==(const name& a) { return _id == a._id; } \
    private:                                                    \
        GLuint _id;                                             \
    }

	// Generate a non-copyable ID struct for OpenGL resources created with glCreate-type functions.
#define GL_ID_STRUCT_CREATE(name, suffix)                       \
    struct name : public utils::noncopyable                     \
    {                                                           \
        name() { _id = glCreate##suffix(); }                    \
        ~name() { glDelete##suffix(_id); }                      \
        operator GLuint() const { return _id; }                 \
        bool operator==(const name& a) { return _id == a._id; } \
    private:                                                    \
        GLuint _id;                                             \
    }

	GL_ID_STRUCT_GEN(BufferId, Buffers);
	GL_ID_STRUCT_CREATE(ProgramId, Program);
	GL_ID_STRUCT_GEN(TextureId, Textures);
    GL_ID_STRUCT_GEN(VertexArrayId, VertexArrays);

    struct ShaderId : public utils::noncopyable
    {
        ShaderId(ShaderType type) { _id = glCreateShader(utils::value(type)); }
        ~ShaderId() { glDeleteShader(_id); }
        operator GLuint() const { return _id; }
        bool operator==(const ShaderId& a) { return _id == a._id; }
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
