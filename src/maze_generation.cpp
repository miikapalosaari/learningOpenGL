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

#include <stack>

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

float cellSize = 20.0f;
int rows = static_cast<int>(SCREEN_HEIGHT / cellSize);
int cols = static_cast<int>(SCREEN_WIDTH / cellSize);

bool fogOfWar = true;

static int getIndex(int x, int y)
{
	return x + y * cols;
}

enum Visibility
{
	Hidden,
	Visible
};

struct Cell
{
	float x, y;
	float w, h;
	bool visited;
	bool drawn;
	std::array<bool, 4> walls;
	Visibility visibility = Visibility::Hidden;

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

		fillGrid();
		generateMaze();
		player = new Player();
		addRectangle(0.0f, 0.0f, cellSize, cellSize);
	}

	void fillGrid()
	{
		for (int x = 0; x < rows; x++)
		{
			for (int y = 0; y < cols; y++)
			{
				Cell* cell = new Cell(y, x);
				cells.push_back(cell);
				addCell(cell);
			}
		}
	}

	void generateMaze()
	{
		currentCell = cells[0];
		currentCell->visited = true;

		while (true)
		{
			Cell* nextCell = currentCell->checkNeighbors(cells);

			if (nextCell)
			{
				nextCell->visited = true;
				stack.push(currentCell);
				removeWalls(currentCell, nextCell);
				currentCell = nextCell;
			}
			else if (!stack.empty())
			{
				currentCell = stack.top();
				stack.pop();
			}
			else
			{
				break;
			}
		}
		updateGrid();
	}


	void updateGrid()
	{
		clearLines();

		for (Cell* cell : cells)
		{
			addCell(cell);
		}
	}


	void updateLineBufferIfNeeded()
	{
		if (needsBufferUpdate)
		{
			glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
			glBufferData(GL_ARRAY_BUFFER, lineVertices.size() * sizeof(float), lineVertices.data(), GL_DYNAMIC_DRAW);
			needsBufferUpdate = false;
		}
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
		needsBufferUpdate = true;
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

	void clearLines()
	{
		lineVertices.clear();
		updateLineBufferIfNeeded();
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

	~MazeGeneration()
	{
 		delete camera;
		delete shader;

		while (!stack.empty())
		{
			delete stack.top();
			stack.pop();
		}

		for (Cell* cell : cells)
		{
			delete cell;
		}
		cells.clear();

		delete currentCell;
		currentCell = nullptr;
		delete player;
	}

	Cell* getCellAtPlayerPosition(const glm::ivec2& playerPos, const std::vector<Cell*>& cells)
	{
		for (Cell* cell : cells)
		{
			if (cell->x == playerPos.x && cell->y == playerPos.y)
			{
				return cell;
			}
		}
		return nullptr;
	}


	void handleInput(GLFWwindow* window) override
	{
		glm::ivec2 pos = glm::ivec2(static_cast<int>(player->position.x/cellSize), static_cast<int>(player->position.y/cellSize));
		currentCell = getCellAtPlayerPosition(pos, cells);

		if (player->moveCooldown <= 0.0f) 
		{
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
			{
				if(!currentCell->walls[0])
				{
					player->position.y -= static_cast<int>(1 * cellSize);
					player->moveCooldown = player->moveDelay;
				}
			}
			else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
			{
				if(!currentCell->walls[2])
				{
					player->position.y += static_cast<int>(1 * cellSize);
					player->moveCooldown = player->moveDelay;
				}
			}
			else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
			{
				if(!currentCell->walls[3])
				{
					player->position.x -= static_cast<int>(1 * cellSize);
					player->moveCooldown = player->moveDelay;
				}
			}
			else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
			{
				if(!currentCell->walls[1])
				{
					player->position.x += static_cast<int>(1 * cellSize);
					player->moveCooldown = player->moveDelay;
				}
			}
		} 
	}

	void removeWalls(Cell* a, Cell* b)
	{
		int x = a->x - b->x;
		if(x == 1)
		{
			a->walls[3] = false;
			b->walls[1] = false;
		}
		else if(x == -1)
		{
			a->walls[1] = false;
			b->walls[3] = false;
		}

		int y = a->y - b->y;
		if(y == 1)
		{
			a->walls[0] = false;
			b->walls[2] = false;
		}
		else if(y == -1)
		{
			a->walls[2] = false;
			b->walls[0] = false;
		}
	}

	void update(float deltaTime) override
	{
		updateLineBufferIfNeeded();
		if(fogOfWar) updateVisibility(glm::ivec2(static_cast<int>(player->position.x / cellSize),static_cast<int>(player->position.y / cellSize)), 2);

		if (player->moveCooldown > 0.0f) 
		{
        	player->moveCooldown -= deltaTime;
    	}

		if (static_cast<int>(player->position.x / cellSize) == (rows - 1) && static_cast<int>(player->position.y / cellSize) == (cols - 1))
		{
			std::cout << "Maze completed" << std::endl;
			exit(0);
		}
	}

	void drawRect(Cell* cell, const glm::vec3& color)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(cell->x * cellSize, cell->y * cellSize, 0.0f));

		shader->use();
		shader->setMat4("model", model);
		shader->setVec3("color", color);

		glBindVertexArray(rectVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);
		glBindVertexArray(0);
	}

	void updateVisibility(const glm::ivec2& playerPos, int radius = 1)
	{
		for (Cell* cell : cells)
		{
			cell->visibility = Visibility::Hidden;
		}
			
		for (int dx = -radius; dx <= radius; ++dx)
		{
			for (int dy = -radius; dy <= radius; ++dy)
			{
				glm::ivec2 checkPos = playerPos + glm::ivec2(dx, dy);
				for (Cell* cell : cells)
				{
					if (cell->x == checkPos.x && cell->y == checkPos.y)
					{
						cell->visibility = Visibility::Visible;
						break;
					}
				}
			}
		}
	}

	void render(Renderer& renderer) override
	{
		glm::mat4 model = glm::mat4(1.0f);

		shader->use();
		shader->setMat4("view", camera->getView());
		shader->setMat4("projection", camera->getProjection());

		drawRect(currentCell, glm::vec3(0.678f, 0.847f, 0.902f));

		glBindVertexArray(lineVAO);
		shader->setVec3("color", glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::mat4(1.0f);
		shader->setMat4("model", model);
		glDrawArrays(GL_LINES, 0, lineVertices.size() / 3);

		if(fogOfWar)
		{
			for (Cell* cell : cells)
			{		
				if (cell->visibility == Visibility::Hidden)
				{
					drawRect(cell, glm::vec3(0.827f, 0.827f, 0.827f));
				}
			}
		}
	}

private:
	ResourceManager manager;
	Shader* shader;
	unsigned int lineVAO, lineVBO, rectVAO, rectVBO;
	Camera2D* camera;
	std::vector<float> lineVertices;
	std::vector<float> rectVertices;

	float needsBufferUpdate = true;
	std::stack<Cell*> stack;

	struct Player 
	{
		glm::ivec2 position = glm::ivec2(0,0);
		float speed = 5.0f;
		glm::vec2 velocity = glm::vec2(0.0f);

		float moveCooldown = 0.0f;
    	float moveDelay = 0.1f;
	};

	Player* player;
};

int main()
{
	srand(static_cast<unsigned int>(time(nullptr)));
	MazeGeneration app;
	return app.run();
}