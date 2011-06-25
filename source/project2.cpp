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
#include <map>
#include <utility>
#include <iostream>

using namespace std;

#include "libs/file.hpp"
#include "libs/math.hpp"
//#include "libs/vertex.hpp"
#include "object/vao.hpp"
#include "object/KixorObjectLoader.hpp"
#include "object/Lib3dsLoader.hpp"
#include "object/TransformationStack.hpp"
#include "object/TransformationStackRegistry.hpp"
#include "shaderlib/compiler.hpp"
#include "shaderlib/registry.hpp"
#include "geometry/basic.hpp"

const string FRAGMENT_SHADER = "./source/shaders/fshader.fp";
const string VERTEX_SHADER = "./source/shaders/vshader.vp";

VertexArray* vao1 = 0;
VertexArray* vao2 = 0;
VertexArray* vao3 = 0;
VertexArray* vao4 = 0;
VertexArray* vao5 = 0;
VertexArray* vao6 = 0;
VertexArray* vao7 = 0;

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
GLfloat xRotMain = 0.0f;
GLfloat yRotMain = 0.0f;
GLfloat zRotMain = 0.0f;
GLfloat xRotCount = 0.0f;
GLfloat yRotCount = 0.0f;
GLfloat zRotCount = 0.0f;

bool xRot = false;
bool yRot = false;
bool zRot = false;

GLfloat xRotCounter = 0.0f;
GLfloat yRotCounter = 0.0f;
GLfloat zRotCounter = 0.0f;

// Camera movement
GLfloat camRotZ = 0.0f;

GLuint lightLoc(0);
GLuint dcLoc(0);

KixorObjectLoader* sphereLoader = 0;    
KixorObjectLoader* torusLoader= 0;    
KixorObjectLoader* cubeLoader = 0;    
KixorObjectLoader* hmsLoader = 0;    

/**
 * 3D Studio Models.
 * A pair of strings: Model file, texture file.
 */
map<string, pair<string, string> > models;


void setup()
{
	models["masksalesman"] = make_pair(
			"assets/nintendo/masksalesman.3ds",
			"assets/nintendo/HappyMas.bmp"
		);
	models["luigi"] = make_pair(
			"assets/nintendo/luigi.3ds",
			"assets/nintendo/luigi_gr.bmp"
		);
	models["whomp"] = make_pair(
			"assets/nintendo/whomp.3ds",
			"assets/nintendo/Whomp_gr.bmp"
		);
							

	pId = loadAndCompile(FRAGMENT_SHADER, VERTEX_SHADER);

	// Use shader
	glUseProgram(pId);

	// Backface culling, depth test
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);

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
	math::makePerspectiveProjectionMatrix(mP, 60.0f, 1.0f, 0.5f, 4000.0f);

	//glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// XXX: Trying  http://code.google.com/p/lib3ds/
	//hmsLoader = new KixorObjectLoader("assets/ocarina/untitled.obj");

	Lib3dsLoader* loader = 0;
	
	loader = new Lib3dsLoader(models["luigi"].first);
	vao1 = loader->buildVao();
	vao1->loadTexture(models["luigi"].second);
	delete loader;

	loader = new Lib3dsLoader("assets/nintendo/whomp.3ds");
	vao2 = loader->buildVao();
	vao2->loadTexture("assets/nintendo/Whomp_gr.bmp");
	delete loader;

	loader = new Lib3dsLoader("assets/nintendo/levels/kokiri.3ds");
	vao3 = loader->buildVao();
	vao3->loadTexture("assets/nintendo/levels/kokiri.png");
	delete loader;

	loader = new Lib3dsLoader("assets/nintendo/levels/hyrule.3ds");
	vao4 = loader->buildVao();
	vao4->loadTexture("assets/nintendo/levels/hyrule.png");
	delete loader;




	// Create VAO. 
	/*vao1 = new VertexArray();
	vao1->loadVertices(hmsLoader->getVertices());
	vao1->loadNormals(hmsLoader->getNormals());*/

	// Initial offset
	TransformationStack* transformStack = TransformationStackRegistry::get();
	transformStack->translate(0.0f, 0.0f, 0.0f);
	//transformStack->rotate(xRotCount, yRotCount, zRotCount);
	transformStack->rotate(1.5f, yRotCount, zRotCount); // XXX: Hyrule


	printf("%f, %f, %f\n", xRotCount, yRotCount, zRotCount);

	transformStack->applyTransform();
}


void resizeCb(int w, int h)
{
	glViewport(0, 0, w, h);
}

void render(void) 
{
	TransformationStack* transformStack = TransformationStackRegistry::get();

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
	GLfloat diffuseColor[] = { 0.1f, 0.1f, 0.1f, 1.0f };

	glUniform3fv(lightLoc, 1, lightPos);
	glUniform4fv(dcLoc, 1, diffuseColor);

	// XXX: Rotation and transformation
	counter += 0.05f;
	counter2 -= 0.05f;

	if(xRot) {
		xRotCounter += 0.05f;
	}
	if(yRot) {
		yRotCounter += 0.02f;
	}
	if(zRot) {
		zRotCounter += 0.05f;
	}

	transformStack->push();
	transformStack->rotate(0.0f, 0.0f, camRotZ);
	transformStack->applyTransform();

	transformStack->push();
	transformStack->translate(xTrans, yTrans, zTrans);
	//transformStack->rotate(xRotCounter, yRotCounter, zRotCounter);

	//printf("Translate: %f, %f, %f\n", xTrans, yTrans, zTrans);
	//printf("Rotate: %f, %f, %f\n", xRotCount, yRotCount, zRotCount);

	transformStack->applyTransform();

	vao1->translate(2.0f, 0.0f, 0.0f);
	//vao1->rotate(20.0f + xRotCounter, 40.0f + yRotCounter, zRotCounter);

	vao2->translate(-3.0f, 0.0f, 0.0f);
	//vao2->rotate(20.0f + xRotCounter, 40.0f + yRotCounter, zRotCounter);
	//vao1->scale(0.7f, 0.3f, 0.5f);

	vao3->scale(1000.0, 1000.0, 1000.0);
	//vao3->rotate(1.50f, 0.0f, 0.0f);

	
	vao4->scale(1000.0, 1000.0, 1000.0);

	// Modelview Matrix
	GLuint r = glGetUniformLocation(pId, "mv");
	glUniformMatrix4fv(r, 1, GL_TRUE, transformStack->top());

	// Perspective matrix
	GLuint p = glGetUniformLocation(pId, "p");
	glUniformMatrix4fv(p, 1, GL_TRUE, mP);

	//vao1->draw();
	//vao2->draw();
	vao3->draw();
	//vao4->draw();

	transformStack->pop();
	transformStack->pop();

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
			zTrans += 20.0f;
			break;
		case 'a':
			xTrans += 20.0f;
			break;
		case 's':
			zTrans -= 20.0f;
			break;
		case 'd':
			xTrans -= 20.0f;
			break;
		case 'e': 
			yTrans += 20.0f;
			break;
		case 'r':
			yTrans -= 20.0f;
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
			xRotCount += 0.1f;
			break;
		case 'u':
			yRot = !yRot; 
			//yRotCount += 0.1f;
			break;
		case 'i':
			zRot = !zRot;
			zRotCount += 0.1f;
			break;
	}
}

void mouseButton(int button, int state, int x, int y)
{
	switch(button)
	{
		case GLUT_LEFT_BUTTON:
		case GLUT_RIGHT_BUTTON:
			// Fallthrough. Nothing for now.
			break;
	}
}


// This code is adapted from GLUT lab. 
void mouseMotion(int x, int y)
{
	static bool firstCall= true;
	static int oldX = 0;
	static int oldY = 0;

	if(firstCall) {
		oldX = x;
		oldY = y;
		firstCall = false;
	}

	int deltaX = x - oldX;
	int deltaY = x - oldY;

	printf("New: %d x %d\n", x, y);
	printf("Old: %d x %d\n", oldX, oldY);
	printf("Delta: %d x %d\n\n", deltaX, deltaY);

	camRotZ = - (float)deltaY / 100.0f;
	//yRotCount = - (float)deltaY / 100.0f;

	oldX = x;
	oldY = y;
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
	glutMotionFunc(mouseMotion);
	glutMouseFunc(mouseButton);

	setup();
	//glutFullScreen();
	glutMainLoop();

	return 0;
};

