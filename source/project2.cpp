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

// Program ID
GLint pId = 0;

// Matrices
GLfloat* mRot;
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

int NUM_VERTS = 0;

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
	VertexArray* vao = new VertexArray();

	NUM_VERTS = 12 * 3;
	GLfloat* verts = (GLfloat*)LETTER_B;

	// UGH, this is terrible. I should have done this programatically.
  	GLfloat colors[NUM_VERTS*3];

	// FIXME: No math in loop cond, lazy!
	for(unsigned int i = 0; i < NUM_VERTS/3; i++) {
		colors[9*i] = 0.0f;
		colors[9*i+1] = 0.0f;
		colors[9*i+2] = 0.7f;

		colors[9*i+3] = 0.0f;
		colors[9*i+4] = 0.0f;
		colors[9*i+5] = 0.0f;

		colors[9*i+6] = 0.1f;
		colors[9*i+7] = 0.0f;
		colors[9*i+8] = 0.2f;
	}

	//vao->loadVertices(verts, 12*3, pId);
	//vao->loadColors(colors, 12*3, pId);


	// XXX: Sphere
	//vector<GLfloat> sphereV = makeSphere(0.1f, 9);
	vector<GLfloat> sphereV = makeSphere(0.32f, 9);
	GLfloat sphereVerts[80000];
	int numVerts = sphereV.size();// / 4; // X, y, z, w coords

	for(unsigned int i = 0; i < sphereV.size(); i++) {
		sphereVerts[i] = sphereV.at(i);
	}
	cout << "Size " << sphereV.size() << endl;

	VertexArray* vao2 = new VertexArray();
	vao2->loadVertices(sphereVerts, numVerts*2, pId);


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
	//makePerspectiveProjectionMatrix(mP, 60.0f, 1.0f, 0.5f, 100.0f);
	//makePerspectiveProjectionMatrix(mP, 180.0f, 1.0f, 0.0f, 2100.0f);

	// L, R, B, T, N, F
	glhFrustumf2(mP, -1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f);

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
	counter2 -= 0.005f;

	//translate(mTrans, 0.0f, 0.0f, -0.5f);
	translate(mTrans, 0.1f, 0.1f, 0.1f);
	translate(mTrans, 0.0f, 0.0f, 0.0f);
	rotateY(mRot, counter);
	//rotateY(mRot, 0.0f);

	matrixMult4x4(mMV, mRot, mTrans); // mMV now holds combination...

	GLuint r = glGetUniformLocation(pId, "mv");
	glUniformMatrix4fv(r, 1, GL_TRUE, mMV);

	GLuint p = glGetUniformLocation(pId, "p");
	glUniformMatrix4fv(p, 1, GL_TRUE, mP);

	//glDrawArrays(GL_TRIANGLES, 0, NUM_VERTS);
	glDrawArrays(GL_POINTS, 0, NUM_VERTS);

	// Double buffering -- swap current buffer.
	glutSwapBuffers();
	glutPostRedisplay();
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

	setup();
	glutMainLoop();

	return 0;
};

