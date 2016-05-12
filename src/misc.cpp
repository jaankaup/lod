// #include <iostream>
#include <fstream>
#include <streambuf>
#include <stdexcept>
#include <algorithm>
#include <array>
#include "misc.h"
#include "global.h"

namespace Misc {

std::string loadSource(const std::string& filelocation)
{
    std::ifstream file(filelocation);

    if (!file.is_open())
    {
        throw std::runtime_error("Misc::loadSource: counld'n open file " + filelocation);
    }

    std::string str;
    file.seekg(0, std::ios::end);
    str.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    str.assign((std::istreambuf_iterator<char>(file)),
               std::istreambuf_iterator<char>());

    return str;
}

bool startswith(const std::string& s1, const std::string& s2)
{
    int maxLength = std::max(s1.length(), s2.length());
    for (int i=0 ; i<maxLength ; ++i) if (s1[i] != s2[i]) return false;
    return true;
}

bool endswith(const std::string& s1, const std::string& s2)
{
    int l1 = s1.length();
    int l2 = s2.length();
    for (int i=l2-1, j=l1-1 ; (i>-1 && j>-1) ; --i, --j)
    {
        if (s1[j] != s2[i]) return false;
    }
    return true;
}



GLuint getShaderType(const std::string& filelocation)
{
    /* Supported shader object source code types. */
    static std::array<std::string, 6> fileSuffixes = {".vert",
                                                      ".gtcs",
                                                      ".gtes",
                                                      ".geom",
                                                      ".frag",
                                                      ".comp"
                                                     };

    /* Corrresponding shader object types in opengl. */
    static std::array<GLuint, 6> GLStageEnums = {GL_VERTEX_SHADER,
                                                 GL_TESS_CONTROL_SHADER,
                                                 GL_TESS_EVALUATION_SHADER,
                                                 GL_GEOMETRY_SHADER,
                                                 GL_FRAGMENT_SHADER,
                                                 GL_COMPUTE_SHADER
                                                };

    for (unsigned int i=0 ; i<fileSuffixes.size() ; i++)
    {
        if (endswith(filelocation,fileSuffixes[i])) return GLStageEnums[i];
    }
    return 0;
}

void compileShader(const GLuint id, const std::string& source)
{
    // Transform c++ string to C string.
    const char* src_C = source.c_str();
    // Push source to shader object managed by opengl.
    glShaderSource(id, 1, &src_C, NULL);
    // return shader object handle.
    glCompileShader(id);
}

bool checkShader(GLuint obj)
{
    /* We are checking linking status by default. */
    GLenum pname = GL_COMPILE_STATUS;

    int status;
    int errorLength;
    glGetShaderiv(obj, pname, &status);

    if (status == GL_FALSE)
    {
        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &errorLength);
        GLchar *errorMessage = new GLchar[errorLength];
        glGetShaderInfoLog(obj, errorLength, NULL, errorMessage);
        logError.log("Global::checkShader(): Shader object compilation failed: %", errorMessage);
        delete[] errorMessage;
        return false;
    }
    return true;
}

void tokenize(const std::string &line, std::vector<std::string> &container, const std::string delimRegex)
{
        /* Split line to tokens. */
        std::regex ws(delimRegex);
        std::copy( std::sregex_token_iterator(line.begin(), line.end(), ws, -1),
                   std::sregex_token_iterator(),
                   std::back_inserter(container));
}

}; // namespace Misc

glm::mat3 calculateNormalMatrix(const glm::mat4& modelView)
{
    glm::mat4 temp = glm::transpose(glm::inverse(modelView));
    return (glm::mat3(glm::vec3(temp[0][0],temp[0][1],temp[0][2]),glm::vec3(temp[1][0],temp[1][1],temp[1][2]),glm::vec3(temp[2][0],temp[2][1],temp[2][2])));
}
