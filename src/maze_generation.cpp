#include <common/resource_manager.h>
#include <common/application.h>
#include <common/camera.h>
#include <common/plane.h>
#include <iostream>
#include <array>
#include <cstdlib>
#include <ctime>

#include <thread>
#include <chrono>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float SCREEN_WIDTH = 800.0f;
const float SCREEN_HEIGHT = 800.0f;

float lineVertices[] = 
{
    -0.5f, 0.0f, 0.0f,
     0.5f, 0.0f, 0.0f 
};

float rectangleVertices[] =
{
	0.0f, 0.0f, 0.0f,  // bottom-left
    1.0f, 0.0f, 0.0f,  // bottom-right
    1.0f, 1.0f, 0.0f,  // top-right

    1.0f, 1.0f, 0.0f,  // top-right
    0.0f, 1.0f, 0.0f,  // top-left
    0.0f, 0.0f, 0.0f   // bottom-left
};

float cellSize = 80.0f;
int rows = static_cast<int>(SCREEN_HEIGHT / cellSize);
int cols = static_cast<int>(SCREEN_WIDTH / cellSize);

struct Cell
{
	float x, y;
	float w, h;
	bool visited;
	bool drawn;
	std::array<bool, 4> walls;

	Cell()
	{
		walls = {true, true, true, true};
		visited = false;
		drawn = false;
		x = 0;
		y = 0;
		w = cellSize;
		h = cellSize;
	}

	Cell(float x_, float y_)
	{
		walls = {true, true, true, true};
		visited = false;
		drawn = false;
		x = x_;
		y = y_;
		w = cellSize;
		h = cellSize;
	}

	static int getIndex(int x, int y)
	{
		return x + y * cols;
	}

	Cell* checkNeighbors(std::vector<Cell*> cells)
	{
		std::vector<Cell*> neighbors;
		if (y - 1 >= 0) 
		{
			Cell* top = cells[getIndex(x, y - 1)];
			if (!top->visited) neighbors.push_back(top);
		}

		if (x + 1 < cols)
		{
			Cell* right = cells[getIndex(x + 1, y)];
			if(!right->visited) neighbors.push_back(right);
		}
		
		if(y + 1 < rows)
		{
			Cell* bottom = cells[getIndex(x, y + 1)];
			if(!bottom->visited) neighbors.push_back(bottom);
		}
		
		if(x - 1 >= 0)
		{
			Cell* left = cells[getIndex(x - 1, y)];
			if(!left->visited) neighbors.push_back(left);
		}

		srand(static_cast<unsigned int>(time(nullptr)));
		if(neighbors.size() > 0)
		{
			int random = rand() % neighbors.size();
			return neighbors[random];
		}
		return nullptr;
	}
};

std::vector<Cell*> cells = {};
Cell* currentCell;

class MazeGeneration : public Application
{
public:
	MazeGeneration() 
		: Application(SCREEN_WIDTH, SCREEN_HEIGHT, "Maze")
	{
		glDisable(GL_DEPTH_TEST);
		camera = new Camera2D(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		lineVertices = {};
		rectVertices = {};

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

		glGenVertexArrays(1, &lineVAO);
		glGenBuffers(1, &lineVBO);
		
		glBindVertexArray(lineVAO);
		glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(0);

		glGenVertexArrays(1, &rectVAO);
		glGenBuffers(1, &rectVBO);

		glBindVertexArray(rectVAO);
		glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
		glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		for (int x = 0; x < rows; x++)
		{
			for (int y = 0; y < cols; y++)
			{
				Cell* cell = new Cell(y, x);
				cells.push_back(cell);
				addCell(cell);
			}
		}
		currentCell = cells[0];
	}

	void updateLineBuffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
		glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(float), lineVertices.data(), GL_DYNAMIC_DRAW);
	}

	void updateRectBuffer()
	{
		glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
		glBufferData(GL_ARRAY_BUFFER, rectVertices.size() * sizeof(float), rectVertices.data(), GL_DYNAMIC_DRAW);
	}

	void addLine(glm::vec3 start, glm::vec3 end)
	{
		lineVertices.push_back(start.x);
		lineVertices.push_back(start.y);
		lineVertices.push_back(start.z);

		lineVertices.push_back(end.x);
		lineVertices.push_back(end.y);
		lineVertices.push_back(end.z);

		updateLineBuffer();
	}

	void clearLines()
	{
		lineVertices.clear();
		updateLineBuffer();
	}

	void checkNeighbors(Cell* cell)
	{

	}

	void addRectangle(float x, float y, float w, float h)
	{
		rectVertices.push_back(x);   	 rectVertices.push_back(y);   	  rectVertices.push_back(0.0f); // bottom-left
		rectVertices.push_back(x + w);   rectVertices.push_back(y);       rectVertices.push_back(0.0f); // bottom-right
		rectVertices.push_back(x + w);   rectVertices.push_back(y + h);   rectVertices.push_back(0.0f); // top-right

		rectVertices.push_back(x + w);   rectVertices.push_back(y + h);   rectVertices.push_back(0.0f); // top-right
		rectVertices.push_back(x);       rectVertices.push_back(y + h);   rectVertices.push_back(0.0f); // top-left
		rectVertices.push_back(x);       rectVertices.push_back(y);       rectVertices.push_back(0.0f); // bottom-left

		updateRectBuffer();
	}


	void addCell(Cell* cell)
	{
    	float x = cell->x * cellSize;
		float y = cell->y * cellSize;

		if (cell->walls[0]) addLine(glm::vec3(x, y, 0.0f), glm::vec3(x + cellSize, y, 0.0f));
		if (cell->walls[1]) addLine(glm::vec3(x + cellSize, y, 0.0f), glm::vec3(x + cellSize, y + cellSize, 0.0f));
		if (cell->walls[2]) addLine(glm::vec3(x + cellSize, y + cellSize, 0.0f), glm::vec3(x, y + cellSize, 0.0f));
		if (cell->walls[3]) addLine(glm::vec3(x, y + cellSize, 0.0f), glm::vec3(x, y, 0.0f));
	}

	~MazeGeneration()
	{

	}

	void handleInput(GLFWwindow* window) override
	{

	}

	void update(float deltaTime) override
	{
		float targetFPS = 10.0;
		float targetFrametime = 1.0f / targetFPS;

		float frameTimeRemaining = targetFrametime - deltaTime;
    	if (frameTimeRemaining > 0) 
		{
        	std::this_thread::sleep_for(std::chrono::duration<float>(frameTimeRemaining));
    	}

		currentCell->visited = true;
		Cell* nextCell = currentCell->checkNeighbors(cells);

		if (nextCell)
		{
			nextCell->visited = true;
			currentCell = nextCell;
		}

		for (int i = 0; i < cells.size(); i++)
		{
			if (cells[i]->visited && !cells[i]->drawn)
			{
				addRectangle((cells[i]->x * cells[i]->w) + 1.0f, (cells[i]->y * cells[i]->h) + 1.0f, cells[i]->w - 2.0f, cells[i]->h - 2.0f);
				cells[i]->drawn = true;
			}
		}

	}

	void render(Renderer& renderer) override
	{
		glm::mat4 model = glm::mat4(1.0f);

		shader->use();
		shader->setMat4("model", model);
		shader->setMat4("view", camera->getView());
		shader->setMat4("projection", camera->getProjection());

		glBindVertexArray(lineVAO);
		shader->setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
		glDrawArrays(GL_LINES, 0, lineVertices.size() / 3);

		glBindVertexArray(rectVAO);
		shader->setVec3("color", glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::mat4(1.0f);
		shader->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, rectVertices.size() / 3);
	}

private:
	ResourceManager manager;
	Shader* shader;
	unsigned int lineVAO, lineVBO, rectVAO, rectVBO;
	Camera2D* camera;
	std::vector<float> lineVertices;
	std::vector<float> rectVertices;
};

int main()
{
	MazeGeneration app;
	return app.run();
}