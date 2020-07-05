#include <iostream>
#include <stdexcept>
#include <string>

#include "Empty/render/Context.hpp"
#include "Empty/render/gl/ShaderProgram.hpp"
#include "Empty/render/gl/Texture.h"
#include "Empty/render/gl/VertexArray.h"
#include "Empty/utils/macros.h"
#include "Empty/utils/utils.hpp"

#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Mesh.h"

using namespace render::gl;

int _main(int, char* argv[])
{
    render::Context context("Empty sample program", 1280, 720);
    auto window = context.window;
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported())
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    Mesh mesh;
    mesh.vao.bind();
    if (mesh.load("cube.obj"))
        TRACE("Loading successful: " << mesh.vertices.size() << " vertices and " << mesh.faces.size() << " faces");

    //if (mesh.load("mctet.off"))
    //    TRACE("Loading successful: " << mesh.vertices.size() << " vertices and " << mesh.faces.size() << " faces");

    //utils::setwd(argv);

    Camera camera(90, (float)context.frameWidth / context.frameHeight, 0.01f, 100.f);
    camera.setPosition(0, 0, 5);

    int64_t size = mesh.vertexBuffer.getParameter<BufferParam::Size>();
    TRACE("Successfully reserved " << size << " bytes for buffer");

    {
        BufferMapping mapping = mesh.vertexBuffer.map(BufferAccess::ReadOnly);
        std::cout << "First position is " << mapping.get<math::vec3>(0) << std::endl;
    }

    ShaderProgram program;
    program.attachFile(ShaderType::Fragment, "Fragment.glsl");
    program.attachFile(ShaderType::Vertex, "Vertex.glsl");
    program.use();

    mesh.vao.bindVertexAttribs(mesh.vStruct, program);

    Texture<TextureTarget::Texture2D, TextureFormat::RGBA8> tex;
    tex.bind();
    tex.setParameter<TextureParam::MinFilter>(TextureParamValue::FilterLinear);
    TRACE("Texture default mag filter is " << utils::name(tex.getParameter<TextureParam::MagFilter>()));
    tex.uploadData(0, 64, 64, PixelFormat::RGBA, PixelType::Byte, nullptr);
    tex.unbind();

    utils::checkGLerror(CALL_SITE);

    glEnable(GL_DEPTH_TEST);

    TRACE("Entering drawing loop");

    double prevX, prevY;
    glfwGetCursorPos(window, &prevX, &prevY);

    while (!glfwWindowShouldClose(window))
    {
        program.uniform("uTime", (float)glfwGetTime());
        program.uniform("uCamera", camera.m);
        program.uniform("uP", camera.p);

        context.clearBuffers(true, true);
        if (mesh.isIndexed())
            context.drawElements(PrimitiveType::Triangles, ElementType::Int, 0, 3 * mesh.faces.size());
        else
            context.drawArrays(PrimitiveType::Triangles, 0, mesh.vertices.size());

        context.swap();
        glfwPollEvents();

        double x, y;
        glfwGetCursorPos(window, &x, &y);
        camera.processInput(glfwGetKey(window, GLFW_KEY_W), glfwGetKey(window, GLFW_KEY_S),
                            glfwGetKey(window, GLFW_KEY_SPACE), glfwGetKey(window, GLFW_KEY_LEFT_SHIFT),
                            glfwGetKey(window, GLFW_KEY_A), glfwGetKey(window, GLFW_KEY_D),
                            x - prevX, y - prevY);
        prevX = x;
        prevY = y;
    }
    
    TRACE("Exiting drawing loop");
    
    mesh.vao.unbind();
    
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
