#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 2) in vec2 vertexTexture;

out vec2 TexCoord;

uniform mat4 transform;
uniform mat4 wvp;

void main()
{
	gl_Position = wvp * transform * vec4(vertexPosition, 1.0f);
	TexCoord = vertexTexture;
}