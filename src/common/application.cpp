#include <common/application.h>

Application::Application(int w, int h, const std::string& title)
	: renderer(w, h, title)
{
}

Application::~Application()
{
}

int Application::run()
{
	isRunning = true;

	float lastFrameTime = 0.0f;
	float deltaTime = 0.0f;
	float currentFrameTime = 0.0f;

	while (renderer.isWindowOpen() && isRunning)
	{
		currentFrameTime = static_cast<float>(glfwGetTime());
		deltaTime = currentFrameTime - lastFrameTime;
		lastFrameTime = currentFrameTime;
		handleInput(renderer.getWindow());
		update(deltaTime);
		renderer.onBeginFrame();
		render(renderer);
		renderer.onEndFrame();

		glfwPollEvents();
		if (glfwGetKey(renderer.getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			stop();
		}
	}
	return 0;
}

void Application::stop()
{
	isRunning = false;
}

Renderer& Application::getRenderer()
{
	return renderer;
}

