#include <iostream>
#include <stdexcept>
#include <string>

#include "Empty/render/Context.hpp"
#include "Empty/render/gl/Framebuffer.h"
#include "Empty/render/gl/Renderbuffer.h"
#include "Empty/render/gl/ShaderProgram.hpp"
#include "Empty/render/gl/Texture.h"
#include "Empty/render/gl/VertexArray.h"
#include "Empty/utils/EnumBitfield.h"
#include "Empty/utils/macros.h"
#include "Empty/utils/utils.hpp"

#include <GLFW/glfw3.h>

#include "Camera.h"
#include "Mesh.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace render::gl;

struct GLFWContext : public render::Context
{
    GLFWwindow* window;

    GLFWContext(const char* title, int width, int height, int major = 4, int minor = 5) : Context(width, height, major, minor)
    {
        ASSERT(major * 100 + minor >= 405);
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

        gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    }

    void swap() const override final { glfwSwapBuffers(window); }

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

    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {}
};

int _main(int, char* argv[])
{
    GLFWContext context("Empty sample program", 1280, 720);
    auto *window = context.window;
    if (glfwRawMouseMotionSupported())
    {
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
    }

    // Bake a texture with off-screen rendering because we can
    int imgW, imgH, n;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* img = stbi_load("texture.png", &imgW, &imgH, &n, 4);
    if (!img)
        FATAL("pas trouveeeeeee");
    TRACE("Successfully loaded " << imgW << "x" << imgH << "x" << n << " image");

    // We will later use the second mipmap level of fbtex to demonstrate texture views
    Texture<TextureTarget::Texture2D, TextureFormat::RGBA8> fbtex;
    fbtex.setStorage(10, imgW, imgH);
    {
        // Upload image to texture
        Texture<TextureTarget::Texture2D, TextureFormat::SRGBA8> tex;
        tex.setStorage(1, imgW, imgH);
        tex.setParameter<TextureParam::MinFilter>(TextureParamValue::FilterLinear);
        TRACE("Texture default mag filter is " << utils::name(tex.getParameter<TextureParam::MagFilter>()));
        tex.uploadData(0, 0, 0, imgW, imgH, DataFormat::RGBA, DataType::UByte, img);
        stbi_image_free(img);
        utils::checkGLerror(CALL_SITE);

        // Create stuff for off-screen rendering
        Framebuffer fb;
        fb.attachTexture<FramebufferAttachment::Color>(0, fbtex, 0);
        Renderbuffer rb;
        rb.setStorage(RenderbufferFormat::Depth, imgW, imgH);
        fb.attachRenderbuffer<FramebufferAttachment::Depth>(rb);
        TRACE(utils::name(fb.checkStatus(FramebufferTarget::Draw))); // FramebufferStatus::Complete

        ShaderProgram fbprog;
        fbprog.attachFile(ShaderType::Vertex, "DeferredVertex.glsl");
        fbprog.attachFile(ShaderType::Fragment, "DeferredFragment.glsl");
        fbprog.build();
        fbprog.registerTexture("uTexture", tex);

        VertexStructure vs;
        vs.add("aPosition", VertexAttribType::Float, 2);

        fbprog.locateAttributes(vs);

        static const math::vec2 points[6] = { {0, 0}, {1, 0}, {1, 1}, {0, 0}, {1, 1}, {0, 1} };

        Buffer pointsBuf;
        pointsBuf.setStorage(sizeof(points), BufferUsage::StaticDraw, points);

        VertexArray va;
        va.attachVertexBuffer(pointsBuf, vs);

        context.bind(va);
        context.setFramebuffer(fb, FramebufferTarget::Draw, imgW, imgH);
        context.setShaderProgram(fbprog);

        context.drawArrays(PrimitiveType::Triangles, 0, 6);

        // We've done it ; fbtex should contain the render

        fbtex.generateMipmaps();

        context.resetViewport();
    }

    TRACE("5th mipmap has dimensions " << fbtex.getLevelParameter<TextureLevelParam::Width>(5) << "x" << fbtex.getLevelParameter<TextureLevelParam::Height>(5));

    // Set up a texture view because we can
    auto fbtexview = fbtex.makeView<TextureTarget::Texture2D, TextureFormat::RGBA8>(0, 10);

    // Set up the actual scene

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
    program.registerTexture("uTexture", fbtexview);

    mesh.vao.attachVertexBuffer(mesh.vertexBuffer, mesh.vStruct);
    mesh.vao.attachElementBuffer(mesh.triBuffer);

    context.enable(ContextCapability::DepthTest);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    context.enable(ContextCapability::Blend);
    context.enable(ContextCapability::FramebufferSRGB);

    utils::checkGLerror(CALL_SITE);

    TRACE("Entering drawing loop");

    double prevX, prevY;
    glfwGetCursorPos(window, &prevX, &prevY);

    context.bind(mesh.vao);

    context.clearColor = math::vec4(0, 0, 0, 1);
    context.clearDepth = 1;

    context.setShaderProgram(program);

    math::mat4 modelRotation;

    while (!glfwWindowShouldClose(window))
    {
        // Sick zoom-in because hey look I do what I want
        modelRotation = math::mat4::Identity() * (float)(1. + exp(-(1. + cos(glfwGetTime() * 15.5)) * 10.));
        modelRotation(3, 3) = 1.;
        modelRotation *= math::rotateZ((float)glfwGetTime() * 1.5f) * math::rotateY((float)glfwGetTime() * 2.f);

        program.uniform("uModel", modelRotation);
        program.uniform("uTime", (float)glfwGetTime());
        program.uniform("uCamera", camera.m);
        program.uniform("uP", camera.p);

        context.clearBuffer(DrawBufferType::Color);
        context.clearBuffer(DrawBufferType::Depth);

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
