#include "renderer.h"

Renderer::Renderer()
{
    commands_.reserve(5000);
}

Renderer::~Renderer()
{

}

void Renderer::initialize()
{
    s_ = ShaderManager::getInstance().getShader("ufovalo"); // alustetaan s_ ufovalolla. Ufo valo taytyy olla olemassa. TODO:: huono ratkaisu!
    setRenderMode(RenderMode::NORMAL);
    /* Alustetaan maski laittomilla arvoilla. Nama siis muuttuvat, kun renderer alkaa piirtaa commmandeja.
       Nyt heittaa herjaa jos command maskia ei koskaan paiviteta. */
    mask_.indexCount = -9999;
    mask_.model = glm::mat4(-1.0f);
    mask_.shader = "eipa_ole_olemassa_tallaista_shaderia";
    mask_.texture = "eipa_ole_olemassa_tallaista_tekstuuria";
    mask_.startIndex = -4949;
    mask_.vao = 191919;

    glEnable(GL_DEPTH_TEST); // Enable depth test
    glDepthFunc(GL_LESS); // Accept fragment if it closer to the camera than the former one
    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);
}

void Renderer::createCommand(const Model* model)
{
    for (const auto& obj : model->createCommands())
    {
        commands_.push_back(obj);
    }
}

void Renderer::clearCommands()
{
    commands_.clear();
}

void Renderer::render()
{
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    /* Tahan laitettu valon paivitys. Valon muutosta ei tarkasteta, mika aiheuttaa nyt set, etta kutsutaan turhaan setUniformia. */

    if (s_ != 0) s_->setUniform("LightPosition_worldspace", lightPosition_);

    if (commands_.size() == 0)
    {
        logWarning.log("Renderer::render() -> ei ole commandeja!");
        return;
    }

    for (const auto& com : commands_)
    {
        if (mask_.shader != com.shader)
        {
            mask_.shader = com.shader;
            s_ = ShaderManager::getInstance().getShader(com.shader);
            s_->use();
        }

        if (wireframe) /* WIREFRAME */
        {
            mask_.texture = "wireframe";
            t_ = TextureManager::getInstance().getTexture("wireframe");
            t_->use(0);
            s_->setUniform("myTextureSampler", 0);
        }
        else if (mask_.texture != com.texture)
        {
            mask_.texture = com.texture;
            t_ = TextureManager::getInstance().getTexture(com.texture);
            t_->use(0);
            s_->setUniform("myTextureSampler", 0);
        }

        if (mask_.vao != com.vao)
        {
            glBindVertexArray(com.vao);
            mask_.vao = com.vao;
        }

        if ((mask_.model != com.model) || viewChanged_)
        {
            s_->setUniform("MVP", projection_ * view_ * com.model);
            s_->setUniform("MV", view_ * com.model);
            s_->setUniform("M",com.model);
            s_->setUniform("V",view_);
            mask_.model = com.model;
            viewChanged_ = false;
        }

        glDrawElements(GL_TRIANGLES,
                       com.indexCount,
                       GL_UNSIGNED_INT,
                       (void*)(sizeof(unsigned int)*com.startIndex));
    }
}

void Renderer::setProjectionMatrix(const glm::mat4& proj)
{
    projection_ = proj;
}

void Renderer::setViewMatrix(const glm::mat4& view)
{
    view_ = view;
    viewChanged_ = true;
}

void Renderer::setRenderMode(const RenderMode renderMode)
{

}

void Renderer::setTesselationMode(const TesselationMode TesselationMode)
{

}

/**
 * Wireframe p‰‰lle.
 * Yksinkertaistettu, ei vaihda tekstuureja/shadereita.
 */
void Renderer::toggleWireframe()
{
    if (!wireframe)
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
        wireframe = true;
    }
    else
    {
        glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
        wireframe = false;
    }
}
