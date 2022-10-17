#include "FraktGraphics.h"
#include <functional>

FraktGraphics::FraktGraphics(int width, int height) : fGen(&windowConf)
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	window = glfwCreateWindow(width, height, "FraktGen", nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);
	glewExperimental = GL_TRUE;
	glewInit();

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &ebo);

	glGenBuffers(1, &vba);
	glBindBuffer(GL_ARRAY_BUFFER, vba);

	pixels = std::vector<float>(width * height * 3);

	windowConf.height = height;
	windowConf.width = width;

	if (windowConf.boxZoom)
	{
		boxHandler = BoxZoom();
	}
}

void FraktGraphics::init()
{
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

}

void FraktGraphics::run(int maxIter)
{
	fGen.writeMandel(pixels, maxIter);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowConf.width, windowConf.height, 0, GL_RGB, GL_FLOAT, pixels.data());


	while (!glfwWindowShouldClose(window))
	{
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glfwSwapBuffers(window);
		glfwWaitEvents();
		if (windowConf.boxZoom)
		{
			if (boxHandler.getInput(window, windowConf))
			{
				fGen.writeMandel(pixels, maxIter);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowConf.width, windowConf.height, 0, GL_RGB, GL_FLOAT, pixels.data());
			}
		}
	}
	glfwTerminate();

}
