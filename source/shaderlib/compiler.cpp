#include "compiler.hpp"
#include "registry.hpp"
#include "../libs/file.hpp"
#include <stdio.h>

using namespace std;

// Forward decs
GLint makeShader(string source, GLenum sType);

GLint loadAndCompile(string vShaderFile, string fShaderFile)
{
	GLint pId(0);
	string fragTxt;
	string vertTxt;
	GLint fCmpId(0);
	GLint vCmpId(0);
	
	fragTxt = readFile("./source/shaders/fshader.fp");
	vertTxt = readFile("./source/shaders/vshader.vp");

	fCmpId = makeFragmentShader(fragTxt);
	vCmpId = makeVertexShader(vertTxt);

	printf("Frag Shader Compiled ID: %d\n", fCmpId);
	printf("Vert Shader Compiled ID: %d\n", vCmpId);

	pId = makeShaderProgram(vCmpId, fCmpId);
	Registry::setProgramId(pId);

	printf("Linked program ID: %d\n", pId);

	return pId;
}




bool compiledStatus(GLint shaderID)
{
	GLint compiled = 0;
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &compiled);
	if (compiled) {
		return true;
	}
	else {
		GLint logLength;
		glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
		char* msgBuffer = new char[logLength];
		glGetShaderInfoLog(shaderID, logLength, NULL, msgBuffer);
		printf ("%s\n", msgBuffer);
		delete (msgBuffer);
		return false;
	}
}

/**
 * Shader Program Compilation and Linking. 
 * Code adapted from course libraries. 
 */

GLint makeVertexShader(string source)
{
	return makeShader(source, GL_VERTEX_SHADER);
}

GLint makeFragmentShader(string source)
{
	return makeShader(source, GL_FRAGMENT_SHADER);
}

GLint makeShader(string source, GLenum sType) 
{
	const GLchar* src = (const GLchar*)source.c_str();
	GLint vShaderId = glCreateShader(sType);

	glShaderSource(vShaderId, 1, &src, NULL);
	glCompileShader(vShaderId);

	if (compiledStatus(vShaderId)) {
		return vShaderId;
	}
	return -1;
}

bool linkedStatus(GLint programID)
{
	GLint linked = 0;
	glGetProgramiv(programID, GL_LINK_STATUS, &linked);
	if (linked) {
		return true;
	}
	else {
		GLint logLength;
		glGetProgramiv(programID, GL_INFO_LOG_LENGTH, &logLength);
		char* msgBuffer = new char[logLength];
		glGetProgramInfoLog(programID, logLength, NULL, msgBuffer);
		printf ("%s\n", msgBuffer);
		delete (msgBuffer);
		return false;
	}
}

// A function that takes in the IDs for vertex and fragment shader,
// and returns the ID of a shader program that has been linked.
GLuint makeShaderProgram (GLint vertexShaderID, GLint fragmentShaderID) {
	printf ("Linking %d with %d\n", vertexShaderID, fragmentShaderID);
	GLuint programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram (programID);

	bool linked = linkedStatus (programID);

	if (linked) {
		return programID;
	}
	return -1;
}

