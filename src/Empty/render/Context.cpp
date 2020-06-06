#include "Empty/render/Context.hpp"

render::Context::Context(const char* title, int width, int height, int major, int minor) : frameWidth(width), frameHeight(height)
{
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