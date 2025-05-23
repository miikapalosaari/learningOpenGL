#pragma once
#include <string>
#include <glad/gl.h>
#include <GLFW/glfw3.h>

class Renderer
{
public:
	Renderer(int w, int h, const std::string& title);
	~Renderer();
	GLFWwindow* getWindow();
	bool isWindowOpen() const;
	void onBeginFrame();
	void onEndFrame();
private:
	GLFWwindow* window;
	float width;
	float height;

	Renderer();
	Renderer(const Renderer&);
	Renderer& operator=(const Renderer&);
};