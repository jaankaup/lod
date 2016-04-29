#include <vector>
#include <array>
#include <math.h>
#include "program.h"
#include "global.h"
#include "modelparser.h"

Program::~Program() {
   if (vao_ != 0) delete vao_; // only for testing.
   if (vbo_ != 0) delete vbo_; // only for testing.
   if (shader_ != 0) delete shader_; // only for testing.
   if (texture_ != 0) delete texture_; // only for testing.
   if (ib_ != 0) delete ib_; // only for testing.
}

void Program::initialize()
{
    ModelParser mp;
    mp.parse("data/models/terrain.obj");

    window_ = Window("Test!", 800,600,true,false);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // N�m� liittyy tekstuureihin.
    glFrontFace(GL_CW);
    glCullFace(GL_FRONT);
    glEnable(GL_CULL_FACE);

    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    /* Luodaan shader. */
    shader_ = new Shader(); // Just testing.

    /* T�h�n shaderien sijainnit, joista sitten muodostetaan varsinainen varjostusohjelma. */
    std::vector<std::string> shaderSources = {"data/shaders/testShader.vert","data/shaders/testShader.frag"};
    /* Rakennetaan shader. */
    shader_ -> createFromSource(shaderSources);
    /* K�ytet��n shaderia. */
    shader_ -> use();
    /* Luodaan ensimm�inen project view projectio. */
    glm::mat4 modelviewprojection = projection_ * camera_.getMatrix() * model_;
    /* Sy�tet��n se shaderille. */
    shader_ -> setUniform(std::string("MVP"), modelviewprojection);

    /* We bind vertex array object first. It remembers all settings we make on vertex buffers. */
    /* Luodaan siis vertex array object. */
    vao_ = new VAO(); // JUST TESTING!
    /*Bindataan se. */
    vao_-> bind();

    /* Luodaan vertexbufferi. */
    vbo_ = new VertexBuffer(GL_ARRAY_BUFFER,GL_STATIC_DRAW); // Just testing!!!
    /* Luodaan index bufferi. */
    ib_ = new IndexBuffer();

    /* Lets put data to the vertex object. */
    /* Isket��n dataa vertexbufferiin. */
    std::vector<float> dat = mp.getData();
    vbo_ -> setData(&dat[0], sizeof(float)*dat.size(), AttributeConfiguration::VTN_Interleaved);
    ib_ -> setData(mp.getIndexArray(), mp.getIndexSize(), mp.getIndiceCount());

    textureP1_ = TextureControl::getInstance().createTexture("data/textures/dirt.jpg","dirt");
    textureP2_ = TextureControl::getInstance().createTexture("data/textures/cliff.jpg","cliff");
    textureP3_ = TextureControl::getInstance().createTexture("data/textures/grass.jpg","outo");

    /* Bindataan ensimm�inen, t�ll� hetkell� ainoa, tekstuuri. */
    textureP1_->bind();
    /* Isket��n shaderille tieto tekstuurista. */
    shader_ -> setUniform(std::string("myTextureSampler"), 0);
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
                    shader_ -> setUniform(std::string("myTextureSampler"), textureP1_->getUnit());
                    break;

                case SDLK_2:
                    textureP1_->bind();
                    shader_ -> setUniform(std::string("myTextureSampler"), textureP2_->getUnit());
                    break;

                case SDLK_3:
                    textureP3_->bind();
                    shader_ -> setUniform(std::string("myTextureSampler"), textureP3_->getUnit());
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
    float blaahX = 80.0f + 60.0f * sin(f); // En tied� mit� tekee.
    float blyyhY = 80.0f + 60.0f * cos(f); // En tied� mit� tekee.
    float bliihZ = 80.0f + 60.0f * sin(f) * cos(f); // Mit� t�m� tekee? HT.

    camera_.translate(glm::vec3(blaahX,blyyhY,bliihZ)); // Testataan ohjelmaa. Ei muuta virkaa.
    /* Muodostetaan uusi p�ivitetty model view projektio. */
    glm::mat4 modelviewprojection = projection_ * camera_.getMatrix() * model_;
    /* L�hetet��n uusi p�ivitetty model view projektio shaderille. */
    shader_ -> setUniform(std::string("MVP"), modelviewprojection);
}

void Program::render()
{
    glClearColor(0.0f,0.0f,0.0f,1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    shader_ -> use();
    /* Bindaillaan juttuja ennen piirtoa. */
    //textureP1_->bind();
    //shader_ -> setUniform(std::string("myTextureSampler"), 0);
    vao_ -> bind();
    vbo_ -> bind();
    ib_ -> bind();
    /* Piirret��n indeksipuskuria hy�dynt�en. */
    glDrawElements(GL_TRIANGLES, ib_->getElementCount(), GL_UNSIGNED_INT, (void*)0);
    /* Tuodaan piirretty juttu n�kyville. */
    window_.swapBuffers();
}
