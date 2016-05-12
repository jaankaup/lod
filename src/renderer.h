#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "model.h"
#include "shader.h"
#include "texture.h"
#include "materialmanager.h"
#include "ShaderManager.h"
#include "TextureManager.h"
#include "global.h"



class Model;

enum class RenderMode {WIRE = 0, NORMAL = 1};
enum class TesselationMode {NONE = 0, JOKUTESSELAATIO = 1};

class Renderer
{
    public:
        Renderer();
        ~Renderer();

        void initialize();

        /// Ottaa modelilta tarvittavat jutut talteen piirtamista varten.
        void createCommand(const Model* model);
        /// Jarjestaa asioita. EI TOTEUTETTU!
        void sortCommands();
        /// Suorittaa piirtamisen. EI TOTEUTETTU!
        void drawCommands();
        /// Tyhjentaa command listan.
        void clearCommands();

        void render();
        void setProjectionMatrix(const glm::mat4& proj);
        void setViewMatrix(const glm::mat4& view);
        void setRenderMode(const RenderMode renderMode); // EI TOTEUTETTU
        void setTesselationMode(const TesselationMode TesselationMode); // EI TOTEUTETTU
        void toggleWireframe();

    private:

        std::vector<Command> commands_;
        Command mask_;
        glm::mat4 projection_ = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 300.f);
        glm::mat4 view_;
        bool viewChanged_ = false;
        glm::vec3 lightPosition_ = glm::vec3(20.0f,20.0f,20.0f);
        Shader* s_ = 0;
        Texture* t_ = 0;
        RenderMode renderMode_ = RenderMode::NORMAL;

        bool wireframe = false;
};

#endif // RENDERER_H
