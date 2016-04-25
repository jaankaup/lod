#include <exception>
#include "global.h"
#include "vertexbuffer.h"

VertexBuffer::VertexBuffer(const GLenum target, const GLenum usage)
{
    // Is this necessery?
    switch (target) {
      case GL_ARRAY_BUFFER : break;
      case GL_ATOMIC_COUNTER_BUFFER : break;
      case GL_COPY_READ_BUFFER : break;
      case GL_COPY_WRITE_BUFFER : break;
      case GL_DISPATCH_INDIRECT_BUFFER : break;
      case GL_DRAW_INDIRECT_BUFFER : break;
      case GL_ELEMENT_ARRAY_BUFFER : break;
      case GL_PIXEL_PACK_BUFFER : break;
      case GL_PIXEL_UNPACK_BUFFER : break;
      case GL_QUERY_BUFFER : break;
      case GL_SHADER_STORAGE_BUFFER : break;
      case GL_TEXTURE_BUFFER : break;
      case GL_TRANSFORM_FEEDBACK_BUFFER : break;
      case GL_UNIFORM_BUFFER : break;
      default : throw std::runtime_error("VertexBuffer::VertexBuffer() : illegal GLenum target.");
    };

    glGenBuffers(1,&vboID_);
    target_ = target;
    usage_ = usage;
};

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1,&vboID_);
}

void VertexBuffer::bind() const
{
	//TODO: check that out target_ is well initialized.
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

    // vertex attributes for out program. Out data is in this shape: VVVTTNNN VVVTTNNN VVVTTNNN....
    // We have interleaved data. V = vertex coordinate, T is texture coordinate, N is normal coordinate.
    // So we have 3 floats for vertex data (vec3), 2 floats for texture data (vec2) and 3 floats for normal data (vec3).
    // VVVTTNNN is ONE point in out model. VVVTTNNN VVVTTNNN VVVTTNNN is a triangle. V:s goes to shader location 0.
    // N:s goes to shader location 1 and N:s goes to shader location 2 in our shader programs.

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

    /*
    std::array<Attribute,3> attributes = {Attribute(0,3,GL_FLOAT,GL_FALSE,0,0),
                                          Attribute(1,2,GL_FLOAT,GL_FALSE,sizeof(GL_FLOAT)*3,0),
                                          Attribute(2,3,GL_FLOAT,GL_FALSE,sizeof(GL_FLOAT)*5,0)
                                         };
    */

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
    }
};
