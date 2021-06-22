#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h> // GLFW needs to be included after glad

#include "Empty/render/gl/GLEnums.hpp"
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
	 */
	struct Context
	{
		GLFWwindow* window;
		int frameWidth, frameHeight;
		// Color to clear color buffers with. Defaults to (0, 0, 0, 1).
		math::vec4 clearColor;
		// Depth value to clear depth buffers with. Defaults to 0.f.
		float clearDepth;
		// Stencil value to clear stencil buffers with. Defaults to 0.
		unsigned int clearStencil;

		Context(const char* title, int width, int height, int major = 4, int minor = 5);
		~Context();

		/**
		 * Bind a buffer to the context.
		 */
		void bind(const gl::BufferInfo& b, gl::BufferTarget target);
		/**
		 * Bind a framebuffer to the context.
		 */
		void bind(const gl::FramebufferInfo& fb, gl::FramebufferTarget target);
		/**
		 * Bind a texture to the context.
		 */
		void bind(const gl::TextureInfo& t, int unit);
		/**
		 * Bind a vertex array to the context.
		 */
		void bind(const gl::VertexArrayInfo& va);

		/**
		 * Set the active shader program.
		 */
		void setShaderProgram(const gl::ShaderProgram& sp);

		/**
		 * Draw a range of vertices from a bound array buffer, arranged in successive primitives.
		 */
		void drawArrays(gl::PrimitiveType type, int first, int count) const { glDrawArrays(utils::value(type), first, count); }

		/**
		 * Draw a range of vertices from a bound array buffer, arranged in primitives indexed by a range from a bound element array buffer.
		 */
		void drawElements(gl::PrimitiveType type, gl::ElementType elemType, int first, int count) const
		{
			glDrawElements(utils::value(type), count, utils::value(elemType), reinterpret_cast<void*>(static_cast<size_t>(first)));
		}

		/**
		 * Clear a variety of buffers with the corresponding colors.
		 */
		void clearBuffers(gl::DrawBufferType buffer) const
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

		void swap() const { glfwSwapBuffers(window); }

	private:
		static void errorCallback(int error, const char* description)
		{
			std::cerr << "GLFW error #" << error << " : " << description << std::endl;
		}

		static void keyCallback(GLFWwindow* window, int key, int scanCode, int action, int mods)
		{
			if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
				glfwSetWindowShouldClose(window, true);
		}

		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
		{}

		/// State keeping
		
		// Currently bound shader program
		std::weak_ptr<gl::ProgramId> _currentProgram;
	};
}
