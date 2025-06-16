#include <common/sprite_batch.h>
#include <common/resource_manager.h>
#include <common/application.h>
#include <common/camera.h>
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
		shader = manager.loadShader("../shaders/spritebatchShader.vert", "../shaders/spritebatchShader.frag", true);
		tex1 = manager.loadTexture("../assets/container.jpg");
		tex2 = manager.loadTexture("../assets/awesomeface.png");

		speed = 200.0f;
		cameraPos = glm::vec2(0.0f, 0.0f);
		direction = glm::vec2(0.0f, 0.0f);
		camera = new Camera2D(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		camera->setPosition(cameraPos);
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

		projection = camera->getProjection();
		view = camera->getView();
	}

	~TestApplication()
	{
		delete camera;
	}

	void handleInput(GLFWwindow* window) override
	{
		direction = glm::vec2(0.0f, 0.0f);

		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			direction.y -= 1.0f;
		}

		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			direction.x -= 1.0f;
		}

		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			direction.y += 1.0f;
		}

		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			direction.x += 1.0f;
		}

		if (glm::length(direction) > 0.0f)
		{
			direction = glm::normalize(direction);
		}
	}

	void update(float deltaTime) override
	{
		cameraPos.x += direction.x * speed * deltaTime;
		cameraPos.y += direction.y * speed * deltaTime;
		camera->setPosition(cameraPos);
	}

	void render(Renderer& renderer) override
	{
		spriteBatch.draw(shader, camera->getProjection(), camera->getView(), tex1->getTextureId());
	}

private:
	ResourceManager manager;
	Shader* shader;
	Texture* tex1;
	Texture* tex2;
	SpriteBatch spriteBatch;

	Camera2D* camera;
	glm::mat4 projection;
	glm::mat4 view;

	glm::vec2 direction;
	float speed;
	glm::vec2 cameraPos;
};

int main()
{
	TestApplication app;
	return app.run();
}