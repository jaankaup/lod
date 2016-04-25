#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H
#include <GL/glew.h>

class IndexBuffer
{
    public:
        IndexBuffer();
        ~IndexBuffer();
        void bind();

        void setData(const void* data, unsigned int sizeOfData, unsigned int numberOfElements);

        unsigned int getElementCount() const;

    private:
        GLuint ib_;
        unsigned int numberOfElemets_ = 0;

};

#endif // INDEXBUFFER_H
