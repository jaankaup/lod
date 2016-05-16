#ifndef PROGRAM_H
#define PROGRAM_H
#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include "window.h"
#include "vertexbuffer.h"
#include "vao.h"
#include "shader.h"
#include "texture.h"
#include "Camera.h"
#include "indexbuffer.h"
#include "texturecontrol.h"
#include "misc.h"
#include "TextureManager.h"
#include "model.h"

/// forward declaration.
class Window;
class VertexBuffer;

/** The main program class. */
class Program
{
	public:
		/// Constructor.
		Program() {}
	    /// Destructor.
		~Program();
        /// Setup up program.
		void initialize();
		/// Start the main loop.
		void run();
		/// Event handling.
		void handleEvents();
		/// Update things and prepare everything for rendering.
		void update(float f);
		/// Render the scene.
		void render();

	private:
		/// a handle to the current rendering window.
		Window window_;
		/// This is the prediate value for main loop. True for running the loop and false for breaking the main loop.
		bool running_ = true;
		/// a test buffer.
		VertexBuffer* vbo_ = 0;
		VertexBuffer* vbo2_ = 0;
		VAO* vao_ = 0;
		VAO* vao2_ = 0;
		Shader* shader_ = 0;
		Texture* texture_ = 0;
		IndexBuffer* ib_ = 0;
		IndexBuffer* ib2_ = 0;
		Model* modelUUS_ = 0;
		TexturePtr textureP1_;
		TexturePtr textureP2_;
		TexturePtr textureP3_;
		TexturePtr textureP4_;
		//VAO vaoUus_;
		//VAO vaoUus2_;
		//Camera camera_ = Camera(glm::vec3(98.2f,120.2f,99.2f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
		Camera camera_ = Camera(glm::vec3(5.2f,5.2f,5.2f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
		glm::mat4 projection_ = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 300.f);
		glm::mat4 model_ = glm::mat4(1.0f);
		//glm::mat4 model2_ = glm::mat4(1.0f);
		glm::mat4 model2_ = glm::scale(glm::mat4(1.0f),glm::vec3(20.0f));//glm::mat4(1.0f);
		unsigned int time_ = 0;
};

#endif // PROGRAM_H
