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
	glfwSwapInterval(1);

	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	glEnable(GL_DEPTH_TEST);
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
	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::onEndFrame()
{
	glfwSwapBuffers(window);
	glfwPollEvents();
}

void Renderer::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
	if (renderer)
	{
		renderer->width = width;
		renderer->height = height;
		glViewport(0, 0, width, height);
	}
}
