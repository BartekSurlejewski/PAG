#pragma once
#define GLFW_INCLUDE_NONE
#include "Core.h"
#include "stdafx.h"
#include <memory>

int main()
{
	Core core;
	if (!core.Initialize())
	{
		return -1;
	}
	else
	{
		core.Update();
	}
	return 0;
}