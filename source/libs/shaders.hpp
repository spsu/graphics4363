#ifndef BT_GRAPHICS_LIBS_SHADERS
#define BT_GRAPHICS_LIBS_SHADERS

/**
 * Shader loading, compilation, and linking.
 *
 * These library functions have been adapted from those given to us
 * throughout the course thus far. 
 */

#include <GL/glew.h>
#include <GL/glut.h>
#include <string>

using namespace std;

/**
 * Read the contents of a file.
 */
string readFile(string filename);

/**
 * Compile a shader, and return the handle.
 */
GLint makeFragmentShader(string source);
GLint makeVertexShader(string source);

/**
 * Link the shaders into a shader program.
 */
GLuint makeShaderProgram (GLint vertexShaderID, GLint fragmentShaderID);

#endif
