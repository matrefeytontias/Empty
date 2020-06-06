#pragma once

#include "Empty/glad/glad.h"
#include <GLFW/glfw3.h> // GLFW needs to be included after glad

#include "Empty/render/gl/GLEnums.hpp"
#include "Empty/utils/macros.h"
#include "Empty/utils/utils.hpp"

namespace render
{
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
		int clearStencil;

		Context(const char* title, int width, int height, int major = 3, int minor = 3);
		~Context();

		/**
		 * Draw a range of vertices from a bound array buffer, arranged in successive primitives.
		 */
		inline void drawArrays(gl::PrimitiveType type, int first, int count) const { glDrawArrays(utils::value(type), first, count); }

		/**
		 * Draw a range of vertices from a bound array buffer, arranged in primitives indexed by a range from a bound element array buffer.
		 */
		inline void drawElements(gl::PrimitiveType type, gl::ElementType elemType, int first, int count) const
		{
			glDrawElements(utils::value(type), count, utils::value(elemType), reinterpret_cast<void*>(static_cast<size_t>(first)));
		}

		/**
		 * Clear a variety of buffers with the corresponding colors.
		 */
		inline void clearBuffers(bool color = true, bool depth = false, bool stencil = false) const
		{
			glClear(GL_COLOR_BUFFER_BIT * color | GL_DEPTH_BUFFER_BIT * depth | GL_STENCIL_BUFFER_BIT * stencil);
		}

		inline void swap() const { glfwSwapBuffers(window); }

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
	};
}
