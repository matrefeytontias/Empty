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

#include "Empty/math/mat.h"
#include "Empty/math/vec.h"
#include "Empty/gl/Texture.h"
#include "Empty/utils/macros.h"

static constexpr const float M_PI = 3.14159258536f;

void Empty::utils::setwd(char **argv)
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

std::string Empty::utils::getFileContents(const std::string_view &path)
{
    std::ifstream ifs;
    ifs.open(path.data(), std::ios_base::in | std::ios_base::binary);
    if(ifs.fail())
        FATAL("Could not open file " << path);
    std::string contents(std::istreambuf_iterator<char>{ifs}, std::istreambuf_iterator<char>{});
    return contents;
}

std::vector<std::string_view> Empty::utils::split(const std::string_view &s, const std::string_view &delim)
{
    std::vector<std::string_view> r;
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
    
    (void)std::remove_if(r.begin(), r.end(), [](std::string_view &s) { return !s.size(); });
    return r;
}

void Empty::utils::checkGLerror(const char *file, int line)
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
void Empty::utils::perspective(math::mat4 &p, float fov, float ratio, float near, float far)
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

void Empty::utils::setAspectRatio(math::mat4 &p, float ratio)
{
    p(1, 1) = -p(0, 0) * ratio;
}

Empty::gl::CubemapFace Empty::utils::getCubeMapFaceFromIndex(int i)
{
    using namespace Empty::gl;
    static constexpr const CubemapFace faces[6] = {
        CubemapFace::PlusX,
        CubemapFace::MinusX,
        CubemapFace::PlusY,
        CubemapFace::MinusY,
        CubemapFace::PlusZ,
        CubemapFace::MinusZ
    };
    ASSERT(i >= 0 && i < 6 && "requested invalid cubemap face index");
    return faces[i];
}

bool Empty::utils::endsWith(const std::string_view& str, const std::string_view& suffix)
{
    return str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix);
}
