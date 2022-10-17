#pragma once
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <vector>
#include "Config.h"
#include "BoxZoom.h"
#include "FraktGeneration.h"

class FraktGraphics
{
private:
	GLuint vao;
	GLuint ebo;
	GLuint vba;
	GLFWwindow* window;
	std::vector<float> pixels;
	Config windowConf;
	BoxZoom boxHandler;
	FraktGeneration fGen;
public:
	FraktGraphics(int w, int h);
	void init();
	void run(int maxIter);
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};
