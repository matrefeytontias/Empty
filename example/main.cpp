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

int _main(int, char *argv[])
{
    Mesh mesh;
    if (mesh.load("mctet.off"))
        TRACE("Loading successful: " << mesh.vertices.size() << " vertices and " << mesh.faces.size() << " faces");

    utils::setwd(argv);
    
    render::Context context("Empty sample program", 1280, 720);
    auto window = context.window;

    math::vec2 mee(1.4f, 7.4f);
    math::vec2 moo(0.5f, 5.8f);
    mee += moo;

    math::Matrix2f mat = math::Matrix2f::Constant(1);
    mat *= 2;

    VertexArray vao;
    vao.bind();

    VertexStructure vstruct;
    vstruct.add("position", VertexAttribType::Float, 3);
    vstruct.add("id", VertexAttribType::Int, 1);
    vstruct.add("color", VertexAttribType::Byte, 4);
    size_t mockVertices = 8;

    Texture<TextureTarget::Texture2D, TextureFormat::RGBA8> tex;
    tex.bind();
    tex.setParameter<TextureParam::MinFilter>(TextureParamValue::FilterLinear);
    TRACE("Texture default mag filter is " << utils::name(tex.getParameter<TextureParam::MagFilter>()));
    tex.uploadData(0, 64, 64, PixelFormat::RGBA, PixelType::Byte, nullptr);
    tex.unbind();

    Buffer<BufferTarget::Array> buffer;
    buffer.bind();
    {
        std::vector<uint8_t> data(mockVertices * vstruct.bytesPerVertex(), 0);
        buffer.uploadData(data.size(), BufferUsage::DynamicRead, data.data());
    }
    int64_t size = buffer.getParameter<BufferParam::Size>();
    TRACE("Successfully reserved " << size << " bytes for buffer");

    ShaderProgram program;
    program.attachSource(ShaderType::Vertex, "in vec3 position; uniform float uTime; void main() { gl_Position = vec4(position * cos(uTime), 1.); }");
    program.use();
    program.uniform("uTime", 0.1f);

    vao.bindVertexAttribs(vstruct, program);

    {
        BufferMapping mapping = buffer.map(BufferAccess::ReadOnly);
        std::cout << "First position is " << mapping.get<math::vec3>(0) << std::endl;
        std::cout << "First id is " << mapping.get<int>(12) << std::endl;
        std::cout << "First color is " << mapping.get<int>(16) << std::endl;
    }

    utils::checkGLerror(CALL_SITE);

    TRACE("Entering drawing loop");

    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        context.swap();
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
