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
    s_ = ShaderManager::getInstance().getShader("test"); // alustetaan s_ ufovalolla. Ufo valo taytyy olla olemassa. TODO:: huono ratkaisu!
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
    //glEnable(GL_MULTISAMPLE);
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

    if (s_ != 0) s_->setUniform("LightPosition_worldspace", glm::vec3(10.0f,50.0f,10.0f));
    //if (s_ != 0) s_->setUniform("LightPosition_worldspace", eyePosition_);

    if (commands_.size() == 0)
    {
        logWarning.log("Renderer::render() -> ei ole commandeja!");
        return;
    }

    for (const auto& com : commands_)
    {
        //s_->setUniform("gEyeWorldPos", eyePosition_);
        //logDebug.log("eyePosition_ = (%,%,%)", eyePosition_.x, eyePosition_.y,eyePosition_.z);
        if (mask_.shader != com.shader)
        {
            if (tessellationMode_ == TesselationMode::NONE)
            {
                mask_.shader = "ufovalo";
                s_ = ShaderManager::getInstance().getShader("ufovalo");
                s_->use();
            }
            else
            {
                mask_.shader = com.shader;
                s_ = ShaderManager::getInstance().getShader(com.shader);
            }
            s_->use();
            if (s_ != 0) s_->setUniform("gEyeWorldPos", eyePosition_);
        }
        if (mask_.texture != com.texture)
        {
            mask_.texture = com.texture;

            /* Tekstuurin vaihto suoritusaikana */
            if (com.texture == "soil" && mask_.texture != currentTerrainTex)
            {
                mask_.texture = currentTerrainTex;
                t_ = TextureManager::getInstance().getTexture(currentTerrainTex);
            }
            else
            {
                /* Perustekstuuri */
                t_ = TextureManager::getInstance().getTexture(com.texture);
            }

            /* If-ketjua voisi optimoida */
            if (wireframe)
                t_ = TextureManager::getInstance().getTexture("wireframe");

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
            //s_->setUniform("normalMatrix", calculateNormalMatrix(com.model));
            s_->setUniform("MVP", projection_ * view_ * com.model);
            s_->setUniform("MV", view_ * com.model);
            s_->setUniform("M",com.model);
            s_->setUniform("V",view_);
            //s_->setUniform("P",projection_);
            s_->setUniform("VP",projection_*view_/*glm::vec4(1.0f)*/);
            mask_.model = com.model;
            viewChanged_ = false;
        }


        if (mask_.shader == "tessellation")
        {
            //logDebug.log("renderoidaan tesselaatiota");
            //logDebug.log("eyePosition_ = (%,%,%)", eyePosition_.x, eyePosition_.y,eyePosition_.z);
            s_->setUniform("displacement_factor", displacement_factor_);
            //s_->setUniform("gEyeWorldPos", eyePosition_);
            Texture* displace = TextureManager::getInstance().getTexture("displace" + mask_.texture);
            displace->use(1);
            s_->setUniform("dispTexture", 1);
            glPatchParameteri(GL_PATCH_VERTICES,3);
            glDrawElements(GL_PATCHES,
                           com.indexCount,
                           GL_UNSIGNED_INT,
                           (void*)(sizeof(unsigned int)*com.startIndex));
        }
        //logDebug.log("renderoidaan muita");
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

void Renderer::setTesselationMode(int newTesselationMode)
{
    switch (newTesselationMode) {
        case 0 : tessellationMode_ = TesselationMode::NONE; break;
        case 1 : tessellationMode_ = TesselationMode::ENABLED; break;
        default: break;
    }
    logInfo.log("Tessellation mode: %", (int)tessellationMode_);
}

void Renderer::setEyePosition(const glm::vec3& pos)
{
    eyePosition_ = pos;
}

glm::mat3 Renderer::calculateNormalMatrix(const glm::mat4& model)
{
    return glm::transpose(glm::inverse(glm::mat3(view_ * model)));
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

/**
 * Muuttaa displacement-kerrointa
 * @param adjust muutos
 */
void Renderer::adjustDisplacement(const float &adjust)
{
    displacement_factor_ += adjust;
    //displacement_factor_ = glm::clamp(displacement_factor_, -1.0f, 1.0f);
    logDebug.log("Displacement factor: %", displacement_factor_);
}

void Renderer::setTerrainTex(const std::string &textureName)
{
    currentTerrainTex = textureName;
}
