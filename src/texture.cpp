#include "texture.h"
#include "global.h"

Texture::Texture()
{
    glGenTextures(1,&textureID_);
}

Texture::~Texture()
{
    if (textureID_ != 0) glDeleteTextures(1,&textureID_);
}

/* For more information: https://open.gl/textures */
void Texture::create(const std::string& fileloc)
{
    int width, height;
    unsigned char* image = SOIL_load_image(fileloc.c_str(), &width, &height, 0, SOIL_LOAD_RGB);

    if (image == 0)
    {
        logError.log("Error: Texture::create(%)", fileloc);
        throw std::runtime_error(SOIL_last_result());
    }

    /* HATARATKAISU: jos polku alkaa nain, niin bindataan tekstuuri texture unit 1:seen. */
    if (Misc::startswith(fileloc, "data/textures/displace"))
    {
        use(1);
    }
    else use(0);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    SOIL_free_image_data(image);
}

void Texture::bind() const
{
    glBindTexture(GL_TEXTURE_2D,textureID_);
}

void Texture::use(const int unit) const
{
    glActiveTexture(GL_TEXTURE0 + unit);
    bind();
}

GLuint Texture::getID() const
{
    return textureID_;
}

