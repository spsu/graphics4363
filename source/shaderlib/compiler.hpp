#ifndef BT_GRAPHICS_SHADERLIB_COMPILER
#define BT_GRAPHICS_SHADERLIB_COMPILER

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

// ================ High Level API ===================== //

/**
 * Load the shader files, compile, and link them.
 * Convenience function -- handles all of the work. 
 * The program ID is saved to Registry,
 * 		Glint pid = Registry::getProgramId();
 */
GLint loadAndCompile(string vertexShaderFile, string fragmentShaderFile);

// ================ Lower Level API ==================== //

/**
 * Compile a shader, and return the handle.
 */
GLint makeFragmentShader(string source);
GLint makeVertexShader(string source);

/**
 * Link the shaders into a shader program.
 */
GLuint makeShaderProgram(GLint vertexShaderID, GLint fragmentShaderID);

#endif
