#version 400 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform vec3 cameraPos;
uniform samplerCube skybox;
uniform bool R;

void main()
{             
	vec3 Ref;
    vec3 I = normalize(Position - cameraPos);

	if (R == true)
	{
		float ratio = 1.00/ 2.42;
		Ref = refract(I, normalize(Normal), ratio);
	}
	else
	{
		Ref = reflect(I, normalize(Normal));
	}
    FragColor = vec4(texture(skybox, Ref).rgb, 1.0);
}