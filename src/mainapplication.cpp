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
    renderer_.setProjectionMatrix(glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 600.f));
    renderer_.setViewMatrix(camera_.getMatrix());
    time_ = SDL_GetTicks();
    /* Tassa projektissa kun ei ole dynaamisia kappaleita, niin muodostetaan commandit vain kerran. Jos kappaleita liikutellaan tai lisataan
       dynaamisesti, taytyy taman olla esim update funktiossa. */
    for (const auto& model : models_)
    {
        renderer_.createCommand(model.get());
    }

    /*
    Shader* s = ShaderManager::getInstance().getShader("test");
    s->setUniform("gNumPointLights",0);
    s->setUniform("gNumSpotLights",2);

    s->setUniform("gDirectionalLight.Base.Color",glm::vec3(1.0f,1.0f,1.0f));
    s->setUniform("gDirectionalLight.Base.AmbientIntensity",0.0f);
    s->setUniform("gDirectionalLight.Base.DiffuseIntensity",0.2f);
    s->setUniform("gDirectionalLight.Direction",glm::vec3(0.0f,0.0f,1.0f));

    s->setUniform("gSpotLights[0].Base.Base.Color",glm::vec3(0.0f, 1.0f, 1.0f));
    s->setUniform("gSpotLights[0].Base.Base.AmbientIntensity",0.01f);
    s->setUniform("gSpotLights[0].Base.Base.DiffuseIntensity",0.9f);
    s->setUniform("gSpotLights[0].Base.Position",glm::vec3(5.0f,10.0f,12.0f));
    s->setUniform("gSpotLights[0].Base.Atten.Linear",0.1f);
    s->setUniform("gSpotLights[0].Direction",glm::vec3(0.0f, -1.0f, 0.0f));
    s->setUniform("gSpotLights[0].Cutoff",10.0f);

    s->setUniform("gSpotLights[1].Base.Base.Color",glm::vec3(0.0f, 1.0f, 1.0f));
    s->setUniform("gSpotLights[1].Base.Base.AmbientIntensity",0.01f);
    s->setUniform("gSpotLights[1].Base.Base.DiffuseIntensity",0.9f);
    s->setUniform("gSpotLights[1].Base.Position",glm::vec3(-5.0f,5.0f,-12.0f));
    s->setUniform("gSpotLights[1].Base.Atten.Linear",0.1f);
    s->setUniform("gSpotLights[1].Direction",glm::vec3(0.0f, -1.0f, 0.0f));
    s->setUniform("gSpotLights[1].Cutoff",20.0f);

    s->setUniform("gPointLights[0].Base.Color",glm::vec3(1.0f, 0.5f, 0.0f));
    s->setUniform("gPointLights[0].Base.DiffuseIntensity",0.25f);
    s->setUniform("gPointLights[0].Position",glm::vec3(5.0f, 3.0f, 10.0f));
    s->setUniform("gPointLights[0].Atten.Linear",0.1f);

    s->setUniform("gPointLights[1].Base.Color",glm::vec3(1.0f, 1.0f, 1.0f));
    s->setUniform("gPointLights[1].Base.DiffuseIntensity",0.9f);
    s->setUniform("gPointLights[1].Position",glm::vec3(5.0f, 3.0f, 10.0f));
    s->setUniform("gPointLights[1].Atten.Linear",0.1f);

    s->setUniform("gMatSpecularIntensity",1.0f);
    s->setUniform("gSpecularPower",32.0f);

    //s->setUniform("gMatSpecularIntensity",0.0f);
    //s->setUniform("gSpecularPower",0.0f);
    */
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
    renderer_.setEyePosition(camera_.getPosition());
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

        case SDL_KEYDOWN:
            switch (e.key.keysym.sym)
            {
                case SDLK_1:
                    renderer_.setTesselationMode(0);
                    break;
                case SDLK_2:
                    renderer_.setTesselationMode(1);
                    break;
                case SDLK_r:
                case SDLK_F3: /* Displacement */
                    renderer_.adjustDisplacement(0.05);
                    break;
                case SDLK_t:
                case SDLK_F4: /* Displacement */
                    renderer_.adjustDisplacement(-0.05);
                    break;

                case SDLK_4: /* Terrain texture */
                    renderer_.setTerrainTex("soil");
                    break;
                case SDLK_5: /* Terrain texture */
                    renderer_.setTerrainTex("dirt");
                    break;
                case SDLK_6: /* Terrain texture */
                    renderer_.setTerrainTex("brick");
                    break;
                case SDLK_7: /* Terrain texture */
                    renderer_.setTerrainTex("stones");
                    break;

                case SDLK_SPACE: /* Kamera */
                    camera_.toggleMode();
                    break;
                case SDLK_F5: /* Kamera */
                    camera_.adjustSpeed(-1.0);
                    break;
                case SDLK_F6: /* Kamera */
                    camera_.adjustSpeed(1.0);
                    break;
                case SDLK_F7: /* Kamera */
                    camera_.adjustSensitivity(-0.1);
                    break;
                case SDLK_F8: /* Kamera */
                    camera_.adjustSensitivity(0.1);
                    break;

                case SDLK_q: /* Wireframe */
                    renderer_.toggleWireframe();
                    break;
                case SDLK_ESCAPE: /* Sulje */
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

        /* Kameralle */
        case SDL_MOUSEMOTION:
        case SDL_MOUSEBUTTONDOWN:
            camera_.handleMouseInput(e);
            break;

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

            /// FPS
            frametimeSum += delta;
            frametimeCounter++;
            if (frametimeCounter >= 30)
            {
                // FPS pyöristetään lähimpään kokonaislukuun
                unsigned int fps = glm::round(1000/(frametimeSum/float(frametimeCounter)));
                window_.setTitle("LOD", fps);
                frametimeSum = 0;
                frametimeCounter = 0;
            }
        }
        time_ = timeNow;
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
    // Wireframe
    TextureManager::getInstance().createTexture("data/textures/wireframe.jpg","wireframe");

    // Terrain base - jos  on käytössä, tekstuuri vaihdetaan suoritusaikana rendererin toimesta
    TextureManager::getInstance().createTexture("data/textures/terrainbase.jpg","terrainbase");
    TextureManager::getInstance().createTexture("data/textures/displaceterrainbase.jpg","displaceterrainbase");

    // Cube + displacement
    TextureManager::getInstance().createTexture("data/textures/cube.jpg","cube");
    TextureManager::getInstance().createTexture("data/textures/displacecube.jpg","displacecube");

    TextureManager::getInstance().createTexture("data/textures/barrel.jpg","manty");
    TextureManager::getInstance().createTexture("data/textures/barrel.jpg","tynnyri");
    TextureManager::getInstance().createTexture("data/textures/rock.jpg","kivi");

    TextureManager::getInstance().createTexture("data/textures/dirt.jpg","dirt");
    TextureManager::getInstance().createTexture("data/textures/displacedirt.jpg","displacedirt");

    TextureManager::getInstance().createTexture("data/textures/house_roof.jpg","house_roof");
    TextureManager::getInstance().createTexture("data/textures/displacehouse_roof.jpg","displacehouse_roof");

    TextureManager::getInstance().createTexture("data/textures/house_wall.jpg","house_wall");
    TextureManager::getInstance().createTexture("data/textures/displacehouse_wall.jpg","displacehouse_wall");

    TextureManager::getInstance().createTexture("data/textures/house_wood_parts.jpg","house_wood_parts");
    TextureManager::getInstance().createTexture("data/textures/displacehouse_wood_parts.jpg","displacehouse_wood_parts");

    TextureManager::getInstance().createTexture("data/textures/soil_cracked_0026_01.jpg","soil");
    TextureManager::getInstance().createTexture("data/textures/displacesoil_cracked_0026_01.jpg","displacesoil");

    TextureManager::getInstance().createTexture("data/textures/brick_medieval_0116_01.jpg","brick");
    TextureManager::getInstance().createTexture("data/textures/displacebrick_medieval_0116_01.jpg","displacebrick");

    TextureManager::getInstance().createTexture("data/textures/ground_stone_ground_0018_01.jpg","stones");
    TextureManager::getInstance().createTexture("data/textures/displaceground_stone_ground_0018_01.jpg","displacestones");
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainApplication::loadShaders()
{
    std::vector<std::string> shaderSources = {"data/shaders/StandardShading.vert", "data/shaders/StandardShading.frag"};
    ShaderManager::getInstance().createShader(shaderSources, "ufovalo");

    std::vector<std::string> shaderSources2 = {"data/shaders/testShader.vert", "data/shaders/testShader.frag"};
    ShaderManager::getInstance().createShader(shaderSources2, "pah");

    std::vector<std::string> shaderSources3 = {"data/shaders/lightingNoTessellation.vert", "data/shaders/StandardShading.frag"};
    ShaderManager::getInstance().createShader(shaderSources3, "test");

    std::vector<std::string> shaderSources4 =
       {"data/shaders/tessellation.vert", "data/shaders/tessellation.gtcs",
        "data/shaders/tessellation.gtes", "data/shaders/tessellation.frag"};
    ShaderManager::getInstance().createShader(shaderSources4, "tessellation");

    /*
    std::vector<std::string> shaderSources4 = {"data/shaders/lighting.vert", "data/shaders/lighting.frag",
                                               "data/shaders/lighting.gtes","data/shaders/lighting.gtcs"};
    ShaderManager::getInstance().createShader(shaderSources4, "tessellation2");
    */
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void MainApplication::loadModels()
{
    std::unique_ptr<Model> m2(new Model());
    m2->setTextureTiling(8.0);
    m2->create("data/models/terrain.obj");
    models_.push_back(std::move(m2));

    std::unique_ptr<Model> cube(new Model());
    cube->setModelMatrix(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(2.0f)), glm::vec3(0.0f, 20.0f, 0.0f)));
    cube->create("data/models/cube_simple.obj");
    models_.push_back(std::move(cube));

    std::unique_ptr<Model> m(new Model());
    m->setModelMatrix(glm::translate(glm::scale(glm::mat4(1.0f), glm::vec3(0.1f)), glm::vec3(0.0f, 0.0f, -15.0f)));
    m->create("data/models/Semi_truck.obj");
    models_.push_back(std::move(m));

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
    // Cube + displacement
    MaterialInfo cube;
    cube.nimi = "cube_material";
    cube.shaderName = "tessellation";
    cube.textureName = "cube";
    MaterialManager::getInstance().create(cube);

    MaterialInfo puu;
    puu.nimi = "barrel";
    puu.shaderName = "ufovalo";
    puu.textureName = "tynnyri";
    MaterialManager::getInstance().create(puu);

    MaterialInfo ruohikko;
    ruohikko.nimi = "terrain";
    ruohikko.shaderName = "tessellation";
    ruohikko.textureName = "terrainbase"; // Jos terrainbase on käytössä, tekstuuri vaihdetaan suoritusaikana
    MaterialManager::getInstance().create(ruohikko);

    MaterialInfo kivi;
    kivi.nimi = "rock";
    kivi.shaderName = "ufovalo";
    kivi.textureName = "kivi";
    MaterialManager::getInstance().create(kivi);

    MaterialInfo katto;
    katto.nimi = "house_roof";
    katto.shaderName = "ufovalo";
    katto.textureName = "house_roof";
    MaterialManager::getInstance().create(katto);

    MaterialInfo seina;
    seina.nimi = "house_wall";
    seina.shaderName = "ufovalo";
    seina.textureName = "house_wall";
    MaterialManager::getInstance().create(seina);

    MaterialInfo puuosat;
    puuosat.nimi = "house_wood_parts";
    puuosat.shaderName = "ufovalo";
    puuosat.textureName = "house_wood_parts";
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
