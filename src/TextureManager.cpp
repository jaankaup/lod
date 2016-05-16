#include "TextureManager.h"

TextureManager::TextureManager()
{

}

TextureManager::~TextureManager()
{
    //dtor
}

TextureManager& TextureManager::getInstance()
{
    static TextureManager instance;
    return instance;
}

void TextureManager::createTexture(const std::string& fileLocation, const std::string& nimi)
{
    TexturePtr t(new Texture());
    t->create(fileLocation);
    tekstuurit_.push_back(std::make_pair(nimi,std::move(t)));
}

Texture* TextureManager::getTexture(const std::string& nimi) const
{
    for (const auto& obj : tekstuurit_)
    {
        if (std::get<0>(obj) == nimi)
        {
            return std::get<1>(obj).get();
        }
    }
    throw std::runtime_error("TextureManager::createTexture -> ei loytynyt tekstuuria nimella '" + nimi + "'.");
}

