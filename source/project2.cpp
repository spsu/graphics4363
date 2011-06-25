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
#include <math.h>

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
GLfloat timerFastSlowReset = 0.0f;

// Camera movement
GLfloat camRotZ = 0.0f;

GLuint lightLoc(0);
GLuint dcLoc(0);

/**
 * 3D Studio Models.
 * A pair of strings: Model file, texture file.
 */
typedef map<string, pair<string, string> > ModelFileMap;
ModelFileMap models;

// Older models.
VertexArray* sphere = 0;

// Forward declarations
void incrementTimers();
void drawArwing();
void drawTorusTower(int num);

/**
 * Map of VAOs. 
 * TODO: Enable copying of objects for easy propagation. 
 */
map<string, VertexArray*> V;

void setup()
{
	// Characters
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
	models["dodongo"] = make_pair(
			"assets/nintendo/dodongo.3ds",
			"assets/nintendo/dodongo.png"
		);
	models["linkA"] = make_pair(
			"assets/nintendo/linkA.3ds",
			"assets/nintendo/linkA.png"
		);
	models["arwing"] = make_pair(
			"assets/nintendo/arwing.3ds",
			"assets/nintendo/arwing.png"
		);

	// Levels
	models["hyrule"] = make_pair(
			"assets/nintendo/levels/hyrule.3ds",
			"assets/nintendo/levels/hyrule.png"
		);
	models["kokiri"] = make_pair(
			"assets/nintendo/levels/kokiri.3ds",
			"assets/nintendo/levels/kokiri.png"
		);
	models["hcastle"] = make_pair(
			"assets/nintendo/levels/hyrulecastle.3ds",
			"assets/nintendo/levels/hyruleca.bmp"
		);

	// Shape primitives
	models["torus"] = make_pair(
			"assets/torus.3ds",
			""
		);

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
	dcLoc = glGetUniformLocation(pId, "diffuseColor");

	// mat, fov, aspect, near, far
	math::makePerspectiveProjectionMatrix(mP, 60.0f, 1.77f, 0.5f, 20000.0f);

	//glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

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

	// Initial offset
	TransformationStack* transformStack = TransformationStackRegistry::get();
	transformStack->translate(0.0f, 0.0f, 0.0f);
	//transformStack->rotate(xRotCount, yRotCount, zRotCount);
	//transformStack->rotate(1.5f, 0.0f, 0.0f); // XXX: Hyrule
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

	// Increment the timers this function uses. 
	incrementTimers();

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

	// XXX: Camera Movement. 
	transformStack->push();
	transformStack->rotate(0.0f, camRotZ, 0.0f);
	transformStack->applyTransform();
	transformStack->push();
	transformStack->translate(xTrans, yTrans, zTrans);
	transformStack->applyTransform();

	// Modelview Matrix
	GLuint r = glGetUniformLocation(pId, "mv");
	glUniformMatrix4fv(r, 1, GL_TRUE, transformStack->top());

	// Perspective matrix
	GLuint p = glGetUniformLocation(pId, "p");
	glUniformMatrix4fv(p, 1, GL_TRUE, mP);


	//vao1->translate(2.0f, 0.0f, 0.0f);
	//vao1->rotate(20.0f + xRotCounter, 40.0f + yRotCounter, zRotCounter);

	//vao2->translate(-3.0f, 0.0f, 0.0f);
	//vao2->rotate(20.0f + xRotCounter, 40.0f + yRotCounter, zRotCounter);
	//vao1->scale(0.7f, 0.3f, 0.5f);

	// XXX: Levels
	V["kokiri"]->scale(1000.0, 1000.0, 1000.0);
	V["kokiri"]->rotate(1.50f, 0.7f, 0.0f);
	V["kokiri"]->translate(4000.0f, -270.0f, -2000.0f);

	V["hyrule"]->scale(1000.0, 1000.0, 1000.0);
	V["hyrule"]->rotate(1.50f, 0.0f, 0.0f);
	V["hyrule"]->translate(-2000.0f, -270.0f, -2500.0f);

	V["hcastle"]->scale(1200.0);
	V["hcastle"]->rotate(1.50f, 0.0f, 0.0f);
	V["hcastle"]->translate(-2200.0f, 500.0f, -8500.0f);

	// XXX: Characters.
	V["whomp"]->scale(80.0f);
	V["whomp"]->rotate(1.50f, -1.9f, 0.0f);
	V["whomp"]->translate(-3000.0f, (timerSlow*300.0f)+200, -5800.0f);

	V["luigi"]->scale(10.0f);
	V["luigi"]->rotate(1.50f, 1.0f, (timerFast/4)*0.2f);
	V["luigi"]->translate(1000.0f, -150.0f, -2000.0f);

	V["dodongo"]->scale(10.7);
	V["dodongo"]->rotate(1.50f, (timerSlow/4*0.7f), (timerFastSlowReset/4)*0.1f);
	V["dodongo"]->translate(-1247.0f, -400.0f, -502.0f);
	V["dodongo"]->draw();

	V["linkA"]->scale(0.7);
	V["linkA"]->rotate(1.50f, 1.0f, (timerFast/4)*0.1f);
	V["linkA"]->translate(-847.0f, -170.0f, -5702.0f);
	V["linkA"]->draw();

	// Arwings have a flight pattern. 
	V["arwing"]->scale(7.0f);
	drawArwing();

	// Shapes
	V["torus"]->scale(300.0f);
	drawTorusTower(4);

	// Draw models.
	V["kokiri"]->draw();
	V["hyrule"]->draw();
	V["whomp"]->draw();
	V["luigi"]->draw();
	V["hcastle"]->draw();

	printf("Translation: %f, %f, %f\n", xTrans, yTrans, zTrans);

	transformStack->pop();
	transformStack->pop();

	// Double buffering -- swap current buffer.
	glutSwapBuffers();
	glutPostRedisplay();
}

void drawArwing()
{
	static bool change = false;
	
	GLfloat rX = 0;
	GLfloat rY = 0;
	GLfloat rZ = 0;

	GLfloat tx = 0;
	GLfloat ty = 0;
	GLfloat tz = 0; 
	
	tx = sin(timerSuperSlowImmediateReset*2*PI) * 7000;
	tz = cos(timerSuperSlowImmediateReset*2*PI) * 10000;

	// Keep facing forward
	rY = -2 * PI * timerSuperSlowImmediateReset -PI / 2; 

	V["arwing"]->rotate(1.50f + rX, 0.0f + rY, 0.0f + rZ);
	V["arwing"]->translate(-850.0f + tx, 2000.0f + ty, -10700.0f + tz);
	V["arwing"]->draw();
}

void drawTorusTower(int num)
{
	int sign = 1;
	int xOffset = 0;
	int yOffset = 0;
	for(unsigned int i = 0; i < num; i++)
	{
		float x = timerSuperSlowImmediateReset*2*PI; 

		V["torus"]->rotate(PI/(4-2*x)*sign, timerSlowImmediateReset*2*PI, 0);
		V["torus"]->translate(-4270.0f, 360.0f + yOffset, -2200.0f);
		V["torus"]->draw();
		sign *= -1;
		yOffset += 620 + timerSlow*2*PI*-50;
	}
}

void keypress(unsigned char key, int x, int y)
{
	switch (key) {
		case 'q':
			exit(1);
			break;

		// Translate -- This took me forever to figure out!
		case 'w': 
			zTrans += cos(camRotZ) * 20;
			xTrans += sin(camRotZ) * 20;
			break;

		case 'a':
			xTrans += cos(camRotZ) * 20;
			zTrans -= sin(camRotZ) * 20;
			break;

		case 's':
			zTrans -= cos(camRotZ) * 20;
			xTrans -= sin(camRotZ) * 20;
			break;
		case 'd':
			xTrans -= cos(camRotZ) * 20;
			zTrans += sin(camRotZ) * 20;
			break;
		
		// Just in case...
		case 'u': 
			yTrans += 20.0f;
			break;
		case 'i':
			yTrans -= 20.0f;
			break;
			
		// Reset position.
		case 'p': 
			yTrans = 0.0f;
			xTrans = 0.0f;
			zTrans = 0.0f;
			camRotZ = 0.0f;
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
	camRotZ = - (float)deltaY / 100.0f;

	oldX = x;
	oldY = y;
}

// TODO: This could all be replaced with a call to system time...
void incrementTimers()
{
	const float FAST = 0.1f;
	const float SLOW = 0.01f;
	const float SUPER_SLOW = 0.002f;

	static bool tFastD = true;
	static bool tSlowD = true;
	static bool tSuperSlowD = true;
	static bool tFastSlowD = true;

	timerFastImmediateReset += FAST;
	timerSlowImmediateReset += SLOW;
	timerSuperSlowImmediateReset += SUPER_SLOW;
	
	if(timerFastImmediateReset >= 1.0f) {
		timerFastImmediateReset = 0.0f;
	}

	if(timerSlowImmediateReset >= 1.0f) {
		timerSlowImmediateReset = 0.0f;
	}

	if(timerSuperSlowImmediateReset >= 1.0f) {
		timerSuperSlowImmediateReset = 0.0f;
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

