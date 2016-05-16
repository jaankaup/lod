#ifndef BUFFER_H
#define BUFFER_H

#include <iostream>
#include <GL/glew.h>

template <typename T>
struct StaticVertexCreator
{
    static T create(const void* data, unsigned int dataSize)
    {
        GLuint handle;
        glGenBuffers(1,&handle);

    }
};

class Buffer
{
    public:
        Buffer();
        ~Buffer();

    protected:

    private:
};

#endif // BUFFER_H
