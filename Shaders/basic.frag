#version 400

in vec2 TexCoord;

out vec4 color;

uniform sampler2D Texture;

void main()
{
	color = texture(Texture, TexCoord);
}