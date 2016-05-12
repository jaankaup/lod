#include "mainapplication.h"

MainApplication::MainApplication()
{
    //ctor
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

MainApplication::~MainApplication()
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainApplication::initialize()
{
    window_ = Window("LOD project", 800,600,true,false);
    //glThings();
    loadMaterials();
    loadTextures();
    loadShaders();
    loadModels();
    renderer_.initialize();
    renderer_.setProjectionMatrix(glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 300.f));
    renderer_.setViewMatrix(camera_.getMatrix());
    time_ = SDL_GetTicks();
    /* Tassa projektissa kun ei ole dynaamisia kappaleita, niin muodostetaan commandit vain kerran. Jos kappaleita liikutellaan tai lisataan
       dynaamisesti, taytyy taman olla esim update funktiossa. */
    for (const auto& model : models_)
    {
        renderer_.createCommand(model.get());
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainApplication::render()
{
    renderer_.render();
    window_.swapBuffers();

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainApplication::update(float delta)
{
    /* Välitetään päivityskomento kameralle */
    camera_.update(delta);
    renderer_.setViewMatrix(camera_.getMatrix());
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainApplication::handleEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            running_ = false;
            break;

        /* Kameralle */
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
            camera_.handleMouseInput(e);

        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
                case SDLK_1:
                    //textureP2_->bind();
                    //shader_ -> setUniform(std::string("myTextureSampler"), 0);
                    break;

                case SDLK_2:
                    //textureP1_->bind();
                    //shader_ -> setUniform(std::string("myTextureSampler"), 0);
                    break;

                case SDLK_3:
                    //textureP3_->bind();
                    //shader_ -> setUniform(std::string("myTextureSampler"), 0);
                    break;

                case SDLK_SPACE: /* Kameralle */
                    camera_.toggleMode();
                    break;
                case SDLK_F5: /* Kameralle */
                    camera_.adjustSpeed(-1.0);
                    break;
                case SDLK_F6: /* Kameralle */
                    camera_.adjustSpeed(1.0);
                    break;
                case SDLK_F7: /* Kameralle */
                    camera_.adjustSensitivity(-0.1);
                    break;
                case SDLK_F8: /* Kameralle */
                    camera_.adjustSensitivity(0.1);
                    break;
                case SDLK_q: /* Wireframe */
                    renderer_.toggleWireframe();
                    break;
                case SDLK_ESCAPE: /* Wireframe */
                    running_ = false;
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainApplication::run()
{
    while (running_)
    {
        unsigned int timeNow = SDL_GetTicks();
        unsigned int delta = timeNow - time_;
        if (delta != 0)
        {
            update(((float)timeNow)/((float)time_));
        }
        time_ = timeNow; //Kun kommentit ottaa pois, niin bugi katoaa, mutta talloin kamera ei toimi. Kamera on ottanut bugin huomioon.
        render();
        handleEvents();
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainApplication::glThings() const
{

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainApplication::loadTextures()
{
    // TODO:: lisaa puuttuvat tekstuurit!
    TextureManager::getInstance().createTexture("data/textures/dirt.jpg","ruoho");
    TextureManager::getInstance().createTexture("data/textures/barrel.jpg","tynnyri");
    TextureManager::getInstance().createTexture("data/textures/rock.jpg","kivi");
    TextureManager::getInstance().createTexture("data/textures/house_roof.jpg","katto");
    TextureManager::getInstance().createTexture("data/textures/house_wall.jpg","seina");
    TextureManager::getInstance().createTexture("data/textures/house_wood_parts.jpg","puuosat");
    TextureManager::getInstance().createTexture("data/textures/barrel.jpg","manty");

    /* WIREFRAME */
    TextureManager::getInstance().createTexture("data/textures/wireframe.jpg","wireframe");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainApplication::loadShaders()
{
    std::vector<std::string> shaderSources = {"data/shaders/StandardShading.vert", "data/shaders/StandardShading.frag"};
    ShaderManager::getInstance().createShader(shaderSources, "ufovalo");

    std::vector<std::string> shaderSources2 = {"data/shaders/testShader.vert", "data/shaders/testShader.frag"};
    ShaderManager::getInstance().createShader(shaderSources2, "test");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainApplication::loadModels()
{
    std::unique_ptr<Model> m(new Model());
    m->setModelMatrix(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)), glm::vec3(0.0f, 0.0f, -15.0f)));
    m->create("data/models/Semi_truck.obj");
    models_.push_back(std::move(m));

    std::unique_ptr<Model> m2(new Model());
    m2->create("data/models/terrain.obj");
    models_.push_back(std::move(m2));

    std::unique_ptr<Model> m3(new Model());
    m3->setModelMatrix(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(5.0f)), glm::vec3(0.0f, 0.0f, -15.0f)));
    m3->create("data/models/house.obj");
    models_.push_back(std::move(m3));

    std::unique_ptr<Model> m4(new Model());
    m4->setModelMatrix(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(5.0f)), glm::vec3(12.0f, 1.5f, 5.0f)));
    m4->create("data/models/barrel.obj");
    models_.push_back(std::move(m4));

    std::unique_ptr<Model> m5(new Model());
    m5->setModelMatrix(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(5.0f)), glm::vec3(12.0f, 1.5f, -15.0f)));
    m5->create("data/models/rock.obj");
    models_.push_back(std::move(m5));

    std::unique_ptr<Model> m6(new Model());
    m6->setModelMatrix(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(5.0f)), glm::vec3(8.0f, 1.0f, -8.0f)));
    m6->create("data/models/tree.obj");
    models_.push_back(std::move(m6));

    std::unique_ptr<Model> m7(new Model());
    m7->setModelMatrix(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(5.0f)), glm::vec3(-12.0f, 1.0f, 12.0f)));
    m7->create("data/models/mill.obj");
    models_.push_back(std::move(m7));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainApplication::loadMaterials()
{
    MaterialInfo puu;
    puu.nimi = "barrel";
    puu.shaderName = "ufovalo";
    puu.textureName = "tynnyri";
    MaterialManager::getInstance().create(puu);

    MaterialInfo ruohikko;
    ruohikko.nimi = "terrain";
    ruohikko.shaderName = "ufovalo";
    ruohikko.textureName = "ruoho";
    MaterialManager::getInstance().create(ruohikko);

    MaterialInfo kivi;
    kivi.nimi = "rock";
    kivi.shaderName = "ufovalo";
    kivi.textureName = "kivi";
    MaterialManager::getInstance().create(kivi);

    MaterialInfo katto;
    katto.nimi = "house_roof";
    katto.shaderName = "ufovalo";
    katto.textureName = "katto";
    MaterialManager::getInstance().create(katto);

    MaterialInfo seina;
    seina.nimi = "house_wall";
    seina.shaderName = "ufovalo";
    seina.textureName = "seina";
    MaterialManager::getInstance().create(seina);

    MaterialInfo puuosat;
    puuosat.nimi = "house_wood_parts";
    puuosat.shaderName = "ufovalo";
    puuosat.textureName = "puuosat";
    MaterialManager::getInstance().create(puuosat);

    MaterialInfo manty;
    manty.nimi = "tree_leaves";
    manty.shaderName = "ufovalo";
    manty.textureName = "manty";
    MaterialManager::getInstance().create(manty);

    MaterialInfo manty2;
    manty2.nimi = "tree_bark";
    manty2.shaderName = "ufovalo";
    manty2.textureName = "manty";
    MaterialManager::getInstance().create(manty2);

    MaterialInfo truck;
    truck.nimi = "Semi_truck.tga";
    truck.shaderName = "ufovalo";
    truck.textureName = "manty";
    MaterialManager::getInstance().create(truck);

    MaterialInfo truckGlass;
    truckGlass.nimi = "car_glass.tga";
    truckGlass.shaderName = "ufovalo";
    truckGlass.textureName = "manty";
    MaterialManager::getInstance().create(truckGlass);
}
