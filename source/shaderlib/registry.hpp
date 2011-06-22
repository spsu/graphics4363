#ifndef BT_GRAPHICS_SHADERLIB_REGISTRY
#define BT_GRAPHICS_SHADERLIB_REGISTRY

#include <GL/glew.h>
#include <GL/glut.h>

/**
 * GL Shader Program Regisry.
 * Instead of passing around the ID/handle of the shader program, it is
 * statically held here for retrieval. 
 */
class Registry
{
	public: 
		// Set the program ID.
		static void setProgramId(GLint pid) 
				{ programId = pid; };

		// Get the program ID. 
		static GLint getProgramId() 
				{ return programId; };

	private:
		// XXX: This is a singleton!
		Registry() {};
		Registry(const Registry&) {};
		Registry& operator=(const Registry&) {};

		// The program ID
		static GLint programId;
};

#endif
