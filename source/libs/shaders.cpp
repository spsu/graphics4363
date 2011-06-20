#include "shaders.hpp"
#include <stdio.h>
#include <iostream>
#include <fstream>

using namespace std;

// Forward decs
GLint makeShader(string source, GLenum sType);

string readFile(string filename)
{
	/*FILE* fp = 0;
	fp = fopen(filename, "r");
	if(!fp) {
		fprintf(stderr, "There was a problem opening: %s\n", filename);
		return "";
	}

	// Go to the beginning and get the length of the file
	fseek (fp, 0, SEEK_END);
	long file_length = ftell(fp);

	// Go back to the beginning and read the contents
	fseek (fp, 0, SEEK_SET);
	char* contents = new char[file_length+1];
	for (int i = 0; i < file_length+1; i++) {
		contents[i] = 0;
	}
	fread (contents, 1, file_length, fp);
	contents[file_length] = '\0';
	fclose(fp);

	return contents;*/

	//ifstream fs;

	//fs.open(filename, ifstream::in);

	ifstream fs(filename.c_str());

	return string(istreambuf_iterator<char>(fs), istreambuf_iterator<char>());

	//fs.close();
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

