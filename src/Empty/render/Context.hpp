#pragma once

#include "Empty/glad/glad.h"
#include <GLFW/glfw3.h> // GLFW needs to be included after glad

#include "Empty/utils/macros.h"

namespace render
{
	struct Context
	{
		GLFWwindow* window;
		int frameWidth, frameHeight;

		Context(const char* title, int width, int height, int major = 3, int minor = 3);

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
