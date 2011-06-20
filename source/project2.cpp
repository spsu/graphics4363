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

GLfloat xTrans = 0.0f;
GLfloat yTrans = 0.0f;
GLfloat zTrans = 0.0f;

bool xRot = false;
bool yRot = false;
bool zRot = false;

GLfloat xRotCounter = 0.0f;
GLfloat yRotCounter = 0.0f;
GLfloat zRotCounter = 0.0f;

int NUM_VERTS = 0;

vector<GLfloat> getVertices(objLoader* loader)
{
	vector<GLfloat> vertices;
	obj_vector* vertex;
	obj_face* face;

	/*for(int i = 0; i < loader->vertexCount; i++)
	{
		vertex = loader->vertexList[i];
		vertices.push_back((float)vertex->e[0]);
		vertices.push_back((float)vertex->e[1]);
		vertices.push_back((float)vertex->e[2]);
	}*/

	printf("Wavefront .obj Report\n======================\n");
	printf("Faces:\t%i\n", loader->faceCount);
	printf("Vertices:\t%i\n", loader->vertexCount);

	for(int i = 0; i < loader->faceCount; i++)
	{
		face = loader->faceList[i];
		for(int j = 0; j < 3; j++) {
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

	for(int i = 0; i < loader->normalCount; i++)
	{
		normal = loader->normalList[i];
		normals.push_back((float)normal->e[0]);
		normals.push_back((float)normal->e[1]);
		normals.push_back((float)normal->e[2]);
	}

	return normals;
}

void objReport(objLoader* loader)
{

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

	// Make sure I'm winding the triangles correctly
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	//GLuint vao = createVertexArray();
	/*VertexArray* vao = new VertexArray();

	GLfloat* verts = (GLfloat*)LETTER_B;*/

	vector<GLfloat> vertices;
	vector<GLfloat> normals;

	objLoader *objData = new objLoader();
	objData->load("assets/torus.obj");

	vertices = getVertices(objData);
	normals = getNormals(objData);
	//vertices = vector<GLfloat>(TRIANGLE_A, TRIANGLE_A + sizeof(TRIANGLE_A) / sizeof(GLfloat));
	//vertices = vector<GLfloat>(LETTER_B, LETTER_B+ sizeof(LETTER_B) / sizeof(float));

	NUM_VERTS = vertices.size()/3;
	cout << "Number of points: " << vertices.size() << endl;
	printVertices(vertices);


	/*GLfloat verts2[900000];
	int numVerts = vertices.size();

	for(unsigned int i = 0; i < vertices.size(); i++) {
		verts2[i] = vertices.at(i);
	}
	cout << "Size " << vertices.size() << endl;
	NUM_VERTS = vertices.size() / 3;

	VertexArray* vao2 = new VertexArray();
	//vao2->loadVertices(verts2, vertices.size(), pId);
	vao2->loadVertices(verts2, NUM_VERTS, pId);*/

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

	// fov, aspect, near, far
	makePerspectiveProjectionMatrix(mP, 60.0f, 1.0f, 0.5f, 100.0f);
	//makePerspectiveProjectionMatrix(mP, 180.0f, 1.0f, 0.0f, 2100.0f);

	// L, R, B, T, N, F
	//glhFrustumf2(mP, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

	// FOV(Deg), Aspect, Znear, Zfar
	//glhPerspectivef2(mP, 120.0f, 1.0f, 1.0f, 100.0f);

	glClearColor(0.10f, 0.10f, 0.10f, 1.0f);
}


void resizeCb(int w, int h)
{
	glViewport(0, 0, w, h);
}


void render(void) 
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

	// XXX: Work here
	counter += 0.05f;
	counter2 -= 0.05f;

	//translate(mTrans, 0.0f, 0.0f, -0.5f);
	//translate(mTrans, 0.1f, 0.1f, 0.1f);
	//translate(mTrans, 0.0f, 0.0f, counter2);

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

	//glDrawArrays(GL_TRIANGLES, 0, NUM_VERTS*3);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, NUM_VERTS);
	//glDrawArrays(GL_QUADS, 0, NUM_VERTS);
	//glDrawArrays(GL_POLYGON, 0, NUM_VERTS);
	//glDrawArrays(GL_POINTS, 0, NUM_VERTS);

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
	glutCreateWindow ("Project 1");

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

