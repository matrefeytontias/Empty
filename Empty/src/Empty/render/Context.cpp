#include "Empty/render/Context.hpp"

#include "Empty/render/gl/Buffer.h"
#include "Empty/render/gl/Framebuffer.h"
#include "Empty/render/gl/ShaderProgram.hpp"
#include "Empty/render/gl/Texture.h"
#include "Empty/render/gl/VertexArray.h"

render::Context::Context(const char* title, int width, int height, int major, int minor) : frameWidth(width), frameHeight(height), clearColor(0, 0, 0, 1), clearDepth(0.f), clearStencil(0)
{
	ASSERT(major * 100 + minor >= 405);
	glfwSetErrorCallback(errorCallback);
	if (!glfwInit())
		FATAL("Could not initialize GLFW. Aborting");
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	window = glfwCreateWindow(frameWidth, frameHeight, title, NULL, NULL);
	glfwMakeContextCurrent(window);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	glfwSwapInterval(1);

	gladLoadGLLoader((GLADloadproc) glfwGetProcAddress);
}

render::Context::~Context()
{
	glfwTerminate();
}

void render::Context::bind(const gl::BufferInfo& b, gl::BufferTarget target)
{
	glBindBuffer(utils::value(target), *b.id);
}

void render::Context::bind(const gl::BufferInfo& b, gl::IndexedBufferTarget target, int index)
{
	glBindBufferBase(utils::value(target), index, *b.id);
}

void render::Context::bind(const gl::BufferInfo& b, gl::IndexedBufferTarget target, int index, size_t offset, size_t size)
{
	glBindBufferRange(utils::value(target), index, *b.id, offset, size);
}

void render::Context::setFramebuffer(const gl::FramebufferInfo& fb, gl::FramebufferTarget target, int width, int height, int x, int y)
{
	glBindFramebuffer(utils::value(target), *fb.id);
	glViewport(x, y, width, height);
}

void render::Context::bind(const gl::TextureInfo& t, int unit)
{
	glBindTextureUnit(unit, *t.id);
}

void render::Context::bind(const gl::VertexArrayInfo& va)
{
	glBindVertexArray(*va.id);
}

void render::Context::setShaderProgram(const gl::ShaderProgram& sp)
{
	gl::ShaderProgramInfo spi = sp;

	// Use program
	if (_currentProgram.lock() != spi.id)
	{
		glUseProgram(*spi.id);
		_currentProgram = spi.id;
	}

	// Bind textures
	auto shaderTextures = sp.dumpTextures();
	for (auto st : shaderTextures)
		bind(st.textureInfo, st.unit);
}

void render::Context::clearBuffer(gl::DrawBufferType buffer) const
{
	if (buffer == gl::DrawBufferType::Color)
		glClearNamedFramebufferfv(0, GL_COLOR, 0, clearColor);
	else if (buffer == gl::DrawBufferType::Depth)
		glClearNamedFramebufferfv(0, GL_DEPTH, 0, &clearDepth);
	else if (buffer == gl::DrawBufferType::Stencil)
		glClearNamedFramebufferuiv(0, GL_STENCIL, 0, &clearStencil);
	else // if (buffer == gl::DrawBufferType::DepthStencil)
		glClearNamedFramebufferfi(0, GL_DEPTH_STENCIL, 0, clearDepth, clearStencil);
}
