#include "stdafx.h"
#include "UI.h"


UI::UI(const int width, const int height, GLFWwindow* window)
{
	TwInit(TW_OPENGL, NULL);
	TwWindowSize(width, height);
	//bar = TwNewBar("Model administration");
	/*bar = TwNewBar("Model administration");

	TwAddVarRW(bar, "Aa", TW_TYPE_FLOAT, &Aa, "");
	TwAddVarRW(bar, "Bb", TW_TYPE_CDSTRING, &Bb, "");
	TwAddVarRW(bar, "Cc", TW_TYPE_CDSTRING, &Cc, "");*/

	/*glfwSetCursorPosCallback(window, (GLFWcursorposfun)TwEventMousePosGLFW);
	glfwSetMouseButtonCallback(window, (GLFWmousebuttonfun)TwEventMouseButtonGLFW);*/
}


UI::~UI()
{
}
