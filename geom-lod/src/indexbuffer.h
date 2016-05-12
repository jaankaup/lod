#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H
#include <GL/glew.h>

class IndexBuffer
{
    public:
        IndexBuffer();
        ~IndexBuffer();

        /*
        IndexBuffer(const IndexBuffer&) = delete;
        IndexBuffer(IndexBuffer&&) = default;
        IndexBuffer& operator=(const IndexBuffer&) = delete;
        IndexBuffer& operator=(IndexBuffer&&) = default;
        */

        void bind();

        void setData(const void* data, unsigned int sizeOfData, unsigned int numberOfElements);

        unsigned int getElementCount() const;

        GLuint getID() const;

        IndexBuffer& getInstance();

    private:
        GLuint ib_;
        unsigned int numberOfElemets_ = 0;

};

#endif // INDEXBUFFER_H
