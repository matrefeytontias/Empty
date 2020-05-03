#include "utils/utils.hpp"

#include <algorithm>
#include <cstring>
#include <fstream>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>

#ifdef _WIN32
#include <direct.h>
#undef near
#undef far
#define chdir _chdir
#else
#include <unistd.h>
#endif

#include "math/Matrix.hpp"
#include "math/Vector.hpp"
#include "render/gl/Texture.h"
#include "utils/macros.h"

static constexpr const float M_PI = 3.14159258536f;

void utils::setwd(char **argv)
{
    std::string buf(argv[0]);
    // Handle both possible separators
    size_t pos = buf.find_last_of('/');
    if (pos == std::string::npos)
        pos = buf.find_last_of('\\');
    if(pos != std::string::npos)
    {
        buf[pos] = '\0';
        chdir(buf.c_str());
    }
}

std::string utils::getFileContents(const std::string &path)
{
    std::ifstream ifs;
    ifs.open(path, std::ios_base::in | std::ios_base::binary);
    if(ifs.fail())
        FATAL("Could not open file " << path);
    std::string contents(std::istreambuf_iterator<char>{ifs}, std::istreambuf_iterator<char>{});
    return contents;
}

std::vector<std::string> utils::split(const std::string &s, const std::string &delim)
{
    std::vector<std::string> r;
    size_t index = 0;
    while(index < s.size())
    {
        size_t minSplitting = s.size();
        for(char c : delim)
        {
            size_t splitting = s.find(c, index);
            if(splitting < minSplitting)
                minSplitting = splitting;
        }
        r.push_back(s.substr(index, minSplitting - index));
        index = minSplitting + 1;
    }
    
    (void)std::remove_if(r.begin(), r.end(), [](std::string &s) { return !s.size(); });
    return r;
}

GLuint utils::createShaderFromSource(GLenum type, const std::string &path)
{
    GLuint shader = glCreateShader(type);
    std::string source = getFileContents(path);
    const char *ind = source.c_str();
    glShaderSource(shader, 1, &ind, NULL);
    glCompileShader(shader);
    return shader;
}

void utils::printShaderLog(GLuint shader)
{
    GLint logSize;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logSize);
    if(logSize > 0)
    {
        char *log = new char[logSize];
        glGetShaderInfoLog(shader, logSize, &logSize, log);
        std::cerr << "Log for shader #" << shader << " :" << std::endl;
        std::cerr << log << std::endl;
        delete[] log;
    }
    else
        std::cerr << "Shader #" << shader << " has no log" << std::endl;
}

void utils::checkGLerror(const char *file, int line)
{
    GLenum erro = glGetError();
    std::cerr << file << ":" << line << " : 0x" << std::hex;
    do
    {
        std::cerr << erro << " ";
    } while ((erro = glGetError()));
    std::cerr << std::dec << std::endl;
}

// Expects an identity matrix as input
void utils::perspective(math::Matrix4f &p, float fov, float ratio, float near, float far)
{
    float d = 1 / tan(fov * M_PI / 180 / 2);
    float ir = 1.f / (near - far);
    
    p(0, 0) = d;
    p(1, 1) = -d * ratio;
    p(2, 2) = (near + far) * ir;
    p(3, 3) = 0;
    p(3, 2) = -1;
    p(2, 3) = 2 * near * far * ir;
}

void utils::setAspectRatio(math::Matrix4f &p, float ratio)
{
    p(1, 1) = -p(0, 0) * ratio;
}

void utils::displayTexture(GLint texture, float dx, float dy)
{
    const float verts[] =
    {
        0.f + dx, 1.f + dy, 0.f, 0.f,
        0.f + dx, 0.f + dy, 0.f, 1.f,
        1.f + dx, 0.f + dy, 1.f, 1.f,
        1.f + dx, 1.f + dy, 1.f, 0.f
    };
    const unsigned int indices[] = { 0, 1, 2, 0, 2, 3 };
    
    GLuint vao;
    GLuint buffer, program, vertex_shader, fragment_shader;
    GLint aPos_location, aTexCoord_location, uTex_location;
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
    
    vertex_shader = createShaderFromSource(GL_VERTEX_SHADER, "shaders/displayTextureVertex.glsl");
    fragment_shader = createShaderFromSource(GL_FRAGMENT_SHADER, "shaders/displayTextureFragment.glsl");
    
    program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
    glUseProgram(program);
    
    aPos_location = glGetAttribLocation(program, "POSITION");
    aTexCoord_location = glGetAttribLocation(program, "TEXCOORD0");
    uTex_location = glGetUniformLocation(program, "uTex");
    glEnableVertexAttribArray(aPos_location);
    glVertexAttribPointer(aPos_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)0);
    glEnableVertexAttribArray(aTexCoord_location);
    glVertexAttribPointer(aTexCoord_location, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void *)(sizeof(float) * 2));
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(uTex_location, 0);
    
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, indices);
    
    glDisableVertexAttribArray(aPos_location);
    glDisableVertexAttribArray(aTexCoord_location);
    glBindTexture(GL_TEXTURE_2D, 0);
    glUseProgram(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    glDeleteProgram(program);
    glDeleteBuffers(1, &buffer);
    glDeleteVertexArrays(1, &vao);
}

constexpr int render::gl::dimensionsFromTarget(TextureTarget target)
{
    ASSERT(target != TextureTarget::Dynamic && "TextureTarget::Dynamic is not a real texture target");

    if (target == TextureTarget::Texture1D
        || target == TextureTarget::Proxy1D
        || target == TextureTarget::TextureBuffer)
        return 1;
    else if (target == TextureTarget::Texture3D
             || target == TextureTarget::Texture2DArray
             || target == TextureTarget::TextureCubeMapArray
             || target == TextureTarget::Proxy3D
             || target == TextureTarget::Proxy2DArray
             || target == TextureTarget::ProxyCubeMapArray)
        return 3;
    else
        return 2;
}
