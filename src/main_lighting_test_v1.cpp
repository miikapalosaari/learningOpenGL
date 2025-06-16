#include <common/resource_manager.h>
#include <common/application.h>
#include <common/camera.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random>
#include <iostream>
#include <ctime>

const float SCREEN_WIDTH = 800.0f;
const float SCREEN_HEIGHT = 600.0f;

float vertices[] =
{
	//Positions			  Normals
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
};

class LightingDemo : public Application
{
public:
	LightingDemo()
		: Application(SCREEN_WIDTH, SCREEN_HEIGHT, "Lighting demo")
	{
		lightingShader = manager.loadShader("../shaders/simpleLighting.vert", "../shaders/simpleLighting.frag", true);
		lightShader = manager.loadShader("../shaders/simpleLight.vert", "../shaders/simpleLight.frag", true);

		tex1 = manager.loadTexture("../assets/container.jpg");
		tex2 = manager.loadTexture("../assets/awesomeface.png");

		cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
		cameraDir = glm::vec3(0.0f, 0.0f, 0.0f);
		camera = new Camera3D(45.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 100.0f);
		camera->setPosition(glm::vec3(0.0f, 0.0f, 50.0f));

		glGenVertexArrays(1, &CubeVAO);
		glGenBuffers(1, &VBO);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindVertexArray(CubeVAO);
		
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// normal attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glGenVertexArrays(1, &LightVAO);
		glBindVertexArray(LightVAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glfwSetInputMode(getRenderer().getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		lightPos = glm::vec3(3.0f, 3.0f, 3.0f);
		lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

		srand(static_cast<unsigned int>(std::time(nullptr)));
	}

	~LightingDemo()
	{
		delete camera;

		glDeleteVertexArrays(1, &CubeVAO);
		glDeleteVertexArrays(1, &LightVAO);
		glDeleteBuffers(1, &VBO);
	}

	void handleInput(GLFWwindow* window) override
	{
		cameraDir = glm::vec3(0.0f, 0.0f, 0.0f);
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			cameraDir += camera->getFront();
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			cameraDir -= camera->getRight();
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			cameraDir -= camera->getFront();
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			cameraDir += camera->getRight();
		}

		if (glm::length(cameraDir) > 0.0f)
		{
			cameraDir = glm::normalize(cameraDir);
		}
		
		glfwGetCursorPos(window, &mouseX, &mouseY);
		camera->processMouseMovement(mouseX, mouseY, 0.05);
	}

	glm::vec3 getRandomColor() 
	{
		float r = static_cast<float>(rand()) / RAND_MAX;
		float g = static_cast<float>(rand()) / RAND_MAX;
		float b = static_cast<float>(rand()) / RAND_MAX;

		return glm::vec3(r, g, b);
	}

	void update(float deltaTime) override
	{
		glm::vec3 newPos = camera->getPosition() + cameraDir * camSpeed * deltaTime;
		camera->setPosition(newPos);

		lightColor.x = sin(glfwGetTime() * 2.0f);
		lightColor.y = sin(glfwGetTime() * 0.7f);
		lightColor.z = sin(glfwGetTime() * 1.3f);

		diffuseColor = lightColor * glm::vec3(0.5f);
		ambientColor = diffuseColor * glm::vec3(0.2f);

		rotationAngle += 100 * deltaTime;
	}

	void render(Renderer& renderer) override
	{
		lightingShader->use();
		lightingShader->setVec3("light.position", lightPos);
		lightingShader->setVec3("light.ambient", glm::vec3(1.0f));
		lightingShader->setVec3("light.diffuse", glm::vec3(1.0f));
		lightingShader->setVec3("light.specular", glm::vec3(1.0f));

		lightingShader->setVec3("material.ambient", ambientColor);
		lightingShader->setVec3("material.diffuse", diffuseColor);
		//lightingShader->setVec3("material.ambient", glm::vec3(0.24725f, 0.1995f, 0.0745f));
		//lightingShader->setVec3("material.diffuse", glm::vec3(0.75164f, 0.60648f, 0.22648f));
		lightingShader->setVec3("material.specular", glm::vec3(0.628281f, 0.555802f, 0.366065f));
		lightingShader->setFloat("material.shininess", 32.0f);

		lightingShader->setVec3("viewPos", camera->getPosition());

		glm::mat4 model = glm::mat4(1.0f);
		lightingShader->setMat4("model", model);
		lightingShader->setMat4("view", camera->getView());
		lightingShader->setMat4("projection", camera->getProjection());

		glBindVertexArray(CubeVAO);

		int gridSize = 9;
		float spacing = 1.0;
		float cubeScale = 0.5f;

		glm::vec3 pivot = glm::vec3((gridSize - 1) * spacing * 0.5f, (gridSize - 1) * spacing * 0.5f, 0.0f);
		glm::vec3 rotationAxis = glm::vec3(0.0f, 0.0f, 1.0f);
		//glm::vec3 rotationAxis = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
		for (int x = 0; x < gridSize; x++)
		{
			for (int y = 0; y < gridSize; y++)
			{
				for (int z = 0; z < gridSize; z++)
				{
					glm::vec3 startPos = glm::vec3(x * spacing, y * spacing, z * spacing);

					model = glm::mat4(1.0f);
					model = glm::translate(model, pivot);
					model = glm::rotate(model, glm::radians(rotationAngle), rotationAxis);
					model = glm::translate(model, startPos - pivot);
					model = glm::scale(model, glm::vec3(cubeScale));
					lightingShader->setMat4("model", model);
					glDrawArrays(GL_TRIANGLES, 0, 36);
				}
			}
		}
		

		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));

		lightShader->use();
		lightShader->setMat4("model", lightModel);
		lightShader->setMat4("view", camera->getView());
		lightShader->setMat4("projection", camera->getProjection());

		glBindVertexArray(LightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

private:
	Camera3D* camera;
	glm::vec3 cameraPos;
	glm::vec3 cameraDir;
	ResourceManager manager;
	Shader* lightingShader;
	Shader* lightShader;
	Texture* tex1;
	Texture* tex2;

	double mouseX = 0.0f;
	double mouseY = 0.0f;
	unsigned int VBO;
	unsigned int CubeVAO;
	unsigned int LightVAO;

	float camSpeed = 10.0f;
	glm::vec3 lightPos;
	glm::vec3 lightColor;
	float rotationAngle = 0.0f;
	glm::vec3 diffuseColor = glm::vec3(1.0f);
	glm::vec3 ambientColor = glm::vec3(1.0f);
};

int main()
{
	LightingDemo app;
	return app.run();
}