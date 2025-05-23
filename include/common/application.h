#pragma once
#include <common/renderer.h>

class Application
{
public:
    Application(int w, int h, const std::string& title);
    virtual ~Application();

    virtual void handleInput(GLFWwindow* window) = 0;
    virtual void update(float deltaTime) = 0;
    virtual void render(Renderer& renderer) = 0;
    int run();
    void stop();
    Renderer& getRenderer();

private:
    Application();
    Application(const Application&);
    Application& operator=(const Application&);

    bool isRunning = false;
    Renderer renderer;

};