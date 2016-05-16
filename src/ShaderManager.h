#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include "shader.h"

using ShaderPtr = std::unique_ptr<Shader>;

class ShaderManager
{
    public:
        /// Antaa viitteen instanssiin.
        static ShaderManager& getInstance();
        /// Luo shaderin.
        void createShader(const std::vector<std::string>& fileLocations, const std::string& nimi);
        /// Palauttaa const pointerin const Shaderiin.
        Shader* getShader(const std::string& nimi) const;

    private:
        ShaderManager();
        ~ShaderManager();

        /// Sis‰lt‰‰ shaderit ja niit‰ vastaavat nimet.
        std::vector<std::pair<const std::string, ShaderPtr>> shaderit_;
};

#endif // SHADERMANAGER_H
