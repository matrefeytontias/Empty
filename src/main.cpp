#include <iostream>
#include <stdexcept>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "math/Vector.hpp"
#include "render/gl/Buffer.h"
#include "render/gl/ShaderProgram.hpp"
#include "render/gl/Texture.h"
#include "render/gl/VertexArray.h"
#include "utils/macros.h"
#include "utils/utils.hpp"

static void glfw_error_callback(int error, const char *description)
{
    TRACE("Error " << error << " : " << description);
}

void keyCallback(GLFWwindow *window, int key, int scanCode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
}

using namespace render::gl;

int _main(int, char *argv[])
{
    utils::setwd(argv);
    
    // Setup window
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
    {
        TRACE("Couldn't initialize GLFW");
        return 1;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow *window = glfwCreateWindow(1280, 720, "GLFW Window", NULL, NULL);
    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    glfwSwapInterval(1); // Enable vsync
    
    glfwSetKeyCallback(window, keyCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    
    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    
    TRACE("Entering drawing loop");

    VertexArray vao;
    vao.bind();

    Texture<TextureTarget::Texture2D, TextureFormat::RGBA8> tex;
    tex.bind();
    tex.setParameter<TextureParam::MinFilter>(TextureParamValue::FilterLinear);
    TRACE("Texture default mag filter is " << utils::name(tex.getParameter<TextureParam::MagFilter>()));
    tex.uploadData(0, 64, 64, PixelFormat::RGBA, PixelType::Byte, nullptr);
    tex.unbind();

    Buffer<BufferTarget::Array> buffer;
    buffer.bind();
    buffer.uploadData(10, BufferUsage::DynamicRead);
    int64_t size = buffer.getParameter<BufferParam::Size>();
    TRACE("Successfully reserved " << size << " bytes for buffer");

    BufferMapping mapping = buffer.map(BufferAccess::ReadOnly);
    mapping.unmap();

    buffer.unbind();

    ShaderProgram program;
    program.attachSource(ShaderType::Fragment, "uniform float uTime; void main() { gl_FragColor = vec4(fract(uTime), 0, 0, 1); }");
    program.use();
    program.uniform("uTime", 0.1f);


    VertexStructure vstruct;
    vstruct.add("position", VertexAttribType::Float, 3);
    vao.bindVertexAttribs(program, buffer.getBindingInfo(), vstruct);

    utils::checkGLerror(CALL_SITE);

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    TRACE("Exiting drawing loop");
    
    vao.unbind();

    // Cleanup
    glfwTerminate();
    
    return 0;
}

int main(int argc, char *argv[])
{
    try
    {
        return _main(argc, argv);
    }
    catch(std::exception &e)
    {
        std::cerr << e.what();
        throw;
    }
}
