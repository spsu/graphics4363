from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *

"""
Shader Support Module
"""

def makeVertexShader(source):
	return makeShader(source, GL_VERTEX_SHADER)

def makeFragmentShader(source):
	return makeShader(source, GL_FRAGMENT_SHADER)

def makeShader(source, stype):
	"""
	Make a shader object.
	Compiles from the source supplied. 
	"""
	# Create empty shader object
	shaderId = glCreateShader(stype)

	# Inject source code into the shader, then compile
	glShaderSource(shaderId, source)
	glCompileShader(shaderId)

	if not glGetShaderiv(shaderId, GL_COMPILE_STATUS):
		print "Shader [%s] failed to compile correctly." % str(stype)
		print glGetShaderInfoLog(shaderId)
		return False

	return shaderId

def linkShaders(*args):
	"""
	Create shader program, then link shaders together.
	"""
	# Create an empty program
	programId = glCreateProgram()

	# Attach the shaders
	for shaderId in args:
		print args
		print type(args)
		glAttachShader(programId, shaderId)

	# Link the program
	glLinkProgram(programId)

	if not glGetProgramiv(programId, GL_LINK_STATUS):
		print "There was an issue linking the shader program."
		print glGetProgramInfoLog(programId)
		return False

	return programId

def compileShaderProg(vsh, fsh):
	"""
	Utilize simple compilation functions made for us by
	the PyOpenGL module maintainers.
	"""
	from OpenGL.GL.shaders import compileShader
	from OpenGL.GL.shaders import compileProgram

	vertShader = compileShader(vsh, GL_VERTEX_SHADER)
	fragShader = compileShader(fsh, GL_FRAGMENT_SHADER)

	return compileProgram(vertShader, fragShader)
