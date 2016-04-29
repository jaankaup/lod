#include "texturecontrol.h"

TextureControl::TextureControl()
{
    for (unsigned int i=0 ; i<units_.size() ; ++i)
    {
        units_[i] = false;
    }
}

TextureControl::~TextureControl()
{
    //dtor
}

TextureControl& TextureControl::getInstance() {
    static TextureControl instance;
    return instance;
}

TexturePtr TextureControl::createTexture(const std::string& fileLocation, const std::string& name)
{
    for (unsigned int i=0 ; i<units_.size() ; ++i)
    {
        if (units_[i] == false)
        {
            TexturePtr t(new Texture());
            t->create(fileLocation,name,i);
            units_[i] = true;
            return t;
        }
    }
    throw std::runtime_error("TextureControl::createTexture -> only 16 texture unit are allowed.");
}
