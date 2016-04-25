#ifndef SHADER_H
#define SHADER_H
#include <iostream>
#include <vector>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include "misc.h"

/** A simple shader class with basic shader operations. */
class Shader
{
	public:
		Shader();
		~Shader();

		/// Create a shader from source files. Throws runtime_exception if it fails.
		void createFromSource(const std::vector<std::string>& sources);
		/// Binds this shader.
		void use();

		void setUniform(const std::string& name, int value) const;
        void setUniform(const std::string& name, float value) const;
        void setUniform(const std::string& name, const glm::vec3& value) const;
        void setUniform(const std::string& name, const glm::vec4& value) const;
        void setUniform(const std::string& name, const glm::mat4& value) const;
        //void SetUniform(const std::string& name, int count, const glm::vec4* values);
        //void SetUniform(const std::string& name, int count, const glm::mat4* values);


	private:
		/// handle to the shader.
		GLuint shaderID_ = 0;

        //std::vector<GLuint> createShaderObjects(const std::vector<std::string>& shaderSources) const;
};


#endif // SHADER_H

