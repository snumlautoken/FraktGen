#include "BoxZoom.h"
#include <algorithm>
#include <iostream>

bool BoxZoom::getInput(GLFWwindow* window, Config& windowConf)
{
	bool boxEnd = false;

	int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);

	if (state == GLFW_PRESS && !boxActive)
	{
		glfwGetCursorPos(window, &xpos, &ypos);
		boxActive = true;
	}

	//state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	if (state == GLFW_RELEASE && boxActive)
	{
		double xpos2, ypos2;
		glfwGetCursorPos(window, &xpos2, &ypos2);

		double xUpperTemp = std::max(xpos, xpos2) / windowConf.width * (windowConf.xUpper - windowConf.xLower) + windowConf.xLower;
		double xLowerTemp = std::min(xpos, xpos2) / windowConf.width * (windowConf.xUpper - windowConf.xLower) + windowConf.xLower;
		windowConf.xUpper = xUpperTemp;
		windowConf.xLower = xLowerTemp;

		double yUpperTemp = std::max(ypos, ypos2) / windowConf.height * (windowConf.yUpper - windowConf.yLower) + windowConf.yLower;
		double yLowerTemp = std::min(ypos, ypos2) / windowConf.height * (windowConf.yUpper - windowConf.yLower) + windowConf.yLower;
		windowConf.yUpper = yUpperTemp;
		windowConf.yLower = yLowerTemp;
		boxActive = false;
		boxEnd = true;
	}

	state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	if (state == GLFW_PRESS)
	{
		windowConf.xUpper = xUpperOriginal;
		windowConf.xLower = xLowerOriginal;
		windowConf.yUpper = yUpperOriginal;
		windowConf.yLower = yLowerOriginal;
		boxEnd = true;
	}

	return boxEnd;
}
