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
    Mesh mesh;
    if (mesh.load("mctet.off"))
        TRACE("Loading successful: " << mesh.vertices.size() << " vertices and " << mesh.faces.size() << " faces");

    //utils::setwd(argv);
    
    render::Context context("Empty sample program", 1280, 720);
    auto window = context.window;

    math::vec2 mee(1.4f, 7.4f);
    math::vec2 moo(0.5f, 5.8f);
    mee += moo;

    math::Matrix2f mat = math::Matrix2f::Constant(1);
    mat *= 2;

    VertexArray vao;
    vao.bind();

    Buffer<BufferTarget::Array> vertexBuffer;
    vertexBuffer.bind();
    VertexStructure vertexStruct(0);
    vertexStruct.add("position", VertexAttribType::Float, 3);
    vertexBuffer.uploadData(vertexStruct.bytesPerVertex() * mesh.vertices.size(), BufferUsage::StaticDraw, mesh.vertices.data());

    int64_t size = vertexBuffer.getParameter<BufferParam::Size>();
    TRACE("Successfully reserved " << size << " bytes for buffer");

    {
        BufferMapping mapping = vertexBuffer.map(BufferAccess::ReadOnly);
        std::cout << "First position is " << mapping.get<math::vec3>(0) << std::endl;
    }

    ShaderProgram program;
    program.attachFile(ShaderType::Fragment, "Fragment.glsl");
    program.attachFile(ShaderType::Vertex, "Vertex.glsl");
    program.use();

    vao.bindVertexAttribs(vertexStruct, program);

    Texture<TextureTarget::Texture2D, TextureFormat::RGBA8> tex;
    tex.bind();
    tex.setParameter<TextureParam::MinFilter>(TextureParamValue::FilterLinear);
    TRACE("Texture default mag filter is " << utils::name(tex.getParameter<TextureParam::MagFilter>()));
    tex.uploadData(0, 64, 64, PixelFormat::RGBA, PixelType::Byte, nullptr);
    tex.unbind();

    Buffer<BufferTarget::ElementArray> triBuffer;
    triBuffer.bind();
    triBuffer.uploadData(sizeof(math::ivec3) * mesh.faces.size(), BufferUsage::StaticDraw, mesh.faces.data());

    utils::checkGLerror(CALL_SITE);

    TRACE("Entering drawing loop");

    while (!glfwWindowShouldClose(window))
    {
        program.uniform("uTime", (float)glfwGetTime());

        context.clearBuffers(true, true);
        context.drawElements(PrimitiveType::Triangles, ElementType::Int, 0, 3 * mesh.faces.size());

        context.swap();
        glfwPollEvents();
    }
    
    TRACE("Exiting drawing loop");
    
    vao.unbind();
    
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
