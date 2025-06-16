#include <common/resource_manager.h>
#include <common/application.h>
#include <common/camera.h>
#include <common/plane.h>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float SCREEN_WIDTH = 800.0f;
const float SCREEN_HEIGHT = 640.0f;

float lineVertices[] = 
{
    -0.5f, 0.0f, 0.0f,
     0.5f, 0.0f, 0.0f 
};

class MazeGeneration : public Application
{
public:
	MazeGeneration() 
		: Application(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze")
	{
		glDisable(GL_DEPTH_TEST);
		camera = new Camera2D(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		vertices = {};

		const char* vertex = 
		"#version 330 core\n"
		"layout (location = 0) in vec3 aPosition;\n"

		"uniform mat4 model;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"

		"void main()\n"
		"{\n"
		"   gl_Position = projection * view * model * vec4(aPosition, 1.0);\n"
		"}";

		const char* fragment = 
		"#version 330 core\n"
		"out vec4 FragColor;\n"

		"uniform vec3 color;\n"

		"void main()\n"
		"{\n"
		"   FragColor = vec4(color, 1.0);\n"
		"}";

		shader = manager.loadShader(vertex, fragment, false);

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

		glBindVertexArray(VAO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);


		addLine(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(32.0f, 0.0f, 0.0f));
		addLine(glm::vec3(32.0f, 0.0f, 0.0f), glm::vec3(32.0f, 32.0f, 0.0f));
		addLine(glm::vec3(32.0f, 32.0f, 0.0f), glm::vec3(0.0f, 32.0f, 0.0f));
		addLine(glm::vec3(0.0f, 32.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	}

	void updateBuffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_DYNAMIC_DRAW);
	}

	void addLine(glm::vec3 start, glm::vec3 end)
	{
		vertices.push_back(start.x);
		vertices.push_back(start.y);
		vertices.push_back(start.z);

		vertices.push_back(end.x);
		vertices.push_back(end.y);
		vertices.push_back(end.z);

		updateBuffer();
	}

	~MazeGeneration()
	{

	}

	void handleInput(GLFWwindow* window) override
	{

	}

	void update(float deltaTime) override
	{

	}

	void render(Renderer& renderer) override
	{
		glm::mat4 model = glm::mat4(1.0f);

		shader->use();
		shader->setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));

		shader->setMat4("model", model);
		shader->setMat4("view", camera->getView());
		shader->setMat4("projection", camera->getProjection());

		glBindVertexArray(VAO);
		glDrawArrays(GL_LINES, 0, vertices.size() / 3);
	}

private:
	ResourceManager manager;
	Shader* shader;
	unsigned int VAO, VBO;
	Camera2D* camera;
	std::vector<float> vertices;
};

int main()
{
	MazeGeneration app;
	return app.run();
}