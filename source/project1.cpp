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

using namespace std;

#include "libs/shaders.hpp"
#include "libs/vertex.hpp"
#include "libs/vao.hpp"
#include "geometry/basic.hpp"

int NUM_VERTS = 0;

void setup()
{
	string fragTxt;
	string vertTxt;
	GLint fCmp = 0;
	GLint vCmp = 0;
	GLint pId = 0;
	
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

	vao->loadVertices(verts, 12*3, pId);
	vao->loadColors(colors, 12*3, pId);
	
	glClearColor(0.10f, 0.10f, 0.10f, 1.0f);
}


void resizeCb(int w, int h)
{
	glViewport(0, 0, w, h);
}


void render(void) 
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

	glDrawArrays(GL_TRIANGLES, 0, NUM_VERTS);
	//glDrawArrays(GL_TRIANGLE_STRIP, 0, NUM_VERTS);

	// Double buffering -- swap current buffer.
	glutSwapBuffers();
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

