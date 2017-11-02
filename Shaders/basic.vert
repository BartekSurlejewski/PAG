#version 400

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 texCoord;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(vertexPosition, 1.0f);
	TexCoord = texCoord;
}