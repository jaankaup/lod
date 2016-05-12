#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <array>
#include <memory>
#include <GL/glew.h>
#include "texture.h"
#include "global.h"
class Texture;

using TexturePtr = std::unique_ptr<Texture>;

/**
 * Singleton luokka tekstuurien k�ytt�� varten.
 *
 * Esimerkki k�ytt�:
 *
 * // Luodaan kolme tekstuuria. TextureManager oliota ei tarvitse luoda erikseen. Se syntyy kun ensimm�isen kerran kutsutaan TextureManager::getInstance()
 * // ja kuolee kun ohjelman suoritus loppuu. getInstance():lla saadaan aina viite TextureManager olioon.
 * TextureManager::getInstance().createTexture("data/textures/groungrass.jpg","isoruoho");
 * TextureManager::getInstance().createTexture("data/textures/barrel.jpg","tynnyri");
 * TextureManager::getInstance().createTexture("data/textures/wierd.jpg","hassu");
 *
 *
 */
class TextureManager
{
    public:
         /// Antaa viitteen instanssiin.
         static TextureManager& getInstance();
         /// Luo tekstuurin.
         void createTexture(const std::string& fileLocation, const std::string& nimi);
         /// Palauttaa pointerinTextureen.
         Texture* getTexture(const std::string& nimi) const;

    private:
        TextureManager();
        ~TextureManager();

        /// Sis�lt�� tekstuurit ja niit� vastaavat nimet.
        std::vector<std::pair<const std::string, TexturePtr>> tekstuurit_;
};

#endif // TEXTUREMANAGER_H
