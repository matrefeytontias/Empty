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

#include "Mesh.h"

using namespace render::gl;

int _main(int, char* argv[])
{
    render::Context context("Empty sample program", 1280, 720);
    auto window = context.window;

    Mesh mesh;
    mesh.vao.bind();
    if (mesh.load("cube.obj"))
        TRACE("Loading successful: " << mesh.vertices.size() << " vertices and " << mesh.faces.size() << " faces");

    //if (mesh.load("mctet.off"))
    //    TRACE("Loading successful: " << mesh.vertices.size() << " vertices and " << mesh.faces.size() << " faces");

    //utils::setwd(argv);

    math::mat4 camera = math::mat4::Identity();
    camera(2, 3) = 10;
    std::cout << camera;
    math::mat4 P = math::mat4::Identity();
    utils::perspective(P, 90, (float)context.frameWidth / context.frameHeight, 0.001f, 100.f);
    std::cout << P;

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

    while (!glfwWindowShouldClose(window))
    {
        program.uniform("uTime", (float)glfwGetTime());
        program.uniform("uCamera", camera);
        program.uniform("uP", P);

        context.clearBuffers(true, true);
        if (mesh.isIndexed())
            context.drawElements(PrimitiveType::Triangles, ElementType::Int, 0, 3 * mesh.faces.size());
        else
            context.drawArrays(PrimitiveType::Triangles, 0, mesh.vertices.size());

        context.swap();
        glfwPollEvents();

        camera(0, 3) += utils::select(0.05f, glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) - utils::select(0.05f, glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS);
        camera(1, 3) += utils::select(0.05f, glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) - utils::select(0.05f, glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS);
        camera(2, 3) += utils::select(0.05f, glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) - utils::select(0.05f, glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS);

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
