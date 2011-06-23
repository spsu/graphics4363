#include "Lib3dsLoader.hpp"
#include "vao.hpp"
#include <string.h>

Lib3dsLoader::Lib3dsLoader(string filename) :
	model(0),
	numFacesCached(0)
{
	// TODO: Error handling. 
	model = lib3ds_file_load(filename.c_str());
}

Lib3dsLoader::~Lib3dsLoader()
{
	lib3ds_file_free(model);
}

int Lib3dsLoader::getNumFaces()
{
	Lib3dsMesh* mesh = 0;
	int cnt = 0;

	// TODO: Error handling

	if(numFacesCached) {
		return numFacesCached;
	}

	mesh = model->meshes;

	while(mesh != 0) {
		cnt += mesh->faces;
		mesh = mesh->next;
	}

	numFacesCached = cnt;
	return cnt;
}

VertexArray* Lib3dsLoader::buildVao()
{
	VertexArray* vao = 0;
	Lib3dsVector* verts = 0;
	Lib3dsVector* norms = 0;
	Lib3dsMesh* mesh = 0;
	unsigned int numFaces = 0;
	unsigned int doneFaces = 0;

	numFaces = getNumFaces();

	verts = new Lib3dsVector[numFaces * 3];
	norms = new Lib3dsVector[numFaces * 3];

	mesh = model->meshes;

	// For all meshes, calculate normals and extract vertices. 
	// Taken from http://www.donkerdump.nl/node/207
	while(mesh != 0)
	{
		lib3ds_mesh_calculate_normals(mesh, &norms[doneFaces*3]);

		// For all faces
		for(unsigned int i = 0; i < mesh->faces; i++)
		{
			Lib3dsFace* face = &mesh->faceL[i];
			for(unsigned int j = 0; j < 3; j++)
			{
				memcpy(&verts[doneFaces * 3 + j], 
						mesh->pointL[face->points[j]].pos,
						sizeof(Lib3dsVector));
			}
			doneFaces++;
		}
		mesh = mesh->next;
	}

	vao = new VertexArray();
	vao->loadVertices(verts, numFaces);
	vao->loadNormals(norms, numFaces);

	delete[] verts;
	delete[] norms;

	return vao;
}

