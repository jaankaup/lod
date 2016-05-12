#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>
#include <vector>
#include <soil.h>
#include <GL/glew.h>
#include "texturecontrol.h"

class Texture
{
    friend class TextureControl;
    friend class TextureManager;

    public:
        ~Texture();
        void use(const int unit) const;
        void bind() const;
        GLuint getID() const;

    private:
        Texture();
        void create(const std::string& fileloc);

        GLuint textureID_ = 0;
};

#endif // TEXTURE_H
