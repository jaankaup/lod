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
 * Pääapplikaatio.
 */
class MainApplication
{
    public:
        MainApplication();
        ~MainApplication();

        /// Alustaa kaikki tarpeelliset jutut.
        void initialize();
        /// Tähän piirtämiseen liittyviä juttuja.
        void render();
        /// Päivitykset.
        void update(float delta);
        /// Eventit.
        void handleEvents();
        /// Pääsilmukka.
        void run();

    private:
        Window window_;
        std::vector<std::unique_ptr<Model>> models_;
        bool running_ = true;
        Renderer renderer_;
        Camera camera_ = Camera(glm::vec3(50.2f,50.2f,50.2f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
        unsigned int time_ = 0;

        unsigned int frametimeSum = 0;
        unsigned int frametimeCounter = 0;

        void glThings() const;
        void loadTextures();
        void loadShaders();
        void loadModels();
        void loadMaterials();
};

#endif // MAINAPPLICATION_H
