#include <stdexcept>
#include "window.h"
#include <GL/glew.h>
#include "global.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Window::Window(const std::string& title, int width, int height, bool vsync, bool fullscreen)
{
    logInfo.log("%", "Creating window...");

    if (!SDL_WasInit(SDL_INIT_VIDEO)) throw std::runtime_error("Cannot initialize window because SDL_INIT_VIDEO wasn't initialized.");

    /* Ei määritellä ikkunan sijaintia. */
    int x = SDL_WINDOWPOS_UNDEFINED;
    int y = SDL_WINDOWPOS_UNDEFINED;

    /* Määritellään SDL:lle flageja liittyen ikkunaan. Flageja voi lisätä or operaattorin kanssa. */
    int flags = SDL_WINDOW_OPENGL|SDL_WINDOW_RESIZABLE;
    if (fullscreen) flags |= SDL_WINDOW_FULLSCREEN;

    /* Asetetaan GL-attribuutit. */
    initializeGLAttributes();

    /* Luodaan ikkuna. */
    window_ = SDL_CreateWindow(title.c_str(), x, y, width, height, flags);

    /* Tarkistetaan onnistuiko ikkunan luonti. */
    if (window_ == NULL) {
        std::string sdlerr = SDL_GetError();
        std::string err = "Error while creating SDL_Window: ";
        throw std::runtime_error(err + sdlerr);
    }

    /* Luodaan glconteksti ikkunaa varten ja alustetaan GLEW. */
    createGLContext();

    /* Asetetaan vsync. */
    setVsync(vsync);

    /*
    if (GLEW_ARB_debug_output)
    {
        glDebugMessageCallback(&gl_debug_message, 0);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    }
    */

    /* Tulostetaan tietoja OpenGL:stä ja laitteistosta. */
    std::string version  = reinterpret_cast<const char*>(glGetString(GL_VERSION));
    std::string vendor   = reinterpret_cast<const char*>(glGetString(GL_VENDOR));
    std::string renderer = reinterpret_cast<const char*>(glGetString(GL_RENDERER));
    logInfo.log("GL_Version: %", version);
    logInfo.log("Vendor: %", vendor);
    logInfo.log("Renderer: %", renderer);

    /* Luodaan ikkunalle perustapahtumat. */
    initialWindowEvents();

    logInfo.log("Window created succesfully.");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Window::~Window()
{
    if (window_ != 0) dispose();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Window& Window::operator=(Window&& other)
{
	std::swap(window_,other.window_);
        std::swap(glcontext_, other.glcontext_);
	return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Window::setVsync(bool vFlag)
{
    if (SDL_GL_SetSwapInterval(vFlag ? 1 : 0) < 0) logWarning.log("Failed on vsync: %", SDL_GetError());
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Window::setFullScreen(bool fullscreen)
{
    Uint32 flags = fullscreen ? SDL_WINDOW_FULLSCREEN : 0;
    if (SDL_SetWindowFullscreen(window_, flags) != 0)
    {
        logWarning.log("%", SDL_GetError());
    }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Window::resize(int w, int h)
{
    SDL_SetWindowSize(window_, w, h);
    glViewport(0, 0, w, h);

    SDL_Event e;
    e.type = SDL_WINDOWEVENT;
    e.window.event = SDL_WINDOWEVENT_RESIZED;
    e.window.windowID = SDL_GetWindowID(window_);
    e.window.data1 = w;
    e.window.data2 = h;
    SDL_PushEvent(&e);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Window::dispose()
{
    if (SDL_WasInit(SDL_INIT_VIDEO) & SDL_INIT_VIDEO)
    {
        logWarning.log("SDL_INIT_VIDEO was uninitialized when calling Window::dispose().");
    }
    SDL_GL_DeleteContext(glcontext_);
    SDL_DestroyWindow(window_);
    logInfo.log("Disposing window...");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Window::createGLContext()
{
    /* Luodaan opengl conteksti ikkunalle. */
    glcontext_ = SDL_GL_CreateContext(window_);
    if (glcontext_ == NULL)
    {
        std::string sdlerr = SDL_GetError();
        throw std::runtime_error("Window::createGLContext: Error while creating openglcontext. " + sdlerr);
    }

    glewExperimental = GL_TRUE;

    std::cout << "initializing glew." << std::endl;
    /* Alustetaan GLEW. Tämä täytyy tehdä glcontekstin luonnin jälkeen. */
    GLenum error = glewInit();
    if (error != GLEW_NO_ERROR) {
        std::string glewError = reinterpret_cast<const char*>(glewGetErrorString(error));
        throw std::runtime_error("Window::createGLContext: Error while initializing GLEW. " + glewError);
     }

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Window::swapBuffers()
{
    SDL_GL_SwapWindow(window_);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Window::initializeGLAttributes()
{
    //SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    //SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    //SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    //SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    //SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    //SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    //SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Window::initialWindowEvents()
{
    int width;
    int height;
    SDL_GetWindowSize(window_, &width, &height);

    SDL_Event e;
    e.type = SDL_WINDOWEVENT;
    e.window.event = SDL_WINDOWEVENT_RESIZED;
    e.window.windowID = SDL_GetWindowID(window_);
    e.window.data1 = width;
    e.window.data2 = height;
    SDL_PushEvent(&e);
}
