#include "materialmanager.h"

MaterialManager::MaterialManager()
{
    //ctor
}

MaterialManager::~MaterialManager()
{
    //dtor
}

MaterialManager& MaterialManager::getInstance()
{
    static MaterialManager instance;
    return instance;
}

void MaterialManager::create(const MaterialInfo& info)
{
    infot_.push_back(info);
}

MaterialInfo MaterialManager::getMaterialInfo(const std::string& nimi) const
{
    for (const auto& obj : infot_)
    {
        if (obj.nimi == nimi) return obj;
    }
    throw std::runtime_error("MaterialManager::getMaterialInfo() -> materiaalia '" + nimi + "' ei löydy.");
}
