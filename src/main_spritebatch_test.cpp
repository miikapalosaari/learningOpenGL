#include <common/sprite_batch.h>
#include <common/resource_manager.h>
#include <common/application.h>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


const float SCREEN_WIDTH = 800.0f;
const float SCREEN_HEIGHT = 600.0f;


class TestApplication : public Application
{
public:
	TestApplication() 
		: Application(SCREEN_WIDTH, SCREEN_HEIGHT, "test application")
	{
		shader = manager.loadShader("../shaders/spritebatchShader.vert", "../shaders/spritebatchShader.frag");
		tex1 = manager.loadTexture("../assets/container.jpg");
		tex2 = manager.loadTexture("../assets/awesomeface.png");
		glm::vec2 startPos(0, 0);
		for (int row = 0; row < 20; ++row)
		{
			for (int col = 0; col < 20; ++col)
			{
				float x = startPos.x + col * (16 + 2);
				float y = startPos.y + row * (16 + 2);
			
				Plane plane(x, y, 32, 32);
				spriteBatch.addPlane(plane);
			}
		}

		projection = glm::ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);
		view = glm::mat4(1.0f);
	}

	void handleInput(GLFWwindow* window) override
	{
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			std::cout << "pressed Q" << std::endl;
		}
	}

	void update(float deltaTime) override
	{

	}

	void render(Renderer& renderer) override
	{
		spriteBatch.draw(shader, projection, view, tex1->getTextureId());
	}

private:
	ResourceManager manager;
	Shader* shader;
	Texture* tex1;
	Texture* tex2;
	SpriteBatch spriteBatch;

	glm::mat4 projection;
	glm::mat4 view;
};

int main()
{
	TestApplication app;
	return app.run();
}