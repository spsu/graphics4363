/**
 * Project One
 * Graphics 4363
 * Brandon Thomas Suit
 * http://possibilistic.org
 * http://github.com/echelon
 */

#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>

using namespace std;

#include "libs/math.hpp"
#include "libs/shaders.hpp"
#include "libs/vertex.hpp"
#include "libs/vao.hpp"

#include "geometry/basic.hpp"
#include "geometry/sphere.hpp"

#include "loader/objLoader.h"

// Program ID
GLint pId = 0;

// Matrices
GLfloat* mRot;
GLfloat* mRotMid;
GLfloat* mRotX;
GLfloat* mRotY;
GLfloat* mRotZ;
GLfloat* mScale;
GLfloat* mTrans;
GLfloat* mP;
GLfloat* mMV;
GLfloat* mCam;

GLfloat counter = 0.0f;
GLfloat counter2 = 0.0f;
GLfloat counter3 = 0.0f;
bool directionRight = true;

GLfloat xTrans = 0.0f;
GLfloat yTrans = 0.0f;
GLfloat zTrans = 0.0f;

bool xRot = false;
bool yRot = false;
bool zRot = false;

GLfloat xRotCounter = 0.0f;
GLfloat yRotCounter = 0.0f;
GLfloat zRotCounter = 0.0f;

GLuint lightLoc(0);
GLuint dcLoc(0);

int NUM_VERTS = 0;

vector<GLfloat> getVertices(objLoader* loader)
{
	vector<GLfloat> vertices;
	obj_vector* vertex;
	obj_face* face;

	printf("Wavefront .obj Report\n======================\n");
	printf("Faces:\t%i\n", loader->faceCount);
	printf("Vertices:\t%i\n", loader->vertexCount);

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

vector<GLfloat> getNormals(objLoader* loader)
{
	vector<GLfloat> normals;
	obj_vector* normal;
	obj_face* face;

	printf("Wavefront .obj Report\n======================\n");
	printf("Faces:\t%i\n", loader->faceCount);
	printf("Normals:\t%i\n", loader->normalCount);

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

void setup()
{
	string fragTxt;
	string vertTxt;
	GLint fCmp = 0;
	GLint vCmp = 0;
	
	fragTxt = readFile("./source/shaders/fshader.fp");
	vertTxt = readFile("./source/shaders/vshader.vp");

	fCmp = makeFragmentShader(fragTxt);
	vCmp = makeVertexShader(vertTxt);

	printf("Frag Shader Compiled ID: %d\n", fCmp);
	printf("Vert Shader Compiled ID: %d\n", vCmp);

	pId = makeShaderProgram(vCmp, fCmp);

	printf("Linked program ID: %d\n", pId);

	// Use shader
	glUseProgram(pId);

	// Backface culling. 
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	// Depth testing.
	glEnable(GL_DEPTH_TEST);

	vector<GLfloat> vertices;
	vector<GLfloat> normals;

	objLoader *objData = new objLoader();
	objData->load("assets/sphere.obj");

	vertices = getVertices(objData);
	normals = getNormals(objData);

	NUM_VERTS = vertices.size()/3;

	VertexArray* vao2 = new VertexArray();
	vao2->loadVertices(vertices, pId);
	vao2->loadNormals(normals, pId);
	
	// XXX: Init matrices
	mRot = new GLfloat[16];
	mRotX = new GLfloat[16];
	mRotY = new GLfloat[16];
	mRotZ = new GLfloat[16];
	mScale = new GLfloat[16];
	mTrans = new GLfloat[16];
	mP = new GLfloat[16];
	mMV = new GLfloat[16];
	mCam = new GLfloat[16];

	// XXX: Lighting
	lightLoc = glGetUniformLocation(pId, "lightPos");
	dcLoc = glGetUniformLocation(pId, "diffuseColor");

	// mat, fov, aspect, near, far
	makePerspectiveProjectionMatrix(mP, 60.0f, 1.0f, 0.5f, 100.0f);

	glClearColor(0.10f, 0.10f, 0.10f, 1.0f);
}


void resizeCb(int w, int h)
{
	glViewport(0, 0, w, h);
}

void render(void) 
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	// XXX: Moving lighting. 
	if(counter3 > 300.0f) {
		directionRight = false;	
	}
	else if(counter3 < -300.0f) {
		directionRight = true;
	}
	if(directionRight) {
		counter3 += 4.0f;
	}
	else {
		counter3 -= 4.0f;
	}

	// XXX: Lighting
	GLfloat lightPos[] = { counter3, counter3, 100.0f };
	GLfloat diffuseColor[] = { 1.0f, 0.0f, 0.0f, 1.0f };

	glUniform3fv(lightLoc, 1, lightPos);
	glUniform4fv(dcLoc, 1, diffuseColor);

	// XXX: Rotation and transformation
	counter += 0.05f;
	counter2 -= 0.05f;

	if(xRot) {
		xRotCounter += 0.05f;
	}
	if(yRot) {
		yRotCounter += 0.05f;
	}
	if(zRot) {
		zRotCounter += 0.05f;
	}

	rotateX(mRotX, xRotCounter);
	rotateY(mRotY, yRotCounter);
	rotateZ(mRotZ, zRotCounter);

	translate(mTrans, xTrans, yTrans, zTrans);

	//rotateY(mRot, 0.0f);

	matrixMult4x4(mRot, mRotX, mRotY);
	//matrixMult4x4(mRot, mRotMid, mRotZ);

	matrixMult4x4(mMV, mRot, mTrans); // mMV now holds combination...

	GLuint r = glGetUniformLocation(pId, "mv");
	glUniformMatrix4fv(r, 1, GL_TRUE, mMV);

	GLuint p = glGetUniformLocation(pId, "p");
	glUniformMatrix4fv(p, 1, GL_TRUE, mP);

	//glDrawArrays(GL_TRIANGLES, 0, NUM_VERTS);
	glDrawArrays(GL_QUADS, 0, NUM_VERTS);

	// Double buffering -- swap current buffer.
	glutSwapBuffers();
	glutPostRedisplay();
}

void keypress(unsigned char key, int x, int y)
{
	switch (key) {
		case 'q':
			exit(1);
			break;

		// Translate
		case 'w': 
			yTrans += 0.01f;
			break;
		case 's':
			yTrans -= 0.01f;
			break;
		case 'r': 
			yTrans += 5.0f;
			break;
		case 't':
			yTrans -= 5.0f;
			break;

		case 'a':
			xTrans -= 0.01f;
			break;
		case 'd':
			xTrans += 0.01f;
			break;
		case 'f':
			xTrans -= 5.0f;
			break;
		case 'g':
			xTrans += 5.0f;
			break;

		case 'z':
			zTrans -= 0.05f;
			break;
		case 'x':
			zTrans += 0.05f;
			break;
		case 'c':
			zTrans -= 5.0f;
			break;
		case 'v':
			zTrans += 5.0f;
			break;


		// Rotate
		case 'y':
			xRot = !xRot;
			break;
		case 'u':
			yRot = !yRot; 
			break;
		case 'i':
			zRot = !zRot;
			break;
	}
	printf("Translation: %f, %f, %f\n", xTrans, yTrans, zTrans);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_STENCIL);
	glutInitWindowSize(800, 600);
	glutCreateWindow ("Project 2");

	// Intitalize driver
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		fprintf(stderr, "GLEW error");
		return 1;
	}

	// Callbacks etc.
	glutReshapeFunc(resizeCb);
	glutDisplayFunc(render);
	glutKeyboardFunc(keypress);

	setup();
	glutMainLoop();

	return 0;
};

