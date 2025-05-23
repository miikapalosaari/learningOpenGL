#include <common/resource_manager.h>
#include <common/application.h>
#include <common/camera.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float SCREEN_WIDTH = 800.0f;
const float SCREEN_HEIGHT = 600.0f;

float vertices[] =
{
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

glm::vec3 cubePositions[] =
{
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

class Demo3dApp : public Application
{
public:
	Demo3dApp() 
		: Application(SCREEN_WIDTH, SCREEN_HEIGHT, "3D camera movement demo")
	{
		shader = manager.loadShader("../shaders/simpleShader2.vert", "../shaders/simpleShader2.frag");
		tex1 = manager.loadTexture("../assets/container.jpg");
		tex2 = manager.loadTexture("../assets/awesomeface.png");
		cameraPos = glm::vec3(0.0f, 0.0f, 0.0f);
		cameraDir = glm::vec3(0.0f, 0.0f, 0.0f);
		camera = new Camera3D(45.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.1f, 100.0f);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glBindVertexArray(VAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glfwSetInputMode(getRenderer().getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	~Demo3dApp()
	{
		delete camera;
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
	}

	void render(Renderer& renderer)
	{
		shader->use();
		
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, tex1->getTextureId());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, tex2->getTextureId());
		shader->setInt("myTexture1", 0);
		shader->setInt("myTexture2", 1);

		glBindVertexArray(VAO);
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model = glm::mat4(1.0f);
			float angle = 20.0f * i;
			model = glm::translate(model, cubePositions[i]);
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			shader->setMat4("model", model);
			shader->setMat4("view", camera->getView());
			shader->setMat4("projection", camera->getProjection());
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}

private:
	Camera3D* camera;
	glm::vec3 cameraPos;
	glm::vec3 cameraDir;
	ResourceManager manager;
	Shader* shader;
	Texture* tex1;
	Texture* tex2;

	double mouseX = 0.0f;
	double mouseY = 0.0f;
	unsigned int VBO, VAO, EBO;

	float camSpeed = 2.0f;
	float visibilityMultiplier = 0.5f;
};

int main()
{
	Demo3dApp app;
	return app.run();
}