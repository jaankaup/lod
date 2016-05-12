#ifndef COMMAND_H
#define COMMAND_H

#include <vector>
#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp>

struct CommandBlock {
    GLuint vao = 0;
    int startIndex = 0;
    int indexCount = 0;
    std::string texture;
    std::string shader;
    glm::mat4 model;
};

struct Command {
    GLuint vao = 0;
    int startIndex = 0;
    int indexCount = 0;
    std::string texture;
    std::string shader;
    glm::mat4 model;
};

/*
class Command
{
    public:
        Command();
        ~Command();
    private:
        struct CommandBlock {
            GLuint vao = 0;
            int startIndex = 0;
            int indexCount = 0;
            GLuint texture = 0;
            GLuint shader  = 0;
            glm::mat4 model;
        };
};
*/

#endif // COMMAND_H
