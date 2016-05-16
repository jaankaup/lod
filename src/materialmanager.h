#ifndef MATERIALMANAGER_H
#define MATERIALMANAGER_H

#include <iostream>
#include <vector>
#include <stdexcept>

struct MaterialInfo {
    std::string nimi;
    std::string shaderName;
    std::string textureName;
};

class MaterialManager
{
    public:
        /// Antaa viitteen instanssiin.
        static MaterialManager& getInstance();
        /// Luo materiaalin.
        void create(const MaterialInfo& info);
        /// Palauttaa MaterialInfon.
        MaterialInfo getMaterialInfo(const std::string& nimi) const;

    private:
        MaterialManager();
        ~MaterialManager();

        std::vector<MaterialInfo> infot_;
};

#endif // MATERIALMANAGER_H
