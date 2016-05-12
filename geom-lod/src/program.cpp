#include <vector>
#include <array>
#include <math.h>
#include "program.h"
#include "global.h"
#include "modelparser.h"

Program::~Program() {
   if (vao_ != 0) delete vao_; // only for testing.
   if (vao2_ != 0) delete vao_; // only for testing.
   if (vbo_ != 0) delete vbo_; // only for testing.
   if (shader_ != 0) delete shader_; // only for testing.
   if (texture_ != 0) delete texture_; // only for testing.
   if (ib_ != 0) delete ib_; // only for testing.
   if (ib2_ != 0) delete ib2_; // only for testing.
   if (vbo2_ != 0) delete vbo2_; // only for testing.
   if (modelUUS_ != 0) delete modelUUS_;
}

void Program::initialize()
{
    ModelParser mp;
    mp.parse("data/models/terrain.obj");

    ModelParser mp2;
    mp2.parse("data/models/barrel.obj");

    window_ = Window("Test!", 800,600,true,false);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // Nämä liittyy tekstuureihin.
    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    modelUUS_ = new Model();
    modelUUS_->create("data/models/terrain.obj");

    /* Luodaan shader. */
    //shader_ = new Shader(); // Just testing.

    /* Tähän shaderien sijainnit, joista sitten muodostetaan varsinainen varjostusohjelma. */
    std::vector<std::string> shaderSources = {"data/shaders/testShader.vert","data/shaders/testShader.frag"};
    //ShaderManager::getInstance().createShader(shaderSources,"test");
    //shader_ = ShaderManager::getInstance().getShader("test");
    //std::vector<std::string> shaderSources = {"data/shaders/kokeilu.vert","data/shaders/kokeilu.frag"};
    //std::vector<std::string> shaderSources = {"data/shaders/StandardShading.vert", "data/shaders/StandardShading.frag"};
    //std::vector<std::string> shaderSources = {"data/shaders/pah.vert", "data/shaders/pah.frag"};
    /* Rakennetaan shader. */
    shader_ -> createFromSource(shaderSources);
    /* Käytetään shaderia. */
    shader_ -> use();
    /* Luodaan ensimmäinen project view projectio. */
    glm::mat4 modelviewprojection = projection_ * camera_.getMatrix() * model_;
    /* Syötetään se shaderille. */
    shader_ -> setUniform(std::string("MVP"), modelviewprojection);
    shader_ -> setUniform(std::string("LightPosition_worldspace"), glm::vec3(28.0f,28.0f,28.0f));


    vao_ = new VAO();
    vao_-> bind();

    vbo_ = new VertexBuffer(); // Just testing!!!
    ib_ = new IndexBuffer();
    std::vector<float> dat = mp.getData();
    vbo_ -> setData(&dat[0], sizeof(float)*dat.size(), AttributeConfiguration::VTN_Interleaved);
    ib_ -> setData(mp.getIndexArray(), mp.getIndexSize(), mp.getIndiceCount());
    std::cout << mp.getIndiceCount() << std::endl;
    vao_->unBind();
    //vaoUus_.unBind();

    vao2_ = new VAO();

    vao2_->bind();

    vbo2_ = new VertexBuffer(); // Just testing!!!
    ib2_ = new IndexBuffer();

    std::vector<float> dat2 = mp2.getData();
    vbo2_ -> setData(&dat2[0], sizeof(float)*dat2.size(), AttributeConfiguration::VTN_Interleaved);
    ib2_ -> setData(mp2.getIndexArray(), mp2.getIndexSize(), mp2.getIndiceCount());
    vao2_->unBind();

    logInfo.log("vao_ = %, vao2_ = %, vbo_ = %, vbo2_ = %, ib_ = %, ib2_ = %",
                vao_->getID(), vao2_->getID(), vbo_->getID(), vbo2_->getID(), ib_->getID(),ib2_->getID());

    textureP1_ = TextureControl::getInstance().createTexture("data/textures/groungrass.jpg","dirt");
    textureP2_ = TextureControl::getInstance().createTexture("data/textures/cliff.jpg","cliff");
    textureP3_ = TextureControl::getInstance().createTexture("data/textures/grass.jpg","outo");
    textureP4_ = TextureControl::getInstance().createTexture("data/textures/barrel.jpg","tynnyri");

    TextureManager::getInstance().createTexture("data/textures/groungrass.jpg","isoruoho");

    /* Bindataan ensimmäinen, tällä hetkellä ainoa, tekstuuri. */
    textureP1_->bind();
    /* Isketään shaderille tieto tekstuurista. */
    //shader_ -> setUniform(std::string("myTextureSampler"), 0);
    shader_ -> setUniform(std::string("LightPosition"), glm::vec4(21.0f,21.0f,21.0f,1.0f));
    //shader_ -> setUniform(std::string("Kd"), glm::vec3(0.8f,0.8f,0.2f)); // Diffuse reflectivity
    //shader_ -> setUniform(std::string("Ld"), glm::vec3(0.5f,0.5f,0.4f)); // Light source intensity
    shader_->setUniform(std::string("DLight.Color"), glm::vec3(0.8f,0.6f,0.3f));
    shader_->setUniform(std::string("DLight.AmbientIntensity"), 0.7f);
    shader_->setUniform(std::string("DLight.Direction"), glm::vec3(40.0f,80.0f,40.0f));
    shader_->setUniform(std::string("DLight.DiffuseIntensity"), 0.2f);
    shader_->setUniform(std::string("Projection"), projection_);

    /* Alustetaan ohjelman aika. */
    time_ = SDL_GetTicks();
}

void Program::run()
{
    while (running_)
    {
        //SDL_Delay(10);
        unsigned int timeNow = SDL_GetTicks();
        unsigned int delta = timeNow - time_;
        if (delta != 0)
        {
            // Lisp koodia :).
            update(((float)timeNow)/((float)time_));
        }

        handleEvents();
        //update();
        render();
    }
}

void Program::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            running_ = false;
            break;

        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
                case SDLK_1:
                    textureP2_->bind();
                    shader_ -> setUniform(std::string("myTextureSampler"), 0);
                    break;

                case SDLK_2:
                    textureP1_->bind();
                    shader_ -> setUniform(std::string("myTextureSampler"), 0);
                    break;

                case SDLK_3:
                    textureP3_->bind();
                    shader_ -> setUniform(std::string("myTextureSampler"), 0);
                    break;
            }


        case SDL_WINDOWEVENT:
            switch(e.window.event)
            {
            case SDL_WINDOWEVENT_CLOSE:
                running_ = false;
                break;
            case SDL_WINDOWEVENT_RESIZED:
            {
                int w = e.window.data1;
                int h = e.window.data2;
                glViewport(0,0,w,h);
                break;
            }
            }
        }
    }
}

void Program::update(float f)
{
    float blaahX = 80.0f + 60.0f * sin(f); // En tiedä mitä tekee.
    float blyyhY = 80.0f + 60.0f * cos(f); // En tiedä mitä tekee.
    float bliihZ = 80.0f + 60.0f * sin(f) * cos(f); // Mitä tämä tekee? HT.

    camera_.translate(glm::vec3(blaahX,blyyhY,bliihZ)); // Testataan ohjelmaa. Ei muuta virkaa.
    /* Muodostetaan uusi päivitetty model view projektio. */
    glm::mat4 modelview = camera_.getMatrix() * model_;
    glm::mat4 modelviewprojection = projection_ * camera_.getMatrix() * model_;
    glm::mat4 temp = glm::transpose(glm::inverse(modelview));
    glm::mat3 normalMatrix = glm::mat3(glm::vec3(temp[0][0],temp[1][0],temp[2][0]),glm::vec3(temp[1][0],temp[1][1],temp[1][2]),glm::vec3(temp[2][0],temp[2][1],temp[2][2]));
    /* Lähetetään uusi päivitetty model view projektio shaderille. */
    shader_ -> setUniform(std::string("V"),camera_.getMatrix());
    shader_ -> setUniform(std::string("M"),model_);
    shader_ -> setUniform(std::string("MV"),camera_.getMatrix()*model_);
    shader_ -> setUniform(std::string("MVP"), modelviewprojection);

    shader_ -> setUniform(std::string("NormalMatrix"), normalMatrix);
    shader_ -> setUniform(std::string("ModelViewMatrix"), modelview);
    shader_ -> setUniform(std::string("ProjectionMatrix"), projection_);

    //Misc::calculateNormalMatrix(modelview);

        //uniform mat4 ModelViewMatrix;
    //uniform mat3 NormalMatrix;
    //uniform mat4 ProjectionMatrix;
    //uniform mat4 MVP; // Projection * ModelView
}

void Program::render()
{
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    shader_ -> use();
    shader_->setUniform(std::string("View"), camera_.getMatrix());
    shader_->setUniform(std::string("Model"), model_);
    /* Bindaillaan juttuja ennen piirtoa. */
    auto tex = TextureManager::getInstance().getTexture("isoruoho");
    tex->use(0);

    //textureP1_->bind();
    shader_ -> setUniform(std::string("myTextureSampler"), 0);
    //vaoUus_.bind();
    /*
    vao_ -> bind();
    glDrawElements(GL_TRIANGLES, ib_->getElementCount(), GL_UNSIGNED_INT, (void*)0);
    */
    /*
    logInfo.log("Model::vao = %, vbo = %, ibo = %", (modelUUS_->getVAO()).getID(),
                                                    (modelUUS_->getVertexBuffer()).getID(),
                                                    (modelUUS_->getIndexBuffer()).getID());
    */
    (modelUUS_->getVAO()).bind();
    glDrawElements(GL_TRIANGLES, (modelUUS_->getIndexBuffer()).getElementCount(), GL_UNSIGNED_INT, (void*)0);
    shader_->setUniform(std::string("Model"), model2_);
    vao2_ -> bind();
    textureP4_->bind();

    glm::mat4 modelview = camera_.getMatrix() * model_;
    glm::mat4 modelviewprojection = projection_ * camera_.getMatrix() * model2_;
    //glm::mat4 temp = glm::transpose(glm::inverse(modelview));
    /* Lähetetään uusi päivitetty model view projektio shaderille. */
    shader_ -> setUniform(std::string("V"),camera_.getMatrix());
    shader_ -> setUniform(std::string("M"),model2_);
    shader_ -> setUniform(std::string("MV"),camera_.getMatrix()*model2_);
    shader_ -> setUniform(std::string("MVP"), modelviewprojection);

    shader_ -> setUniform(std::string("myTextureSampler"), 1);
    glDrawElements(GL_TRIANGLES, ib2_->getElementCount(), GL_UNSIGNED_INT, (void*)0);
    window_.swapBuffers();
}
