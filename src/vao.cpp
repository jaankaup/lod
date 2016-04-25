#include "vao.h"

VAO::VAO()
{
    glGenVertexArrays(1, &vaoID_);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &vaoID_);
}

void VAO::bind() const
{
    glBindVertexArray(vaoID_);
}
