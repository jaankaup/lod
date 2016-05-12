#ifndef MAINAPPLICATION_H
#define MAINAPPLICATION_H

#include <vector>
#include <SDL2/SDL.h>
#include "window.h"
#include "TextureManager.h"
#include "ShaderManager.h"
#include "model.h"
#include "Camera.h"
#include "renderer.h"
#include "materialmanager.h"

/**
 * P‰‰applikaatio.
 */
class MainApplication
{
    public:
        MainApplication();
        ~MainApplication();

        /// Alustaa kaikki tarpeelliset jutut.
        void initialize();
        /// T‰h‰n piirt‰miseen liittyvi‰ juttuja.
        void render();
        /// P‰ivitykset.
        void update(float delta);
        /// Eventit.
        void handleEvents();
        /// P‰‰silmukka.
        void run();

    private:
        Window window_;
        std::vector<std::unique_ptr<Model>> models_;
        bool running_ = true;
        Renderer renderer_;
        Camera camera_ = Camera(glm::vec3(0.0f,100.0f,130.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
        unsigned int time_ = 0;

        void glThings() const;
        void loadTextures();
        void loadShaders();
        void loadModels();
        void loadMaterials();
};

#endif // MAINAPPLICATION_H
