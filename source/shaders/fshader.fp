#version 150

out vec4 fColor;
in vec4 color;		// from the vertex shader
in vec3 texCoord;		// From the vertex shader
uniform sampler2D texture;	 // This is texture in  

void main () {
	//fColor = color;
	fColor = texture2D(texture, texCoord);
}

