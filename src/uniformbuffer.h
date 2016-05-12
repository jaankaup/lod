#ifndef UNIFORMBUFFER_H
#define UNIFORMBUFFER_H

#include <GL/glew.h>

template <typename Block>
class UniformBuffer : public Block
{
    public:
        UniformBuffer();
        void bind();

    protected:
        ~UniformBuffer();

    private:
        GLuint uboID_ = 0;
};

template <typename Block>
UniformBuffer<Block>::UniformBuffer()
{
    glGenBuffers(1, &uboID_);
}

template <typename Block>
UniformBuffer<Block>::~UniformBuffer()
{
    glDeleteBuffers(1, &uboID_);
}

template <typename Block>
void UniformBuffer<Block>::bind()
{
    glDeleteBuffers(GL_UNIFORM_BUFFER, uboID_);
}

class Sand
{
    public:

    private:

};
//class

#endif // UNIFORMBUFFER_H
