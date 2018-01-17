#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"

class Skybox
{
public:
	Skybox();
	~Skybox();

	unsigned int cubemapTexture;
private:
	unsigned int loadCubemap(std::vector<std::string>);

	std::vector<std::string> faces;
};

