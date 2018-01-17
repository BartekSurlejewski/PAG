#include "stdafx.h"
#include "Skybox.h"


Skybox::Skybox()
{
	/*faces = {
		"Skyboxes/mp_blackgold/blackgold_rt.tga",
		"Skyboxes/mp_blackgold/blackgold_lf.tga",
		"Skyboxes/mp_blackgold/blackgold_up.tga",
		"Skyboxes/mp_blackgold/blackgold_dn.tga",
		"Skyboxes/mp_blackgold/blackgold_bk.tga",
		"Skyboxes/mp_blackgold/blackgold_ft.tga"
	};*/

	//cubemapTexture = loadCubemap(faces);
}


Skybox::~Skybox()
{
}

void Skybox::loadCubemap()
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	cubemapTexture = textureID;
}
