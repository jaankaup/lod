#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <stdexcept>
//#include <SDL2/SDL.h>
#include "log.h"

/**
 * Tähän tiedostoon globaalit jutut.
 */

/// Loggerit.
static Log logWarning("WARNING: ",std::cout);
static Log logDebug("DEBUG: ",std::cout);
static Log logError("ERROR: ",std::cout);
static Log logInfo("INFO: ",std::cout);

//void initializeSDL()
//{
//    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER != 0)
//    {
//        logError.log("%", SDL_GetError());
//        throw std::runtime_error("SDL:ää ei voitu alustaa: " + SDL_GetError())
//    }
//}
#endif // GLOBAL_H
