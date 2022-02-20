#include <iostream>
#include <stdexcept>
#include <string>

#include "Empty/math/funcs.h"
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
#ifdef _DEBUG
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif
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

void printGLerror(DebugMessageSource source, DebugMessageType type, DebugMessageSeverity severity, int, const std::string& msg, const void*)
{
    std::cerr << utils::name(source) << " reported " << utils::name(type) << " (severity " << utils::name(severity) << ") : " << msg << std::endl;
}

int _main(int, char* argv[])
{
    // Coordinate swizzling fun to check that it works
    math::vec2 hihi(1, 2);
    math::vec3 haha(3, 4, 5);
    math::vec4 huhu(6, 7, 8, 9);

    hihi.yx() = hihi;
    haha.xz() = hihi.yy();
    huhu.zyw() = haha.xzy();
    // huhu.xx() = hihi; // rightfully doesn't compile

    TRACE(hihi);
    TRACE(haha);
    TRACE(huhu);

    // It works :)

    // Try our math functions for a bit
    TRACE(math::normalize(hihi) << ", " << math::normalize(haha.yx()) << ", " << math::normalize(huhu));
    TRACE(math::min(hihi.xyy(), haha.zyx()) << ", " << math::max(huhu, haha.zzyx()) << ", " << math::clamp(huhu, 2.f, 5.f));

    math::mat2 blah;
    for (int k = 0; k < 4; k++)
        blah[k] = k + 1.f;

    math::dmat3 bleh;
    for (int k = 0; k < 9; k++)
        bleh[k] = k + 1.;
    bleh[7] = 9; // not invertible otherwise

    math::mat4 bloo;
    for (int k = 0; k < 16; k++)
        bloo[k] = k + 1.f;
    bloo[9] = 12.f; // likewise
    bloo[14] = 16.f;

    TRACE("bloo =\n" << bloo << "\ntranspose(bloo) =\n" << math::transpose(bloo));
    TRACE(blah << " * " << math::inverse(blah) << " = " << blah * math::inverse(blah));
    TRACE(bleh << " * " << math::inverse(bleh) << " = " << bleh * math::inverse(bleh));
    TRACE(bloo << " * " << math::inverse(bloo) << " = " << bloo * math::inverse(bloo));

    GLFWContext context("Empty sample program", 1280, 720);
    
    // Bit of debug things ye
    context.enable(ContextCapability::DebugOutput);
    context.enable(ContextCapability::DebugOutputSynchronous);
    context.debugMessageCallback(printGLerror, nullptr);
    context.debugMessageControl(DebugMessageSource::DontCare, DebugMessageType::DontCare, DebugMessageSeverity::DontCare, true);

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

        // Create stuff for off-screen rendering
        Framebuffer fb;
        // Attach texture to color attachment #2 why not (careful to enable it later down the line)
        fb.attachTexture<FramebufferAttachment::Color>(2, fbtex, 0);
        Renderbuffer rb;
        rb.setStorage(RenderbufferFormat::Depth, imgW, imgH);
        fb.attachRenderbuffer<FramebufferAttachment::Depth>(rb);
        TRACE("Framebuffer has status " << utils::name(fb.checkStatus(FramebufferTarget::Draw)));

        ShaderProgram fbprog;
        fbprog.attachFile(ShaderType::Vertex, "DeferredVertex.glsl");
        fbprog.attachFile(ShaderType::Fragment, "DeferredFragment.glsl");
        fbprog.build();
        // Don't use the auto-bind feature to show you don't have to
        fbprog.registerTexture("uTexture", tex, false);

        VertexStructure vs;
        vs.add("aPosition", VertexAttribType::Float, 2);

        fbprog.locateAttributes(vs);

        static const math::vec2 points[6] = { {0, 0}, {1, 0}, {1, 1}, {0, 0}, {1, 1}, {0, 1} };

        Buffer pointsBuf;
        pointsBuf.setStorage(sizeof(points), BufferUsage::StaticDraw, points);

        VertexArray va;
        va.attachVertexBuffer(pointsBuf, vs);

        context.bind(va);
        // Remember when we attached a texture on color attachment #2 ?
        fb.enableColorAttachments(2);
        context.setFramebuffer(fb, FramebufferTarget::Draw, imgW, imgH);
        context.setShaderProgram(fbprog);
        // The texture was not auto-bound, thus we bind it ourselves
        context.bind(tex, fbprog.findUniform("uTexture"));

        context.drawArrays(PrimitiveType::Triangles, 0, 6);

        // We've done it ; fbtex should contain the render

        fbtex.generateMipmaps();

        context.resetViewport();
    }

    TRACE("5th mipmap has dimensions " << fbtex.getLevelParameter<TextureLevelParam::Width>(5) << "x" << fbtex.getLevelParameter<TextureLevelParam::Height>(5));

    // Generate a procedural mask texture with a compute shader, why not
    Texture<TextureTarget::Texture2D, TextureFormat::Red8ui> proceduralMask;
    {
        proceduralMask.setStorage(1, 256, 256);
        proceduralMask.setParameter<TextureParam::MagFilter>(TextureParamValue::FilterNearest);
        proceduralMask.setParameter<TextureParam::MinFilter>(TextureParamValue::FilterNearest);
        ShaderProgram computeProg;
        computeProg.attachFile(ShaderType::Compute, "Compute.glsl");
        computeProg.build();
        computeProg.registerTexture("uProcTex", proceduralMask, false);
        computeProg.uniform("uResolution", 4u);
        context.setShaderProgram(computeProg);
        context.bind(proceduralMask.getLevel(0), 0, AccessPolicy::WriteOnly, TextureFormat::Red8ui);
        context.dispatchCompute(64, 64, 1);
        // Tada, that's it
    }

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
        BufferMapping mapping = mesh.vertexBuffer.map(AccessPolicy::ReadOnly);
        TRACE("First position is " << mapping.get<math::vec3>(0));
    }

    ShaderProgram program;
    program.attachFile(ShaderType::Fragment, "Fragment.glsl");
    program.attachFile(ShaderType::Vertex, "Vertex.glsl");
    program.build();

    program.locateAttributes(mesh.vStruct);
    program.registerTexture("uTexture", fbtexview);
    program.registerTexture("uProcTex", proceduralMask);

    mesh.vao.attachVertexBuffer(mesh.vertexBuffer, mesh.vStruct);
    mesh.vao.attachElementBuffer(mesh.triBuffer);

    context.enable(ContextCapability::DepthTest);
    context.enable(ContextCapability::Blend);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    context.enable(ContextCapability::FramebufferSRGB);
    context.disable(ContextCapability::CullFace);

    TRACE("Entering drawing loop");

    double prevX, prevY;
    glfwGetCursorPos(window, &prevX, &prevY);

    context.bind(mesh.vao);

    context.clearColor = math::vec4(0, 0, 0, 1);
    context.clearDepth = 1;

    context.setShaderProgram(program);

    math::mat4 modelRotation;

    float time = 0;

    while (!glfwWindowShouldClose(window))
    {
        if (!glfwGetKey(window, GLFW_KEY_F))
            time = (float)glfwGetTime();

        // Sick zoom-in because hey look I do what I want
        modelRotation = math::mat4::Identity() * (float)(1. + exp(-(1. + cos(time * 15.5)) * 10.));
        modelRotation(3, 3) = 1.;
        modelRotation *= math::rotateZ(time * 1.5f) * math::rotateY(time * 2.f);

        program.uniform("uModel", modelRotation);
        program.uniform("uTime", time);
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
