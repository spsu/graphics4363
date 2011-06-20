# Brandon Thomas Suit
# echelon at gmail dot com
# http://possibilistic.org
# http://github.com/echelon

"""
Shader Support Module.
Contains functions for compiling and linking shader programs.
"""

from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *

def compileShaderProg(vsh, fsh):
	"""
	Utilize the simple compilation functions made for us by the
	PyOpenGL module maintainers.
	"""
	from OpenGL.GL.shaders import compileShader
	from OpenGL.GL.shaders import compileProgram

	vertShader = compileShader(vsh, GL_VERTEX_SHADER)
	fragShader = compileShader(fsh, GL_FRAGMENT_SHADER)

	return compileProgram(vertShader, fragShader)

# ============================================
# Code below mimics Dr. Chastine's libraries. 
#

def makeVertexShader(source):
	return makeShader(source, GL_VERTEX_SHADER)

def makeFragmentShader(source):
	return makeShader(source, GL_FRAGMENT_SHADER)

def makeShader(source, stype):
	"""
	Compile a shader object from supplied source.
	Returns the shader object id.
	Mimics the C++ code by Dr. Jeff Chastine.
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

# XXX/FIXME: This is not working! Esoteric bug:
def linkShaders(*args):
	"""
	Link shader objects together, creating the shader program.
	Returns the shader program ID.
	Mimics the C++ code by Dr. Jeff Chastine.
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

