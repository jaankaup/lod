#include "ShaderManager.h"

ShaderManager::ShaderManager()
{
    //ctor
}

ShaderManager::~ShaderManager()
{
    //dtor
}

ShaderManager& ShaderManager::getInstance()
{
    static ShaderManager instance;
    return instance;
}

void ShaderManager::createShader(const std::vector<std::string>& sources, const std::string& nimi)
{
    ShaderPtr s(new Shader());
    s->createFromSource(sources);
    shaderit_.push_back(std::make_pair(nimi,std::move(s)));
}

Shader* ShaderManager::getShader(const std::string& nimi) const
{
    for (const auto& obj : shaderit_)
    {
        if (std::get<0>(obj) == nimi)
        {
            return std::get<1>(obj).get();
        }
    }
    throw std::runtime_error("ShaderManager::createShader -> ei loytynyt shaderia nimella '" + nimi + "'.");
}
