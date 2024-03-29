#include "Empty/Context.hpp"

#include "Empty/gl/Buffer.h"
#include "Empty/gl/Framebuffer.h"
#include "Empty/gl/ShaderProgram.hpp"
#include "Empty/gl/Texture.h"
#include "Empty/gl/VertexArray.h"

namespace Empty
{
	void Context::debugMessageCallback(DebugCallback callback, const void* userData)
	{
		_debugCallbackPayload.callback = callback;
		_debugCallbackPayload.userData = userData;
		glDebugMessageCallback(_bypassDebugCallback, &_debugCallbackPayload);
	}

	void Context::bind(const gl::BufferInfo& b, gl::BufferTarget target) const
	{
		glBindBuffer(utils::value(target), b);
	}

	void Context::bind(const gl::BufferInfo& b, gl::IndexedBufferTarget target, int index) const
	{
		glBindBufferBase(utils::value(target), index, b);
	}

	void Context::bind(const gl::BufferInfo& b, gl::IndexedBufferTarget target, int index, size_t offset, size_t size) const
	{
		glBindBufferRange(utils::value(target), index, b, offset, size);
	}

	void Context::setFramebuffer(const gl::FramebufferInfo& fb, gl::FramebufferTarget target, int width, int height, int x, int y) const
	{
		glBindFramebuffer(utils::value(target), fb);
		glViewport(x, y, width, height);
	}

	void Context::bind(const gl::TextureInfo& t, int unit) const
	{
		glBindTextureUnit(unit, t);
	}

	void Context::bind(const gl::TextureLevelInfo& t, int unit, gl::AccessPolicy access, gl::TextureFormat writeFormat) const
	{
		glBindImageTexture(unit, *t.id, t.level, t.layered, t.layer, utils::value(access), utils::value(writeFormat));
	}

	void Context::bind(const gl::VertexArrayInfo& va) const
	{
		glBindVertexArray(va);
	}

	void Context::setShaderProgram(const gl::ShaderProgram& sp)
	{
		gl::ShaderProgramInfo spi = sp;

		// Use program
		if (_currentProgram.lock() != spi.id)
		{
			glUseProgram(spi);
			_currentProgram = spi.id;
		}

		// Bind autobind textures
		auto shaderTextures = sp.dumpTextures();
		for (const auto& st : shaderTextures)
			if (st.autobind && st.unit > -1)
				bind(st.textureInfo, st.unit);
	}
}
