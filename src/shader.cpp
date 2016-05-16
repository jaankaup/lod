#include <stdexcept>
#include <vector>
#include <fstream>
#include <tuple>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include "misc.h"
#include "global.h"

Shader::Shader()
{
}

Shader::~Shader()
{
    if (shaderID_ != 0) glDeleteProgram(shaderID_);
}

void Shader::use()
{
    glUseProgram(shaderID_);
}

void Shader::createFromSource(const std::vector<std::string>& filelocations)
{

    using ShaderObjData = struct{
        GLenum shaderType;
        GLuint shaderObj;
        std::string sourceCode;
    };

    /* A typedef for shader object data: shader object type and source code. */
    using SOD = std::vector<ShaderObjData>;

    /* Create a handle to shader program. */
    shaderID_ = glCreateProgram();

    /* An error occurred while creating the shader program. */
    if (shaderID_ == 0) {
        throw std::runtime_error("Shader::createFromSource: could'n create a program. glCreateProgram() == 0.");
    }

    /* A container for shader objects type and source code. */
    SOD sod;

    /* We iterate trought all source codes and extract shader object types and sourcecodes. */
    for (const auto& location : filelocations)
    {
        logInfo.log("Creating shader from file: %", location);

        /* Solve the type of shader object from file extension. */
        GLenum shaderType = Misc::getShaderType(location);
        if (shaderType == 0)
        {
            logError.log("Shader::createFromSource: Could'n extract the shader object type from filelocation '%'.)",location);
            // TODO: exception
        }

        ShaderObjData shaderObjectData;
        shaderObjectData.shaderType = shaderType;
        shaderObjectData.shaderObj = glCreateShader(shaderType);
        shaderObjectData.sourceCode = Misc::loadSource(location);
        /* Put the source type and source code to sod. */
        sod.push_back(shaderObjectData);
    }

    for (const auto& object : sod)
    {
        Misc::compileShader(object.shaderObj, object.sourceCode);
        if (!Misc::checkShader(object.shaderObj))
            std::runtime_error("Shader::createFromSource: An error occurred while compiling a shader object.");
    }

    /* Attach shader objects to the program. */
    for (const auto& object : sod)
    {
        glAttachShader(shaderID_, object.shaderObj);
    }

    GLenum pname = GL_LINK_STATUS;

    glLinkProgram(shaderID_);

    int status;
    int errorLength;
    glGetShaderiv(shaderID_, pname, &status);

    if (status == GL_FALSE)
    {
        glGetShaderiv(shaderID_, GL_INFO_LOG_LENGTH, &errorLength);
        GLchar *errorMessage = new GLchar[errorLength];
        glGetShaderInfoLog(shaderID_, errorLength, NULL, errorMessage);
        logError.log("Shader object compilation failed: %", errorMessage);
        delete[] errorMessage;
    }

    for (const auto object : sod)
    {
        glDeleteShader(object.shaderObj);
    }
    logInfo.log("Shader created succesfully.");
}

void Shader::setUniform(const std::string& name, int value) const
{
    GLint loc = glGetUniformLocation(shaderID_, name.c_str());
    //if (loc == -1) logDebug.log("setUniform(%) ei loydy!", name);
    glUniform1i(loc, value);
}

void Shader::setUniform(const std::string& name, float value) const
{
    GLint loc = glGetUniformLocation(shaderID_, name.c_str());
    //if (loc == -1) logDebug.log("setUniform(%) ei loydy!", name);
    glUniform1f(loc, value);
}

void Shader::setUniform(const std::string& name, const glm::vec3& value) const
{
    GLint loc = glGetUniformLocation(shaderID_, name.c_str());
    //if (loc == -1) logDebug.log("setUniform(%) ei loydy!", name);
    glUniform3fv(loc, 1,  glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::mat3& value) const
{
    GLint loc = glGetUniformLocation(shaderID_, name.c_str());
    //if (loc == -1) logDebug.log("setUniform(%) ei loydy!", name);
    glUniform4fv(loc, 1,  glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::vec4& value) const
{
    GLint loc = glGetUniformLocation(shaderID_, name.c_str());
    //if (loc == -1) logDebug.log("setUniform(%) ei loydy!", name);
    glUniform4fv(loc, 1,  glm::value_ptr(value));
}

void Shader::setUniform(const std::string& name, const glm::mat4& value) const
{
    GLint loc = glGetUniformLocation(shaderID_, name.c_str());
    //if (loc == -1) logDebug.log("setUniform(%) ei loydy!", name);
    glUniformMatrix4fv(loc, 1, GL_FALSE,  glm::value_ptr(value));
}

GLuint Shader::getID() const
{
    return shaderID_;
}





