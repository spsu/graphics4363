/**
 * Project Three 
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
#include <math.h>

using namespace std;

#include "libs/file.hpp"
#include "libs/math.hpp"
//#include "libs/vertex.hpp"
#include "object/vao.hpp"
#include "object/Lib3dsLoader.hpp"
#include "object/TransformationStack.hpp"
#include "object/TransformationStackRegistry.hpp"
#include "shaderlib/compiler.hpp"
#include "shaderlib/registry.hpp"
#include "geometry/basic.hpp"

const string FRAGMENT_SHADER = "./source/shaders/fshader.fp";
const string VERTEX_SHADER = "./source/shaders/vshader.vp";

const float PI = atan(1) * 4;

// Program ID
GLint pId = 0;

// Matrices
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

// Timer counters. 
// Increment from 0 to 1 at different speeds. 
// Can be used in a variety of mathematical functions.
GLfloat timerFast = 0.0f;
GLfloat timerSlow = 0.0f;
GLfloat timerSuperSlow = 0.0f;
GLfloat timerFastImmediateReset = 0.0f;
GLfloat timerSlowImmediateReset = 0.0f;
GLfloat timerSuperSlowImmediateReset = 0.0f;
GLfloat timerUltraSlowImmediateReset = 0.0f;
GLfloat timerFastSlowReset = 0.0f;

// Camera movement
GLfloat camRotX = 0.0f;
GLfloat camRotY = 0.0f;

// For moving objects
GLfloat objMoveX = 0.0f;
GLfloat objMoveY = 0.0f;
GLfloat objMoveZ = 0.0f;
GLfloat objRotY = 0.0f;

GLuint lightLoc(0);
GLuint viewLoc(0);
GLuint sunColorLoc(0);
GLuint dcLoc(0);

/**
 * 3D Studio Models.
 * A pair of strings: Model file, texture file.
 */
typedef map<string, pair<string, string> > ModelFileMap;
ModelFileMap models;

// Forward declarations
void incrementTimers();
void drawArwing();
void drawTorusTower(unsigned int num);

/**
 * Map of VAOs. 
 * TODO: Enable copying of objects for easy propagation. 
 */
map<string, VertexArray*> V;

/**
 * Load models and textures in a predetermined path.
 * They all use the same globe model, anyway.
 */
void loadModel(string name)
{
	models[name.c_str()] = make_pair(
			"assets/solarsystem/globe.3ds",
			"assets/solarsystem/" + name + ".jpg"
	);
}

void setup()
{
	// Load models of celestial bodies.  
	loadModel("sun");
	loadModel("mercury");
	loadModel("venus1");
	loadModel("earth");
	loadModel("moon");
	loadModel("mars");
	loadModel("mars_phobos");
	loadModel("mars_deimos");
	loadModel("jupiter");
	loadModel("jupiter_ganymede");

	// Skyboxes -- Don't load all of them! 
	// Options: grimm, interstellar, vday
	models["vbox"] = make_pair(
			"assets/skyboxes/interstellar.3ds",
			"assets/skyboxes/interstellar2.png"
		);

	// Shape primitives -- no textures. 
	models["torus"] = make_pair("assets/torus.3ds", "");
	models["sphere"] = make_pair("assets/sphere.3ds", "");
	models["grid"] = make_pair("assets/grid.3ds", "");
	models["cone"] = make_pair("assets/cone.3ds", "");

	pId = loadAndCompile(FRAGMENT_SHADER, VERTEX_SHADER);

	// Use shader
	glUseProgram(pId);

	// Backface culling, depth test
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	// XXX: Init matrices
	mScale = new GLfloat[16];
	mTrans = new GLfloat[16];
	mP = new GLfloat[16];
	mMV = new GLfloat[16];
	mCam = new GLfloat[16];

	// XXX: Lighting
	lightLoc = glGetUniformLocation(pId, "lightPos");
	viewLoc = glGetUniformLocation(pId, "viewLoc");
	dcLoc = glGetUniformLocation(pId, "diffuseColor");
	sunColorLoc = glGetUniformLocation(pId, "sunColor");

	// mat, fov, aspect, near, far
	math::makePerspectiveProjectionMatrix(mP, 60.0f, 1.77f, 0.5f, 200000.0f);

	glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// Load all of the models above.
	for(ModelFileMap::const_iterator i = models.begin(); i != models.end(); i++)
	{
		string name = i->first;
		string modFile = i->second.first;
		string texFile = i->second.second;

		Lib3dsLoader loader = Lib3dsLoader(modFile);
		V[name] = loader.buildVao();
		if(texFile.size()) {
			V[name]->loadTexture(texFile);
		}
	}

	// Set render mode
	V["grid"]->setPrimitiveMode(GL_LINES);

	// Initial offset
	TransformationStack* transformStack = TransformationStackRegistry::get();
	transformStack->translate(0.0f, 0.0f, -25000.0f);
	transformStack->rotate(-0.3f, 0.0f, -0.2f);
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

	// Increment the timers this function uses. 
	incrementTimers();

	// XXX: Lighting
	GLfloat lightPos[] = { xTrans, yTrans, zTrans };
	GLfloat viewPos[] = { 1.0f, camRotY*200.0f, 1.0f};

	GLfloat diffuseColor[] = { 0.02f, 0.02f, 0.02f, 1.0f };
	GLfloat sunColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	glUniform3fv(lightLoc, 1, lightPos);
	glUniform3fv(viewLoc, 1, viewPos);
	glUniform4fv(dcLoc, 1, diffuseColor);
	glUniform4fv(sunColorLoc, 1, sunColor);

	// XXX: Camera Movement. 
	transformStack->push();
	transformStack->rotate(0.0f, camRotY, 0.0f);
	transformStack->applyTransform();
	transformStack->push();
	//transformStack->translate(xTrans, yTrans, zTrans);
	transformStack->applyTransform();

	// Perspective matrix
	GLuint p = glGetUniformLocation(pId, "p");
	glUniformMatrix4fv(p, 1, GL_TRUE, mP);

	// Modelview Matrix
	//GLuint r = glGetUniformLocation(pId, "mv");
	//glUniformMatrix4fv(r, 1, GL_TRUE, transformStack->top());

	// Sun
	V["sun"]->scale(2200.0f);
	V["sun"]->draw();

	// Mercury 
	transformStack->push();
	transformStack->rotate(0.0f, timerSlowImmediateReset*2*PI, 0.15f); 
	transformStack->applyTransform();
	transformStack->translate(0.0f, 0.0f, 6000.0f);
	transformStack->applyTransform();
	V["mercury"]->scale(150.0f);
	V["mercury"]->rotate(0.0f, - timerSlowImmediateReset*2*PI, 0.0f);
	V["mercury"]->draw();
	transformStack->pop(); // Pop mercury rotation

	// Venus 
	transformStack->push();
	transformStack->rotate(0.0f, -timerSuperSlowImmediateReset*2*PI, -0.15f); 
	transformStack->applyTransform();
	transformStack->translate(0.0f, 0.0f, 8000.0f);
	transformStack->applyTransform();
	V["venus1"]->scale(200.0f);
	V["venus1"]->rotate(0.0f, - timerSlowImmediateReset*2*PI, 0.0f);
	V["venus1"]->draw();
	transformStack->pop(); // Pop mercury rotation

	// Earth
	transformStack->push();
	transformStack->rotate(0.0f,timerSuperSlowImmediateReset*2*PI, 0.0f); 
	transformStack->applyTransform();
	transformStack->translate(0.0f, 0.0f, 12000.0f);
	transformStack->applyTransform();
	V["earth"]->scale(400.0f);
	V["earth"]->rotate(0.0f, - timerSlowImmediateReset*2*PI, 0.0f);
	V["earth"]->draw();
	// Moon
	transformStack->push();
	transformStack->rotate(0.0f,timerSuperSlowImmediateReset*2*PI, 0.0f); 
	transformStack->applyTransform();
	V["moon"]->scale(220.0f);
	V["moon"]->translate(0.0f, 200.0f, 1800.0f);
	V["moon"]->rotate(0.0f, timerSlowImmediateReset*2*PI, 0.0f);
	V["moon"]->draw();
	transformStack->pop(); // Pop moon rotation
	transformStack->pop(); // Pop earth rotation

	// Mars
 	transformStack->push();
	transformStack->rotate(0.0f, PI - timerSuperSlowImmediateReset*2*PI, -0.2f); 
	transformStack->applyTransform();
	transformStack->translate(0.0f, 0.0f, 16000.0f);
	transformStack->applyTransform();
	V["mars"]->scale(320.0f);
	V["mars"]->rotate(0.0f, - timerSlowImmediateReset*2*PI, 0.0f);
	V["mars"]->draw();
	// Phobos 
	transformStack->push();
	transformStack->rotate(0.0f, timerSlowImmediateReset*2*PI, 0.7f); 
	transformStack->applyTransform();
	V["mars_phobos"]->scale(100.0f);
	V["mars_phobos"]->translate(0.0f, -200.0f, 1700.0f);
	V["mars_phobos"]->rotate(0.0f, timerSlowImmediateReset*2*PI, 0.0f);
	V["mars_phobos"]->draw();
	transformStack->pop(); // Pop phobos rotation
	// Deimos 
	transformStack->push();
	transformStack->rotate(0.0f, PI - timerSuperSlowImmediateReset*2*PI, 0.0f); 
	transformStack->applyTransform();
	V["mars_deimos"]->scale(120.0f);
	V["mars_deimos"]->translate(0.0f, 300.0f, 1200.0f);
	V["mars_deimos"]->rotate(0.0f, PI - timerSlowImmediateReset*2*PI, -0.3f);
	V["mars_deimos"]->draw();
	transformStack->pop(); // Pop deimos rotation
	transformStack->pop(); // Pop mars rotation

	// Jupiter 
 	transformStack->push();
	transformStack->rotate(0.0f, 2/3.0 * PI + timerSuperSlowImmediateReset*2*PI, -0.2f); 
	transformStack->applyTransform();
	transformStack->translate(0.0f, 0.0f, 23000.0f);
	transformStack->applyTransform();
	V["jupiter"]->scale(1200.0f);
	V["jupiter"]->rotate(0.0f, - timerUltraSlowImmediateReset*2*PI, 0.0f);
	V["jupiter"]->draw();
	// Ganymede 
	transformStack->push();
	transformStack->rotate(0.0f, timerSlowImmediateReset*2*PI, 0.7f); 
	transformStack->applyTransform();
	V["jupiter_ganymede"]->scale(400.0f);
	V["jupiter_ganymede"]->translate(0.0f, -200.0f, 1700.0f);
	V["jupiter_ganymede"]->rotate(0.0f, timerSlowImmediateReset*2*PI, 0.0f);
	V["jupiter_ganymede"]->draw();
	transformStack->pop(); // Pop ganymede rotation
	transformStack->pop(); // Pop jupiter rotation

	// Skybox
	V["vbox"]->rotate(1.50f, 0.0f, 0.0f);
	V["vbox"]->scale(200000.0);
	V["vbox"]->draw();

	transformStack->pop();
	transformStack->pop();

	// Double buffering -- swap current buffer.
	glutSwapBuffers();
	glutPostRedisplay();
}

void keypress(unsigned char key, int x, int y)
{
	switch (key) {
		// Translate -- This took me forever to figure out!
		case 'w': 
			//zTrans += cos(camRotY) * 20;
			//xTrans += sin(camRotY) * 20;
			zTrans += 200;
			break;

		case 'a':
			//xTrans += cos(camRotY) * 20;
			//zTrans -= sin(camRotY) * 20;
			xTrans -= 200;
			break;

		case 's':
			//zTrans -= cos(camRotY) * 20;
			//xTrans -= sin(camRotY) * 20;
			zTrans -= 200;
			break;
		case 'd':
			//xTrans -= cos(camRotY) * 20;
			//zTrans += sin(camRotY) * 20;
			xTrans += 200;
			break;

		// Just in case...
		case 'u': 
			yTrans += 20.0f;
			break;
		case 'i':
			yTrans -= 20.0f;
			break;
		// Move Object.
		case 'z':
			objMoveX += 100.0f;
			break;
		case 'x':
			objMoveX -= 100.0f;
			break;
		case 'c':
			objMoveZ -= 100.0f;
			break;
		case 'v':
			objMoveZ += 100.0f;
			break;
		case 'b':
			objMoveY += 10.0f;
			break;
		case 'n':
			objMoveY -= 5.0f;
			break;
		case 'm':
			objRotY += 0.1f;
			break;
		case ',':
			objRotY -= 0.0f;
			break;

		case 'q':
			exit(1);
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

	//int deltaX = x - oldX;
	int deltaY = x - oldY;

	// Set the camera rotation. 
	camRotY = - (float)deltaY / 100.0f;

	oldX = x;
	oldY = y;
}

// TODO: This could all be replaced with a call to system time...
void incrementTimers()
{
	const float FAST = 0.1f;
	const float SLOW = 0.005f;
	const float SUPER_SLOW = 0.002f;
	const float ULTRA_SLOW = 0.0001f;

	static bool tFastD = true;
	static bool tSlowD = true;
	static bool tSuperSlowD = true;
	static bool tFastSlowD = true;

	timerFastImmediateReset += FAST;
	timerSlowImmediateReset += SLOW;
	timerSuperSlowImmediateReset += SUPER_SLOW;
	timerUltraSlowImmediateReset += ULTRA_SLOW;
	
	if(timerFastImmediateReset >= 1.0f) {
		timerFastImmediateReset = 0.0f;
	}

	if(timerSlowImmediateReset >= 1.0f) {
		timerSlowImmediateReset = 0.0f;
	}

	if(timerSuperSlowImmediateReset >= 1.0f) {
		timerSuperSlowImmediateReset = 0.0f;
	}

	if(timerUltraSlowImmediateReset >= 1.0f) {
		timerUltraSlowImmediateReset = 0.0f;
	}

	if(tFastD) {
		timerFast += FAST;
		if(timerFast > 1.0f) {
			tFastD = false;
		}
	}
	else {
		timerFast -= FAST;
		if(timerFast < 0.0f) {
			tFastD = true;
		}
	}

	if(tSlowD) {
		timerSlow += SLOW;
		if(timerSlow > 1.0f) {
			tSlowD = false;
		}
	}
	else {
		timerSlow -= SLOW;
		if(timerSlow < 0.0f) {
			tSlowD = true;
		}
	}
	if(tSuperSlowD) {
		timerSuperSlow += SUPER_SLOW;
		if(timerSuperSlow > 1.0f) {
			tSuperSlowD = false;
		}
	}
	else {
		timerSuperSlow -= SUPER_SLOW;
		if(timerSuperSlow < 0.0f) {
			tSuperSlowD = true;
		}
	}
	if(tFastSlowD) {
		timerFastSlowReset += FAST;
		if(timerFastSlowReset > 1.0f) {
			tFastSlowD = false;
		}
	}
	else {
		timerFastSlowReset -= SLOW;
		if(timerFastSlowReset < 0.0f) {
			tFastSlowD= true;
		}
	}
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_MULTISAMPLE);
	glutInitWindowSize(1020, 600);
	glutCreateWindow ("Project 3");

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

