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

struct Cell
{
	float x, y;
	float w, h;

	bool walls[4] = {true, true, true, true};

	Cell(float x_, float y_)
	{
		x = x_;
		y = y_;
	}
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
		cells = {};

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

		cellSize = 32.0f;
		cols = static_cast<int>(SCREEN_WIDTH / cellSize);
		rows = static_cast<int>(SCREEN_HEIGHT / cellSize); 

		for (int x = 0; x < rows; x++)
		{
			for (int y = 0; y < cols; y++)
			{
				Cell cell = Cell(y, x);
				cell.walls[0] = false;
				cell.walls[2] = false;
				cells.push_back(cell);
				addCell(cell);
			}
		}
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

	void addCell(Cell& cell)
	{
    	float x = cell.x * cellSize;
		float y = cell.y * cellSize;

		if (cell.walls[0]) addLine(glm::vec3(x, y, 0.0f), glm::vec3(x + cellSize, y, 0.0f));
		if (cell.walls[1]) addLine(glm::vec3(x + cellSize, y, 0.0f), glm::vec3(x + cellSize, y + cellSize, 0.0f));
		if (cell.walls[2]) addLine(glm::vec3(x + cellSize, y + cellSize, 0.0f), glm::vec3(x, y + cellSize, 0.0f));
		if (cell.walls[3]) addLine(glm::vec3(x, y + cellSize, 0.0f), glm::vec3(x, y, 0.0f));
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

	int rows, cols;
	float cellSize;
	std::vector<Cell> cells;
};

int main()
{
	MazeGeneration app;
	return app.run();
}