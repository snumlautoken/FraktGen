#pragma once
#include <GLFW/glfw3.h>
#include "Config.h"
class BoxZoom
{
private:
	double xUpperOriginal = 0.47;
	double xLowerOriginal = -2.0;
	double yUpperOriginal = 1.12;
	double yLowerOriginal = -1.12;
	double xpos;
	double ypos;
	bool boxActive = false;
public:
	bool getInput(GLFWwindow* window, Config& windowConf);
};
