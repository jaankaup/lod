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

    public:
        ~Texture();
        void create(const std::string& fileloc, const std::string& textureUnitName, int unit);
        int getUnit();
        void bind() const;

    private:
        Texture();
        GLuint textureID_ = 0;
        std::string textureName_;
        int unit_ = 0;
};

#endif // TEXTURE_H
