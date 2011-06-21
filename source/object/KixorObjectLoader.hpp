#ifndef BT_GRAPHICS_OBJECT_KIXOROBJECTLOADER
#define BT_GRAPHICS_OBJECT_KIXOROBJECTLOADER

/**
 * This is an interface to the object loader found at:
 * 		<http://www.kixor.net/dev/objloader/>
 *
 * These routines extract/convert the vertices and normals from the 
 * object loader and make them available for our use. 
 */

#include "../loader/objLoader.h"
#include <vector>
#include <string>
#include <GL/glew.h>
#include <GL/glut.h>

using namespace std;

class KixorObjectLoader
{
	public:
		/**
		 * CTOR.
		 * Immediately loads the file. 
		 * TODO: Error handling
		 * TODO: DTOR.
		 */
		KixorObjectLoader(string objFile);

		/**
		 * Print a report on the object file.
		 */
		void printReport();

		/**
		 * Get the model vertices and normals. 
		 */
		vector<GLfloat> getVertices();
		vector<GLfloat> getNormals();

	private:
		/**
		 * Pointer to instance of Kixor object loader.
		 */
		objLoader* loader;
};

#endif
