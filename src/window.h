#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>
#include <memory>
#include <SDL2/SDL.h>
#include <GL/glew.h>

// Ennakkoesittelyt, jotta saadaan koodi käännettyä.
struct SDL_Window;
using SDL_GLContext = void*;

/**
 * Ikkuna luokka.
 */
class Window
{
	public:
		Window() : window_(0), glcontext_(0) {};
		Window(const std::string& name, int width, int height, bool vsync, bool fullscreen);
		~Window();
		Window& operator=(const Window& other) = delete;
		Window& operator=(Window&& other);
		void initialize();
		/// VSync asetus. Palauttaa true jos onnsituu, muutoin palauttaa falsen.
		void setVsync(bool vFlag);
		/// Fullscreen mode.
        void setFullScreen(bool fullscreen);
		/// Päivitetään bufferit.
		void swapBuffers();
		/// Ikkunan koon muuttaminen.
		void resize(int width, int height);

	private:
		/// Kahva SDL_ikkunaan
		SDL_Window* window_;
		/// Kahva SDL_ikkunaan liittyvään GL-kontekstiin.
		SDL_GLContext glcontext_;

		/// OpenGL contextin luonti ikkunalle.
		void createGLContext();

		/// Vapauttaa privaattijäsenmuuttujat jne.
		void dispose();

		/// Asetetaan ikkunan GL-attribuutit.
		void initializeGLAttributes();

		/// Asetetaan ikkunalle ensimmäiset tapahtumat.
		void initialWindowEvents();
};

#endif //WINDOW_H
