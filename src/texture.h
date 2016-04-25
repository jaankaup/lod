#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <soil.h>
#include <GL/glew.h>


class Texture
{
    public:
        Texture();
        ~Texture();
        void create(const std::string& fileloc);
        void bind(GLenum textureUnit) const;
        GLuint getID() const;

    private:
        GLuint textureID_ = 0;
};

#endif // TEXTURE_H
