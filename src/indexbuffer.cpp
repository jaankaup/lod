#include "indexbuffer.h"

IndexBuffer::IndexBuffer()
{
    glGenBuffers(1, &ib_);

}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1,&ib_);
}

void IndexBuffer::bind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib_);
}

void IndexBuffer::setData(const void* data, unsigned int sizeOfData, unsigned int numberOfElements)
{
    bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeOfData, data, GL_STATIC_DRAW);
    numberOfElemets_ = numberOfElements;
}

unsigned int IndexBuffer::getElementCount() const
{
    return numberOfElemets_;
}

GLuint IndexBuffer::getID() const
{
    return ib_;
}

IndexBuffer& IndexBuffer::getInstance()
{
    return *this;
}
