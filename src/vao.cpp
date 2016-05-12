#include "vao.h"
#include <iostream>

VAO::VAO()
{
    glGenVertexArrays(1, &vaoID_);
}

VAO::~VAO()
{
    glDeleteVertexArrays(1, &vaoID_);
}
/*
VAO& VAO::operator=(const VAO& other)
{
    if(&other == this) return *this;
    vaoID_ = other.vaoID_;
    return *this;
}
*/

void VAO::bind() const
{
    glBindVertexArray(vaoID_);
}

void VAO::unBind() const
{
    glBindVertexArray(0);
}

GLuint VAO::getID() const
{
    return vaoID_;
}

VAO& VAO::getInstance()
{
    return *this;
}
