#ifndef TEXTURECONTROL_H
#define TEXTURECONTROL_H
#include <array>
#include <memory>
#include <GL/glew.h>
#include "texture.h"
#include "global.h"
class Texture;

using TexturePtr = std::unique_ptr<Texture>;

/**
 * Singleton luokka tekstuurien luontia varten.
 * K�ytt�: luo ensin ilmentym� luokasta getInstance();
 * T�m�n j�lkeen luo tekstuuri createTexture() funtiolla.
 * Esim. TextureControl::getInstance().createTexture("data/textures/joo.jpg", "jootekstuurin_nimi");
 * TODO: tarkasta yl�puolinen esimerkki.
 */
class TextureControl
{

    public:
         /// Antaa viitteen instanssiin.
         static TextureControl& getInstance();
         /// Luo textuurin.
         TexturePtr createTexture(const std::string& fileLocation, const std::string& name);

    private:
         TextureControl();
         ~TextureControl();

         // friend void releaseUnit(const unsigned int unit);

         std::array<bool, 16> units_;
};

#endif // TEXTURECONTROL_H
