#pragma once

#include "glad/glad.h"

#include "Empty/render/gl/GLEnums.hpp"
#include "Empty/utils/EnumBitfield.h"
#include "Empty/utils/macros.h"
#include "Empty/utils/utils.hpp"

namespace render
{

	namespace gl
	{
		struct ProgramId;

		struct BufferInfo;
		struct FramebufferInfo;
		struct TextureInfo;
		struct VertexArrayInfo;

		struct ShaderProgram;
	}

	/**
	 * Structure used to carry general GL operations, not related to a specific GL object.
	 * `Context` has to be inherited by a structure that implements the `swap` method to
	 * be able to be instantiated, ideally also providing a window and GL context.
	 */
	struct Context
	{
		int frameWidth, frameHeight;
		// Color to clear color buffers with. Defaults to (0, 0, 0, 1).
		math::vec4 clearColor;
		// Depth value to clear depth buffers with. Defaults to 0.f.
		float clearDepth;
		// Stencil value to clear stencil buffers with. Defaults to 0.
		unsigned int clearStencil;

		Context(int width, int height, int major = 4, int minor = 5) : frameWidth(width), frameHeight(height), clearColor(0, 0, 0, 1), clearDepth(0.f), clearStencil(0) { }
		~Context() = default;

		/**
		 * Bind a Buffer to the context.
		 */
		void bind(const gl::BufferInfo& b, gl::BufferTarget target);
		/**
		 * Bind a Buffer to an indexed target. Note this also binds the buffer on the
		 * generic binding point of the target, like the non-indexed `bind` method would.
		 */
		void bind(const gl::BufferInfo& b, gl::IndexedBufferTarget target, int index);
		/**
		 * Bind part of a Buffer to an indexed target. Note this also binds the buffer on
		 * the generic binding point of the target, like the non-indexed `bind` method would.
		 */
		void bind(const gl::BufferInfo& b, gl::IndexedBufferTarget target, int index, size_t offset, size_t size);
		/**
		 * Bind a Texture to the context.
		 */
		void bind(const gl::TextureInfo& t, int unit);
		/**
		 * Bind a VertexArray to the context.
		 */
		void bind(const gl::VertexArrayInfo& va);

		/**
		 * Bind a Framebuffer to the context and set the viewport.
		 */
		void setFramebuffer(const gl::FramebufferInfo& fb, gl::FramebufferTarget target, int width, int height, int x = 0, int y = 0);

		/**
		 * Set the active ShaderProgram.
		 */
		void setShaderProgram(const gl::ShaderProgram& sp);

		/**
		 * Set the viewport dimensions.
		 */
		void setViewport(int width, int height, int x = 0, int y = 0) { glViewport(x, y, width, height); }

		/**
		 * Reset the viewport dimensions to cover the entire default framebuffer.
		 */
		void resetViewport() { glViewport(0, 0, frameWidth, frameHeight); }

		/**
		 * Draw a range of vertices from a bound VertexArray, arranged in successive primitives.
		 */
		void drawArrays(gl::PrimitiveType type, int first, int count) const { glDrawArrays(utils::value(type), first, count); }

		/**
		 * Draw a range of vertices from a bound VertexArray, arranged in primitives indexed by a
		 * the VertexArray's attached element buffer.
		 */
		void drawElements(gl::PrimitiveType type, gl::ElementType elemType, int first, int count) const
		{
			glDrawElements(utils::value(type), count, utils::value(elemType), reinterpret_cast<void*>(static_cast<size_t>(first)));
		}

		/**
		 * Clear a variety of drawing buffers with the corresponding colors.
		 */
		void clearBuffer(gl::DrawBufferType buffer) const;

		void memoryBarrier(utils::EnumBitfield<gl::MemoryBarrierType> barriers) { glMemoryBarrier(barriers); }

		/**
		 * Swaps drawing buffers, essentially refreshing the screen.
		 */
		virtual void swap() const = 0;

	private:
		/// State keeping
		
		// Currently bound shader program
		std::weak_ptr<gl::ProgramId> _currentProgram;
	};
}
