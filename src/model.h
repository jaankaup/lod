#ifndef MODEL_H
#define MODEL_H

#include <memory>
#include <vector>
#include <glm/glm.hpp>
#include "indexbuffer.h"
#include "vertexbuffer.h"
#include "vao.h"
#include "modelparser.h"
#include "command.h"
#include "ShaderManager.h"
#include "materialmanager.h"
#include "TextureManager.h"

class Model
{
    friend class Renderer; //POISTA!!

    public:
        Model() = default;
        ~Model();
        //Model() : vbo_(GL_ARRAY_BUFFER,GL_STATIC_DRAW) {};
        /*
        Model(const Model&) = delete;
        Model(Model&&) = delete;
        Model& operator=(const Model&) = delete;
        Model& operator=(Model&&) = delete;
        */
        void create(const std::string& fileLocation);
        VAO& getVAO();
        VertexBuffer& getVertexBuffer();
        IndexBuffer& getIndexBuffer();
        std::vector<Command> createCommands() const;
        void setModelMatrix(const glm::mat4& matrix);
        void setTextureTiling(float tilingFactor);
        //glm::mat4 getModelMatrix() const;


    private:

        VAO vao_;
        VertexBuffer vbo_;
        IndexBuffer ibo_;
        std::vector<Material> material_;
        glm::mat4 model_ = glm::mat4(1.0f);
        float textureTiling_ = 1.0f;
};

#endif // MODEL_H
