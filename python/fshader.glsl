#version 150

out vec4 fColor;
in vec4 color;		// from the vertex shader

void main () {
	fColor = color;
}
