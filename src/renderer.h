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
enum class TesselationMode {NONE = 0, ENABLED = 1};

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
        void setEyePosition(const glm::vec3& pos);
        void setViewMatrix(const glm::mat4& view);
        void setRenderMode(const RenderMode renderMode); // EI TOTEUTETTU
        void setTesselationMode(const TesselationMode TesselationMode); // EI TOTEUTETTU
        void setTesselationMode(int mode);
        void toggleWireframe();
        void adjustDisplacement(const float &adjust);
        void setTerrainTex(const std::string &textureName);

    private:

        std::vector<Command> commands_;
        Command mask_;
        glm::mat4 projection_ = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 300.f);
        glm::mat4 view_;
        bool viewChanged_ = false;
        glm::vec3 lightPosition_ = glm::vec3(20.0f,20.0f,20.0f);
        glm::vec3 eyePosition_;
        Shader* s_ = 0;
        Texture* t_ = 0;
        RenderMode renderMode_ = RenderMode::NORMAL;
        TesselationMode tessellationMode_ = TesselationMode::ENABLED;
        float displacement_factor_ = 0.25f;

        /// Laskee normaali-matriisin.
        glm::mat3 calculateNormalMatrix(const glm::mat4& model);

        bool wireframe = false;

        /// Tekstuurin vaihto suoritusaikana
        std::string currentTerrainTex = "soil";
};

#endif // RENDERER_H
