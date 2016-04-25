#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H
#include <tuple>
#include <vector>
#include <GL/glew.h>

/** class for vertex buffers. */

enum class AttributeConfiguration {sigle_V = 0, VTN_Interleaved = 1};

class VertexBuffer
{

public:
    /// Constructor.
    VertexBuffer(const GLenum target, const GLenum usage);
    /// Destructor.
    ~VertexBuffer();
    /// Bind buffer. When buffer is binded one can use it or modify it.
    void bind() const;
    /// Set data for buffer object.
    void setData(const void* data, unsigned int dataSize, AttributeConfiguration ac);
    /// Define an array of generic vertex attribute data.
    void addAttributes(AttributeConfiguration ac);

private:

    /// handle to the vertex buffer object.
    GLuint vboID_;
    /// The target of buffer.
    GLenum target_;
    /// The usage of buffer.
    GLenum usage_;

    //std::vector<Attribute> attributes_;
};
#endif // VERTEXBUFFER_H
