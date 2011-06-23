#ifndef BT_GRAPHICS_OBJECT_LIB3DSLOADER
#define BT_GRAPHICS_OBJECT_LIB3DSLOADER

/**
 * Inspired by http://www.donkerdump.nl/node/207
 * and http://www.gamedev.net/topic/490141-lib3ds-texture-coordinates-and-vbos/
 */

#include <string>
#include <lib3ds/file.h>                        
#include <lib3ds/camera.h>
#include <lib3ds/mesh.h>
#include <lib3ds/node.h>
#include <lib3ds/material.h>
#include <lib3ds/matrix.h>
#include <lib3ds/vector.h>
#include <lib3ds/light.h>

using namespace std;

class VertexArray;

class Lib3dsLoader
{

	public:

		Lib3dsLoader(string filename);

		~Lib3dsLoader();

		int getNumFaces();

		VertexArray* buildVao();

	private:
		
		Lib3dsFile* model;
		int numFacesCached;
};

#endif
