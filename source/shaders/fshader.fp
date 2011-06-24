#version 150

out vec4 fColor;
in vec2 texCoord;
uniform sampler2D texture;

void main () 
{
	fColor = texture2D(texture, texCoord);
}
