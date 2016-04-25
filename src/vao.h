#ifndef VAO_H
#define VAO_H

#include <GL/glew.h>

/** class for vertex array objects. */
class VAO
{
	public:
		VAO();
		~VAO();
		void bind() const;

	private:
		GLuint vaoID_;
};
#endif // VAO_H
