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
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace render::gl;

int _main(int, char* argv[])
{
    render::Context context("Empty sample program", 1280, 720);
    auto *window = context.window;
    if (glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    Mesh mesh;
    if (mesh.load("cube.obj"))
        TRACE("Loading successful: " << mesh.vertices.size() << " vertices and " << mesh.faces.size() << " faces");

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
    program.build();

    program.locateAttributes(mesh.vStruct);

    mesh.vao.attachVertexBuffer(mesh.vertexBuffer, mesh.vStruct);
    mesh.vao.attachElementBuffer(mesh.triBuffer);

    int imgW, imgH, n;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* img = stbi_load("texture.png", &imgW, &imgH, &n, 4);
    if (!img)
        FATAL("pas trouveeeeeee");
    TRACE("Successfully loaded " << imgW << "x" << imgH << "x" << n << " image");

    Texture<TextureTarget::Texture2D, TextureFormat::SRGBA8> tex;
    tex.setStorage(1, imgW, imgH);
    tex.setParameter<TextureParam::MinFilter>(TextureParamValue::FilterLinear);
    TRACE("Texture default mag filter is " << utils::name(tex.getParameter<TextureParam::MagFilter>()));
    tex.uploadData(0, 0, 0, imgW, imgH, PixelFormat::RGBA, PixelType::UByte, img);
    stbi_image_free(img);
    utils::checkGLerror(CALL_SITE);

    program.registerTexture("uTexture", tex);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_FRAMEBUFFER_SRGB);

    utils::checkGLerror(CALL_SITE);

    TRACE("Entering drawing loop");

    double prevX, prevY;
    glfwGetCursorPos(window, &prevX, &prevY);

    context.bind(mesh.vao);

    context.clearColor = math::vec4(0, 0, 0, 1);
    context.clearDepth = 1;

    context.setShaderProgram(program);

    while (!glfwWindowShouldClose(window))
    {
        program.uniform("uTime", (float)glfwGetTime());
        program.uniform("uCamera", camera.m);
        program.uniform("uP", camera.p);

        context.clearBuffers(DrawBufferType::Color);
        context.clearBuffers(DrawBufferType::Depth);

        if (mesh.isIndexed())
            context.drawElements(PrimitiveType::Triangles, ElementType::Int, 0, 3 * (int)mesh.faces.size());
        else
            context.drawArrays(PrimitiveType::Triangles, 0, (int)mesh.vertices.size());

        context.swap();
        glfwPollEvents();

        double x, y;
        glfwGetCursorPos(window, &x, &y);
        camera.processInput(glfwGetKey(window, GLFW_KEY_W), glfwGetKey(window, GLFW_KEY_S),
                            glfwGetKey(window, GLFW_KEY_SPACE), glfwGetKey(window, GLFW_KEY_LEFT_SHIFT),
                            glfwGetKey(window, GLFW_KEY_A), glfwGetKey(window, GLFW_KEY_D),
                            static_cast<float>(x - prevX), static_cast<float>(y - prevY));
        prevX = x;
        prevY = y;
    }
    
    TRACE("Exiting drawing loop");

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
