#include "misc.hpp"

// You shouldn't have to call this function (it's called from the functions
// that make shaders); it is a helper function to determine if a shader compiled correctly.
bool compiledStatus(GLint shaderID){
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

// This function takes in the source code for a vertex shader,
// compiles it, and returns the ID (number) of the shader. It returns
// -1 if the shader didn't compile
GLint makeVertexShader (char* shaderSource) {
	GLint vShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource (vShaderID, 1, (const GLchar**)&shaderSource, NULL);
	glCompileShader(vShaderID);
	bool compiledCorrectly = compiledStatus(vShaderID);
	if (compiledCorrectly) {
		return vShaderID;
	}
	return -1;
}

// This function takes in the source code for a fragment shader,
// compiles it, and returns the ID (number) of the shader. It returns
// -1 if the shader didn't compile
GLint makeFragmentShader (char* shaderSource) {
	GLint fShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource (fShaderID, 1, (const GLchar**)&shaderSource, NULL);
	glCompileShader(fShaderID);
	bool compiledCorrectly = compiledStatus(fShaderID);
	if (compiledCorrectly) {
		return fShaderID;
	}
	return -1;
}

// A helper function that takes in a program ID and returns whether
// or not it linked correctly.  If not, it prints out an error
// message letting you know what the problem was. This will
// be called from makeShaderProgram()
bool linkedStatus(GLint programID){
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

// This function reads the contents of a file and returns a char*
const char* readFile (const char* filename) {
	// Open the file
	FILE* fp = 0;
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

	return contents;
}

// This function asks the driver for a vertex array object, which will store information
// about what is being drawn, such as geometry, color information, texture information and so on.
// Hold on to the ID that this function returns, because you can use it later on!
GLuint createVertexArray() {
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	return vao;
}

// This function takes in a vertex array object ID and an array of vertices. It should be called 
// immediately after calling createVertexArray.  It returns a buffer ID (which you probably won't
// use).  NOTE: this assumes that you want triangles and not points or lines.
GLuint loadVertexPositions(GLuint shaderProgramID, GLuint vao, GLfloat* vVerts, GLuint numVertices) {
	glBindVertexArray(vao);
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer (GL_ARRAY_BUFFER, buffer);
	
	// Copy the array data into the active buffer (remember, there's a GLuint above that's active)
	glBufferData(GL_ARRAY_BUFFER, numVertices*3*sizeof(GLfloat), vVerts, GL_STATIC_DRAW);
	
	// Figure out where vPosition is in our shader
	GLuint loc = glGetAttribLocation(shaderProgramID, "vPosition");
	// Turn it on
	glEnableVertexAttribArray(loc);
	// Tell it how the data should be parsed
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);

	return buffer;
}

GLuint loadColors (GLuint shaderProgramID, GLuint vao, GLfloat* vColors, GLuint numVertices) {
	glBindVertexArray(vao);
	GLuint buffer;
	glGenBuffers(1, &buffer);
	glBindBuffer (GL_ARRAY_BUFFER, buffer);

	glBufferData(GL_ARRAY_BUFFER, numVertices*3*sizeof(GLfloat), vColors, GL_STATIC_DRAW);

	GLuint loc = glGetAttribLocation(shaderProgramID, "vColor");
	glEnableVertexAttribArray(loc);
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0);
	return buffer;
}

