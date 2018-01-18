#pragma once
#include <vector>
#include <string>
#include <iostream>
#include <glad/glad.h>
#include "stb_image.h"

class Bloom
{
public:
	Bloom();
	~Bloom();
	void Initialize();

	unsigned int hdrFBO;
	unsigned int colorBuffers[2];
	unsigned int pingpongFBO[2];
	unsigned int pingpongBuffer[2];
};

