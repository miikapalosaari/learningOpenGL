#include <common/shader.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <common/plane.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
GLuint loadTexture(const char* filePath);

float visibility = 0.5f;

const float SCREEN_WIDTH = 800.0f;
const float SCREEN_HEIGHT = 600.0f;

class Batcher
{
public:
	Batcher()
	{
		init();
	}

	void addPlane(const Plane& plane)
	{
		// Collect the plane data and add it to the batch
		std::vector<Vertex> vertices = plane.getVertices();
		std::vector<GLuint> indices = plane.getIndices();
		glm::mat4 model = plane.getModelMatrix();

		// Offset indices by current number of vertices to avoid collisions
		GLuint vertexOffset = currentVertexCount;
		for (auto& index : indices)
		{
			index += vertexOffset;
		}

		std::vector<Vertex> transformedVertices;
		transformedVertices.reserve(vertices.size());

		for (const Vertex& vertex : vertices)
		{
			glm::vec4 transformedPos = model * glm::vec4(vertex.position, 1.0f);
			transformedVertices.push_back({ glm::vec3(transformedPos), vertex.texCoord });
		}

		// Add plane data to the batch
		batchedVertices.insert(batchedVertices.end(), transformedVertices.begin(), transformedVertices.end());
		batchedIndices.insert(batchedIndices.end(), indices.begin(), indices.end());

		// Update current counts
		currentVertexCount += static_cast<GLuint>(vertices.size());
		currentIndexCount += indices.size();
	}

	void draw(glm::mat4 projection, glm::mat4 view)
	{
		// Update the buffers with the batched data before drawing
		updateBuffers();

		shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		shader.setInt("texture0", 0);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, 1);

		// Bind VAO, VBO, and EBO
		glBindVertexArray(VAO);

		// Draw all planes using a single draw call
		glDrawElements(GL_TRIANGLES, currentIndexCount, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}

private:
	GLuint VAO, VBO, EBO;
	Shader shader;
	std::vector<Vertex> batchedVertices;
	std::vector<GLuint> batchedIndices;
	GLuint currentVertexCount = 0;
	GLuint currentIndexCount = 0;

	void init()
	{
		shader = Shader("../shaders/spritebatchShader.vert", "../shaders/spritebatchShader.frag");

		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));
		glEnableVertexAttribArray(1);

		glBindVertexArray(0);
	}

	void updateBuffers() 
	{
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, batchedVertices.size() * sizeof(Vertex), batchedVertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, batchedIndices.size() * sizeof(GLuint), batchedIndices.data(), GL_STATIC_DRAW);

		glBindVertexArray(0);
	}
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// creating a window using glfw
	GLFWwindow* window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "LearningOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSwapInterval(0);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	/******************************************LOAD TEXTURES******************************************/

	GLuint texture1 = loadTexture("../assets/container.jpg");
	GLuint texture2 = loadTexture("../assets/awesomeface.png");

	/******************************************LOAD TEXTURES******************************************/

	Batcher batcher;


	float planeW = 100.0f;
	float planeH = 100.0f;
	float offset = 0.0f;
	int rows = 20;
	int cols = 20;
	glm::vec2 startPos = glm::vec2(0.0f, 0.0f);

	for (int row = 0; row < rows; ++row)
	{
		for (int col = 0; col < cols; ++col)
		{
			float x = startPos.x + col * (planeW + offset);
			float y = startPos.y + row * (planeH + offset);

			Plane plane(x, y, planeW, planeH);
			plane.setSize(glm::vec2(64, 64));
			plane.setScale(glm::vec2(1,1));
			batcher.addPlane(plane);
		}
	}

	

	// Set up the projection matrix (orthographic)
	glm::mat4 projection = glm::ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::mat4(1.0f);  // Identity matrix for simplicity



	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		//simpleShader.use();
		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		projection = glm::ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);

		batcher.draw(projection, view);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);	

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
	{
		visibility += 0.001;
		if (visibility >= 1.0f)
		{
			visibility = 1.0f;
		}
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
	{
		visibility -= 0.001;
		if (visibility <= 0.0f)
		{
			visibility = 0.0f;
		}
	}
}

GLuint loadTexture(const char* filePath)
{
	unsigned int texture;
	int width, height, nrChannels;
	unsigned char* data = {};

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load and generate texture
	stbi_set_flip_vertically_on_load(true);
	data = stbi_load(filePath, &width, &height, &nrChannels, 0);
	if (data)
	{
		GLenum format;
		if (nrChannels == 4) format = GL_RGBA;
		else format = GL_RGB;

		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	return texture;
}
