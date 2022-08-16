#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <thread>
#include <iostream>
#include <chrono>
#include <complex>
#include <vector>
#include <tuple>

const int width = 900;
const int height = 900;

long double xUpperOriginal = 0.47;
long double xLowerOriginal = -2.0;

long double yUpperOriginal = 1.12;
long double yLowerOriginal = -1.12;


long double xUpper = 0.47;
long double xLower = -2.0;

long double yUpper = 1.12;
long double yLower = -1.12;

double xpos;
double ypos;

bool update = false;

const int processor_count = std::thread::hardware_concurrency();

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		glfwGetCursorPos(window, &xpos, &ypos);
	}
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		double xpos2, ypos2;
		glfwGetCursorPos(window, &xpos2, &ypos2);

		double xUpperTemp = std::max(xpos, xpos2) / 900 * (xUpper - xLower) + xLower;
		double xLowerTemp = std::min(xpos, xpos2) / 900 * (xUpper - xLower) + xLower;
		xUpper = xUpperTemp;
		xLower = xLowerTemp;

		double yUpperTemp = std::max(ypos, ypos2) / 900 * (yUpper - yLower) + yLower;
		double yLowerTemp = std::min(ypos, ypos2) / 900 * (yUpper - yLower) + yLower;
		yUpper = yUpperTemp;
		yLower = yLowerTemp;

		update = true;
	}
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
	{
		xUpper = xUpperOriginal;
		xLower = xLowerOriginal;
		yUpper = yUpperOriginal;
		yLower = yLowerOriginal;
		update = true;
	}

}

void palette(std::vector<float>& pixels, int x, int y, int iter, int maxIter)
{
	if (iter < maxIter && iter > 0) {
		int i = iter % 16;
		std::tuple<float,float,float> mapping[16];
		mapping[0] = std::tuple<float,float,float>(66.0f/255.0f, 30.0f/255.0f, 15.0f/255.0f);
		mapping[1] = std::tuple<float,float,float>(25.0f/255.0f, 7.0f/255.0f, 26.0f/255.0f);
		mapping[2] = std::tuple<float,float,float>(9.0f/255.0f, 1.0f/255.0f, 47.0f/255.0f);
		mapping[3] = std::tuple<float,float,float>(4.0f/255.0f, 4.0f/255.0f, 73.0f/255.0f);
		mapping[4] = std::tuple<float,float,float>(0.0f/255.0f, 7.0f/255.0f, 100.0f/255.0f);
		mapping[5] = std::tuple<float,float,float>(12.0f/255.0f, 44.0f/255.0f, 138.0f/255.0f);
		mapping[6] = std::tuple<float,float,float>(24.0f/255.0f, 82.0f/255.0f, 177.0f/255.0f);
		mapping[7] = std::tuple<float,float,float>(57.0f/255.0f, 125.0f/255.0f, 209.0f/255.0f);
		mapping[8] = std::tuple<float,float,float>(134.0f/255.0f, 181.0f/255.0f, 229.0f/255.0f);
		mapping[9] = std::tuple<float,float,float>(211.0f/255.0f, 236.0f/255.0f, 248.0f/255.0f);
		mapping[10] = std::tuple<float,float,float>(241.0f/255.0f, 233.0f/255.0f, 191.0f/255.0f);
		mapping[11] = std::tuple<float,float,float>(248.0f/255.0f, 201.0f/255.0f, 95.0f/255.0f);
		mapping[12] = std::tuple<float,float,float>(255.0f/255.0f, 170.0f/255.0f, 0.0f/255.0f);
		mapping[13] = std::tuple<float,float,float>(204.0f/255.0f, 128.0f/255.0f, 0.0f/255.0f);
		mapping[14] = std::tuple<float,float,float>(153.0f/255.0f, 87.0f/255.0f, 0.0f/255.0f);
		mapping[15] = std::tuple<float,float,float>(106.0f/255.0f, 52.0f/255.0f, 3.0f/255.0f);
		pixels[3 * (width * y + x)] = std::get<0>(mapping[i]);
		pixels[3 * (width * y + x) + 1] = std::get<1>(mapping[i]);
		pixels[3 * (width * y + x) + 2] = std::get<2>(mapping[i]);
	}
	else
	{
		pixels[3 * (width * y + x)] = 0.0f;
		pixels[3 * (width * y + x) + 1] = 0.0f;
		pixels[3 * (width * y + x) + 2] = 0.0f;
	}
}

void calculateMandel(std::vector<float>& pixels, int lowerHeight, int upperHeight)
{
	for (int i = lowerHeight; i < upperHeight; ++i)
	{
		for (int j = 0; j < width; ++j)
		{
			double x0 = ((long double)j / width) * (xUpper - xLower) + xLower;
			double y0 = ((long double)i / height) * (yUpper - yLower) + yLower;
			std::complex<long double> z(0, 0);
			std::complex<long double> c(x0, y0);
			int iter;
			int maxIter = 200;
			for (iter = 0; std::abs(z) < 4 && iter < maxIter; ++iter)
			{
				z = z * z + c;
			}

			float color = 1.0f - (float)iter / maxIter;

			int index = 3 * (i + j);

			palette(pixels, j, i, iter, maxIter);
		}
	}

}

void writeMandel(std::vector<float> &pixels)
{
	std::vector<std::thread> threadVec;

	for (int i = 0; i < processor_count*4; ++i)
	{
		int lowerBound = i * height / (processor_count*4);
		int upperBound = (i + 1) * height / (processor_count*4);
		threadVec.push_back(std::thread(calculateMandel, std::ref(pixels), lowerBound, upperBound));
	}

	for (int i = 0; i < processor_count*4; ++i)
	{
		threadVec[i].join();
	}
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(900, 900, "FraktGen", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	GLuint ebo;
	glGenBuffers(1, &ebo);

	GLuint vertexBuffer;
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	float color[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	float vertices[] = {
		-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // Top-left
		1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // Top-right
		1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // Bottom-right
		-1.0f, -1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f // Bottom-left
	};

	GLuint elements[] = {
		0, 1, 2,
		2, 3, 0 
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

	const char* vertexSource = R"glsl(
		#version 150 core
		in vec2 position;
		in vec3 color;
		in vec2 texcoord;
		out vec3 Color;
		out vec2 Texcoord;
		void main()
		{
			Texcoord=texcoord;
			Color=color;
			gl_Position = vec4(position, 0.0, 1.0);
		}
	)glsl";

	const char* fragmentSource = R"glsl(
		#version 150 core
		in vec3 Color;
		in vec2 Texcoord;
		out vec4 outColor;
		uniform sampler2D tex;
		void main()
		{
			outColor = texture(tex, Texcoord);
		}
	)glsl";
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), 0);
	glEnableVertexAttribArray(posAttrib);

	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(colAttrib);

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));

	std::vector<float> pixels(width*height*3);

	writeMandel(pixels);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, pixels.data());


	while (!glfwWindowShouldClose(window))
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwWaitEvents();
		glfwSetMouseButtonCallback(window, mouse_button_callback);
		if (update)
		{
			writeMandel(pixels);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, pixels.data());
			update = false;
		}
	}
	glfwTerminate();
}
