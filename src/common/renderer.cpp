#include <common/renderer.h>
#include <iostream>

Renderer::Renderer(int w, int h, const std::string& title)
{
	if (!glfwInit())
	{
		std::cout << "GLFW initialization failed" << std::endl;
	}
	window = glfwCreateWindow(w, h, title.c_str(), nullptr, nullptr);
	width = w;
	height = h;

	if (!window)
	{
		std::cout << "GLFW Window creation failed" << std::endl;
		glfwTerminate();
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL(glfwGetProcAddress)) 
	{
		std::cout << "Failed to initialize GLAD";
		glfwDestroyWindow(window);
		glfwTerminate();
	}
	glfwSwapInterval(0);
}

Renderer::~Renderer()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

GLFWwindow* Renderer::getWindow()
{
	return window;
}

bool Renderer::isWindowOpen() const
{
	return !glfwWindowShouldClose(window);
}

void Renderer::onBeginFrame()
{
	glViewport(0, 0, width, height);
	glClearColor(0, 0, 1, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::onEndFrame()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}
