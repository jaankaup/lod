#include <exception>
#include "global.h"
#include "vertexbuffer.h"

VertexBuffer::VertexBuffer()
{
    glGenBuffers(1,&vboID_);
    target_ = GL_ARRAY_BUFFER;
    usage_ = GL_STATIC_DRAW;
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1,&vboID_);
}

void VertexBuffer::bind() const
{
    glBindBuffer(target_, vboID_);
};

void VertexBuffer::setData(const void* data, unsigned int dataSize, AttributeConfiguration ac)
{
    bind();
    glBufferData(target_,dataSize, data, usage_);
    addAttributes(ac);
}

void VertexBuffer::addAttributes(AttributeConfiguration ac)
{
    // a typedef for attibutes. 0 = attribute location, 1 = size, 2 = type, 3 = normalized, 4 = stride, 5 = offset. TODO: define struct rather than tuple.
    using Attribute = std::tuple<GLuint,GLint,GLenum,GLboolean,GLsizei,const GLvoid*>;

    std::vector<Attribute> attributes;

    switch (ac) {
    case AttributeConfiguration::VTN_Interleaved:
        attributes.push_back(Attribute(0,3,GL_FLOAT,GL_FALSE,sizeof(GL_FLOAT)*8,0));
        attributes.push_back(Attribute(1,2,GL_FLOAT,GL_FALSE,sizeof(GL_FLOAT)*8,(GLvoid*)(sizeof(GL_FLOAT)*3)));
        attributes.push_back(Attribute(2,3,GL_FLOAT,GL_FALSE,sizeof(GL_FLOAT)*8,(GLvoid*)(sizeof(GL_FLOAT)*5)));
        break;
    case AttributeConfiguration::sigle_V:
        attributes.push_back(Attribute(0,3,GL_FLOAT,GL_FALSE,0,(GLvoid*)0));
    };

    // tell to opengl how to interpret the data of this buffer.
    for (const auto& attrib : attributes)
    {
        glEnableVertexAttribArray(std::get<0>(attrib));
        glVertexAttribPointer(
            std::get<0>(attrib), // attribute location. location for shader attribute.
            std::get<1>(attrib), // size
            std::get<2>(attrib), // type
            std::get<3>(attrib), // normalized
            std::get<4>(attrib), // stride
            std::get<5>(attrib)  // array buffer offset.
        );
        //glDisableVertexAttribArray(std::get<0>(attrib)); //ÄLÄ MUUTA
    }
};

GLuint VertexBuffer::getID() const
{
    return vboID_;
}

VertexBuffer& VertexBuffer::getInstance()
{
    return *this;
}
