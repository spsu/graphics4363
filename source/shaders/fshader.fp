#version 150

out vec4 fColor;
in vec2 texCoord;
uniform sampler2D texture;

varying vec4 color;		// Going to (and defined in) your fragment shader

void main () 
{
	// XXX: What's the difference? 
	//fColor = texture2D(texture, texCoord);
	gl_FragColor = texture2D(texture, texCoord) + color;
}
