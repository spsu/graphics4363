#include "KixorObjectLoader.hpp"
#include <stdio.h>

using namespace std;

KixorObjectLoader::KixorObjectLoader(string objFile) :
	loader(0)
{
	loader = new objLoader();
	loader->load((char*)objFile.c_str());
}

void KixorObjectLoader::printReport()
{
	printf("Wavefront .obj Report\n======================\n");
	printf("Faces:\t%i\n", loader->faceCount);
	printf("Vertices:\t%i\n", loader->vertexCount);
	printf("Normals:\t%i\n", loader->normalCount);
}

vector<GLfloat> KixorObjectLoader::getVertices()
{
	vector<GLfloat> vertices;
	obj_vector* vertex;
	obj_face* face;

	for(int i = 0; i < loader->faceCount; i++)
	{
		face = loader->faceList[i];
		for(int j = 0; j < face->vertex_count; j++) {
			vertex = loader->vertexList[ face->vertex_index[j] ];
			vertices.push_back((float)vertex->e[0]);
			vertices.push_back((float)vertex->e[1]);
			vertices.push_back((float)vertex->e[2]);
		}
	}
	
	return vertices;
}

vector<GLfloat> KixorObjectLoader::getNormals()
{
	vector<GLfloat> normals;
	obj_vector* normal;
	obj_face* face;

	for(int i = 0; i < loader->faceCount; i++)
	{
		face = loader->faceList[i];
		for(int j = 0; j < face->vertex_count; j++) {
			normal = loader->normalList[ face->normal_index[j] ];
			normals.push_back((float)normal->e[0]);
			normals.push_back((float)normal->e[1]);
			normals.push_back((float)normal->e[2]);
		}
	}
	
	return normals;
}


