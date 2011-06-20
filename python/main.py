#!/usr/bin/env python

from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *

from shaders import * 
from vao import *

# Disable error checking for speed increase.
# OpenGL.ERROR_CHECKING = False

import sys

vertices = [
	0.8,  0.8,  0.0,  1.0,
	0.8, -0.8,  0.0,  1.0,
	-0.8, -0.8,  0.0,  1.0,
]

def setup():
	"""
	Setup function
	Compile the shaders, etc.
	"""

	def read(filename):
		"""Read entire file contents."""
		f = open(filename)
		txt = f.read()
		f.close()
		return txt

	vshader = read('vshader.glsl')
	fshader = read('fshader.glsl')

	#vs = makeVertexShader(vshader)
	#fs = makeFragmentShader(fshader)
	#pid = linkShaders(vs, fs)

	#print "Vertex Shader Compiled ID: %d" % vs
	#print "Fragment Shader Compiled ID: %d" % fs
	#print "Compiled Shader Program ID: %d" % pid

	pid = compileShaderProg(vshader, fshader)
	print "Compiled Shader Program ID: %d" % pid

	glUseProgram(pid)

	# Backface culling.
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	vao = createVertexArray()

	loadVertexPositions(pid, vao, vertices)
	#loadColors(pId, vao, colors, NUM_VERTS)

	glClearColor(0.10, 0.10, 0.10, 1.0)

def render():
	"""
	Render Routine.
	"""
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT)

	# glDrawArrays(GL_TRIANGLES, 0, NUM_VERTS)
	# glDrawArrays(GL_TRIANGLE_STRIP, 0, NUM_VERTS)

	# Double buffer -- swap current
	glutSwapBuffers()


def main():
	glutInit(sys.argv)
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA)
	glutInitWindowSize(512, 512)
	glutCreateWindow("Test Window")

	# TODO: GLEW extension wrangler error check.

	glutReshapeFunc(lambda w, h: glViewport(0, 0, w, h))
	#glutReshapeFunc(resizeCb)
	glutDisplayFunc(render)
	#glutKeyboardFunc()

	setup()
	glutMainLoop()

if __name__ == '__main__':
	main()

