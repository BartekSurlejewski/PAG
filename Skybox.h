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
	void loadCubemap();

	unsigned int cubemapTexture;
private:
	std::vector<std::string> faces = {
		"Skyboxes/standard/right.jpg",
		"Skyboxes/standard/left.jpg",
		"Skyboxes/standard/top.jpg",
		"Skyboxes/standard/bottom.jpg",
		"Skyboxes/standard/back.jpg",
		"Skyboxes/standard/front.jpg"
	};

};

