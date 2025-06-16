#include <common/resource_manager.h>
#include <common/application.h>
#include <common/camera.h>
#include <common/plane.h>
#include <iostream>
#include <random>
#include <ctime>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float SCREEN_WIDTH = 512.0f;
const float SCREEN_HEIGHT = 512.0f;

struct Snake
{
	glm::vec2 position;
	glm::vec2 direction;
};

class TestApplication : public Application
{
public:
	TestApplication() 
		: Application(SCREEN_WIDTH, SCREEN_HEIGHT, "test application")
	{
		glDisable(GL_DEPTH_TEST);
		std::srand(static_cast<unsigned int>(std::time(nullptr)));
		shader = manager.loadShader("../shaders/snakeShader.vert", "../shaders/snakeShader.frag", true);
		camera = new Camera2D(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

		snake.position.x = 0.0f;
		snake.position.y = 0.0f;
		snake.direction.x = 1.0f;
		snake.direction.y = 0.0f;

		snakeHead = new Plane(0.0f, 0.0f, 32.0f, 32.0f);
		food = new Plane(0.0f, 0.0f, 32.0f, 32.0f);
		spawnfood();
		nextDirection = snake.direction;
	}

	~TestApplication()
	{
		delete camera;
		delete snakeHead;
		delete food;
	}

	void spawnfood()
	{
		float x = rand() % 16;
		float y = rand() % 16;

		foodX = x * 32.0f;
		foodY = y * 32.0f;
	}

	void eatFood()
	{
		if (snake.position.x == foodX && snake.position.y == foodY)
		{
			spawnfood();
			score++;

			if (tailLength < 255)
			{
				if (tailLength == 0)
				{
					tailPositions[tailLength] = snake.position;
				}
				else
				{
					tailPositions[tailLength] = tailPositions[tailLength - 1];
				}
				tailLength++;
			}
		}
	}

	void checkForOutOfBounds()
	{
		if (targetPos.x < 0 || targetPos.x >= SCREEN_WIDTH || targetPos.y < 0 || targetPos.y >= SCREEN_HEIGHT)
		{
			std::cout << "snake out of bounds " << std::endl;
			std::cout << "total score: " << score << std::endl;
			stop();
		}
	}

	void checkIfSnakeHitItself()
	{
		for (int i = 0; i < tailLength; i++)
		{
			if (snake.position == tailPositions[i])
			{
				std::cout << "snake hit itself " << std::endl;
				std::cout << "total score: " << score << std::endl;
				stop();
			}
		}
	}

	void handleInput(GLFWwindow* window) override
	{
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && snake.direction != glm::vec2(0, 1))
		{
			nextDirection = glm::vec2(0, -1);
		}

		else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && snake.direction != glm::vec2(0, -1))
		{
			nextDirection = glm::vec2(0, 1);
		}

		else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && snake.direction != glm::vec2(1, 0))
		{
			nextDirection = glm::vec2(-1, 0);
		}

		else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && snake.direction != glm::vec2(-1, 0))
		{
			nextDirection = glm::vec2(1, 0);
		}
	}

	void update(float deltaTime) override
	{
		timer += deltaTime;

		if (!isMoving && timer >= moveDelay)
		{
			timer -= moveDelay;
			isMoving = true;
			moveProgress = 0.0f;

			snake.direction = nextDirection;

			prevPos = snake.position;
			targetPos = snake.position + snake.direction * 32.0f;

			checkForOutOfBounds();
			checkIfSnakeHitItself();
			eatFood();

			for (int i = tailLength; i > 0; --i)
			{
				tailPositions[i] = tailPositions[i - 1];
			}
			tailPositions[0] = prevPos;
		}

		if (isMoving)
		{
			moveProgress += deltaTime / moveDelay;

			if (moveProgress >= 1.0f)
			{
				moveProgress = 1.0f;
				isMoving = false;
				snake.position = targetPos;	
			}
			else
			{
				snake.position = glm::mix(prevPos, targetPos, moveProgress);
			}
		}
	}

	void render(Renderer& renderer) override
	{
		shader->use();
		shader->setMat4("projection", camera->getProjection());
		shader->setMat4("view", camera->getView());

		shader->setVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
		for (int i = 0; i < tailLength; i++)
		{
			glm::mat4 tailModel = glm::mat4(1.0f);
			tailModel = glm::translate(tailModel, glm::vec3(tailPositions[i], 0.0f));
			shader->setMat4("model", tailModel);
			snakeHead->draw();
		}

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(foodX, foodY, 0.0f));
		shader->setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
		shader->setMat4("model", model);
		food->draw();

		
		shader->setVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(snake.position, 0.0f));
		shader->setMat4("model", model);
		snakeHead->draw();
	}

private:
	ResourceManager manager;
	Shader* shader;

	unsigned int VAO;

	glm::vec2 tailPositions[255] = {};

	float moveDelay = 0.1f;
	float timer = 0.0f;

	Snake snake;
	Plane* snakeHead;
	Plane* food;
	float foodX;
	float foodY;
	int score = 0;
	int tailLength = 0;
	float gameOverTimer;
	glm::vec2 nextDirection;
	glm::vec2 prevPos;
	glm::vec2 targetPos;
	bool isMoving = false;
	float moveProgress;

	Camera2D* camera;
};

int main()
{
	TestApplication app;
	return app.run();
}