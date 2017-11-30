#pragma once
#include <AntTweakBar/AntTweakBar.h>
#include <GLFW/glfw3.h>

class UI
{
public:
	UI(const int, const int, GLFWwindow*);
	~UI();

	TwBar* bar;

private:


	std::string Aa;
};

