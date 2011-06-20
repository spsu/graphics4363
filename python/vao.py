# Brandon Thomas Suit
# echelon at gmail dot com
# http://possibilistic.org
# http://github.com/echelon

"""
Work with Vertex Array Objects
"""

from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *
from OpenGL.GL.ARB import vertex_array_object as VAO
from OpenGL.GL.ARB.vertex_array_object import glBindVertexArray

FLOAT_SIZE = 4 # Ugly...

def glGenVertexArray():
	"""
	Adapted from https://bitbucket.org/tartley/gltutpy/src/tip/
	t01.hello-triangle/glwrap.py
	"""
	vaoId = GLuint(0)
	VAO.glGenVertexArrays(1, vaoId)
	return vaoId.value

class VertexArray(object):
	"""
	Object-Oriented VAOs.
	"""

	def __init__(self):
		"""CTOR."""
		self.vao = glGenVertexArray() # Create one.

	def __del__(self):
		"""DTOR."""
		#glDelete ... 
		pass # TODO

	def bind(self):
		"""Must bind to modify state."""
		glBindVertexArray(self.vao)


def createVertexArray():
	"""
	Create and Bind a Vertex Array Object.
	"""
	# Create one empty Vertex Array Object and bind it.
	vao = glGenVertexArray()
	glBindVertexArray(vao)
	return vao

def loadVertexPositions(programId, vao, vertices):
	"""Load Vertex Positions."""
	glBindVertexArray(vao)

	# Create one buffer and bind it. 
	bufferId = glGenBuffers(1)
	glBindBuffer(GL_ARRAY_BUFFER, bufferId)

	# Create new data store for bound buffer
	# TODO: Probably not correct usage
	# XXX, SEE: https://bitbucket.org/tartley/gltutpy/src/tip/
	# t01.hello-triangle/HelloTriangle.py

	arrayType = GLfloat * len(vertices)
	glBufferData(GL_ARRAY_BUFFER, len(vertices)*FLOAT_SIZE, 
			arrayType(*vertices), GL_STATIC_DRAW)

	loc = glGetAttribLocation(programId, 'vPosition')
	glEnableVertexAttribArray(loc)
	
	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0)

	return bufferId

def loadColors(programId, vao, colors):
	"""Load colors"""
	glBindVertexArray(vao)

	# Create one buffer and bind it. 
	bufferId = glGenBuffers(1)
	glBindBuffer(GL_ARRAY_BUFFER, bufferId)

	# Create new data store for bound buffer
	# TODO: Probably not correct usage
	# XXX, SEE: https://bitbucket.org/tartley/gltutpy/src/tip/
	# t01.hello-triangle/HelloTriangle.py
	glBufferData(GL_ARRAY_BUFFER, len(colors)*3, colors, GL_STATIC_DRAW)

	loc = glGetAttribLocation(programId, 'vColor')
	glEnableVertexAttribArray(loc)

	glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0, 0)

	return bufferId

