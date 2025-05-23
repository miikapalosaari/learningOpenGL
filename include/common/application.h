#pragma once
#include <common/renderer.h>

class Application
{
public:
    Application(int w, int h, const std::string& title);
    ~Application();

    virtual void update(float deltaTime) {}
    virtual void render(Renderer& renderer) {}
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