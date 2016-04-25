#include <stdexcept>
#include <SDL2/SDL.h>
#include <GL/glew.h>
#include "global.h"
#include "window.h"
#include "shader.h"
#include "program.h"
#include "vao.h"
#include "vertexbuffer.h"
#include "misc.h"
#include "modelparser.h"

void initializeSDL()
{
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_EVENTS) != 0)
    {
	std::string sdlerr = SDL_GetError();
        throw std::runtime_error("main::initializeSDL(): Failed to initialize SDL." + sdlerr);
    }
    logInfo.log("SDL was initialized succesfully.");
}

int main(int argc, char** argv)
{
    initializeSDL();
    Program p;
    p.initialize();
    p.run();
    SDL_Quit();
    return 0;
}
