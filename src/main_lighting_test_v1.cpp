#include <common/resource_manager.h>
#include <common/application.h>
#include <common/camera.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
		: Application(SCREEN_WIDTH, SCREEN_HEIGHT, "3D camera movement demo")
	{
		lightingShader = manager.loadShader("../shaders/simpleLighting.vert", "../shaders/simpleLighting.frag");
		lightShader = manager.loadShader("../shaders/simpleLight.vert", "../shaders/simpleLight.frag");

		tex1 = manager.loadTexture("../assets/container.jpg");
		tex2 = manager.loadTexture("../assets/awesomeface.png");

		cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
		cameraDir = glm::vec3(0.0f, 0.0f, 0.0f);
		camera = new Camera3D(45.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 100.0f);

		glGenVertexArrays(1, &CubeVAO);
		glGenBuffers(1, &VBO);
		glBindVertexArray(CubeVAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
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

		lightPos = glm::vec3(1.2f, 1.0f, 2.0f);
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

	void update(float deltaTime) override
	{
		glm::vec3 newPos = camera->getPosition() + cameraDir * camSpeed * deltaTime;
		camera->setPosition(newPos);

		lightPos.x = 1.0f + sin(glfwGetTime()) * 2.0f;
		lightPos.z = cos(glfwGetTime()) * 2.0f;
	}

	void render(Renderer& renderer)
	{
		lightingShader->use();
		lightingShader->setVec3("lightPos", lightPos);
		lightingShader->setVec3("lightColor", glm::vec3(1.0f));
		lightingShader->setVec3("objectColor", glm::vec3(1.0f, 0.5f, 0.31f));

		glm::mat4 model = glm::mat4(1.0f);
		lightingShader->setMat4("model", model);
		lightingShader->setMat4("view", camera->getView());
		lightingShader->setMat4("projection", camera->getProjection());

		glBindVertexArray(CubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		lightShader->use();
		glm::mat4 lightModel = glm::mat4(1.0f);
		lightModel = glm::translate(lightModel, lightPos);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));
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

	float camSpeed = 2.0f;
	float visibilityMultiplier = 0.5f;
	glm::vec3 lightPos;
};

int main()
{
	LightingDemo app;
	return app.run();
}