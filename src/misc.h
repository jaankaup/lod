#include <stdint.h>
#include <string>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <type_traits>
#include <vector>
#include <streambuf>
#include <stdexcept>
#include <algorithm>
#include <iterator>
#include <regex>
#include <GL/glew.h>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "global.h"

namespace Misc
{
   extern std::string loadSource(const std::string& filename);
   /// Returns true if the given strings ends with the specified chars, false otherwise.
   extern bool startswith(const std::string& source, const std::string& prefix);
   /// Returns true if the given strings starts with the specified chars, false otherwise.
   extern bool endswith(const std::string& source, const std::string& suffix);
   /// Returns a shader object type on filename. Returns 0 on failure.
   extern GLuint getShaderType(const std::string& filelocation);
   /// Compile shader object. Return false on failure, true otherwise.
   extern void compileShader(const GLuint, const std::string& source);
   /// Returns true if the given shader object or program is compiled succesfully, false otherwise.
   extern bool checkShader(const GLuint);
   /// Parse a line to strings. @param line is the line to being parsed.
   /// @param container is a reference to the container where the result is parsed.
   /// @param delimRegex is the delimiter regex string. Default delimRegex is
   /// skips white spaces and capture all which isn't a white space.
   extern void tokenize(const std::string &line, std::vector<std::string> &container, const std::string delimRegex);

   extern glm::mat3 calculateNormalMatrix(const glm::mat4& modelView);
}; // namespace Misc
