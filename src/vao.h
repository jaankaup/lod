#ifndef VAO_H
#define VAO_H

#include <GL/glew.h>
#include "global.h"

/** class for vertex array objects. */
class VAO
{
    friend class Model;

	public:
		VAO();
		~VAO();
		/*
		VAO(const VAO&) = delete;
        VAO(VAO&&) = default;
        VAO& operator=(const VAO&) = delete;
        VAO& operator=(VAO&&) = default;
        */
        VAO& getInstance();
        //VAO(const VAO&) = default;
		void bind() const;
		void unBind() const; //AUTTAAKO???
		GLuint getID() const;

	private:
		GLuint vaoID_;
};
#endif // VAO_H
