#include "Empty/utils/utils.hpp"

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

#include "Empty/math/Matrix.hpp"
#include "Empty/math/Vector.hpp"
#include "Empty/render/gl/Texture.h"
#include "Empty/utils/macros.h"

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
void utils::perspective(math::mat4 &p, float fov, float ratio, float near, float far)
{
    float d = 1 / tan(fov * M_PI / 180 / 2);
    float ir = 1.f / (near - far);
    
    p(0, 0) = d;
    p(1, 1) = d * ratio;
    p(2, 2) = (near + far) * ir;
    p(3, 3) = 0;
    p(3, 2) = -1;
    p(2, 3) = 2 * near * far * ir;
}

void utils::setAspectRatio(math::mat4 &p, float ratio)
{
    p(1, 1) = -p(0, 0) * ratio;
}

render::gl::CubeMapFace utils::getCubeMapFaceFromIndex(int i)
{
    using namespace render::gl;
    static constexpr const CubeMapFace faces[6] = {
        CubeMapFace::PlusX,
        CubeMapFace::MinusX,
        CubeMapFace::PlusY,
        CubeMapFace::MinusY,
        CubeMapFace::PlusZ,
        CubeMapFace::MinusZ
    };
    ASSERT(i >= 0 && i < 6 && "requested invalid cubemap face index");
    return faces[i];
}