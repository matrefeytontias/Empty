#include "Empty/render/Context.hpp"

#include "Empty/render/gl/Buffer.h"
#include "Empty/render/gl/Framebuffer.h"
#include "Empty/render/gl/ShaderProgram.hpp"
#include "Empty/render/gl/Texture.h"
#include "Empty/render/gl/VertexArray.h"

void render::Context::debugMessageCallback(DebugCallback callback, const void* userData)
{
	_debugCallbackPayload.callback = callback;
	_debugCallbackPayload.userData = userData;
	glDebugMessageCallback(_bypassDebugCallback, &_debugCallbackPayload);
}

void render::Context::bind(const gl::BufferInfo& b, gl::BufferTarget target) const
{
	glBindBuffer(utils::value(target), *b.id);
}

void render::Context::bind(const gl::BufferInfo& b, gl::IndexedBufferTarget target, int index) const
{
	glBindBufferBase(utils::value(target), index, *b.id);
}

void render::Context::bind(const gl::BufferInfo& b, gl::IndexedBufferTarget target, int index, size_t offset, size_t size) const
{
	glBindBufferRange(utils::value(target), index, *b.id, offset, size);
}

void render::Context::setFramebuffer(const gl::FramebufferInfo& fb, gl::FramebufferTarget target, int width, int height, int x, int y) const
{
	glBindFramebuffer(utils::value(target), *fb.id);
	glViewport(x, y, width, height);
}

void render::Context::bind(const gl::TextureInfo& t, int unit) const
{
	glBindTextureUnit(unit, *t.id);
}

void render::Context::bind(const gl::TextureLevelInfo& t, int unit, gl::AccessPolicy access, gl::TextureFormat writeFormat) const
{
	glBindImageTexture(unit, *t.id, t.level, t.layered, t.layer, utils::value(access), utils::value(writeFormat));
}

void render::Context::bind(const gl::VertexArrayInfo& va) const
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

	// Bind autobind textures
	auto shaderTextures = sp.dumpTextures();
	for (const auto& st : shaderTextures)
		if(st.autobind)
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
