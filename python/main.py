#!/usr/bin/env python

from OpenGL.GL import *
from OpenGL.GLU import *
from OpenGL.GLUT import *

from shaders import * 

# Disable error checking for speed increase.
# OpenGL.ERROR_CHECKING = False

import sys

#def resizeCb(w, h):
#	"""Window reshape callback"""
#	glViewport(0, 0, w, h)

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


def main():
	glutInit(sys.argv)
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGBA)
	glutInitWindowSize(512, 512)
	glutCreateWindow("Test Window")

	# TODO: GLEW extension wrangler error check.

	glutReshapeFunc(lambda w, h: glViewport(0, 0, w, h))
	#glutReshapeFunc(resizeCb)
	#glutDisplayFunc()
	#glutKeyboardFunc()

	setup()
	glutMainLoop()

if __name__ == '__main__':
	main()

