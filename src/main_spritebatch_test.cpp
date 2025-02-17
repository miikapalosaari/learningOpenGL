#include <common/shader.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
GLuint loadTexture(const char* filePath);

float visibility = 0.5f;

const float SCREEN_WIDTH = 800.0f;
const float SCREEN_HEIGHT = 600.0f;

class Plane 
{
public:
	float xPos, yPos, width, height;
	GLuint VAO, VBO, EBO;
	Shader simpleShader;

	Plane(float x, float y, float w, float h)
		: xPos(x), yPos(y), width(w), height(h), simpleShader("../shaders/spritebatchShader.vert", "../shaders/spritebatchShader.frag") {
		init();
	}

	void init() 
	{
		// Define the vertex data for the plane (4 unique vertices)
		vertices = 
		{
			xPos, yPos, 0.0f,					1.0f, 0.0f, 0.0f,	// bottom-left
			xPos + width, yPos, 0.0f,			0.0f, 1.0f, 0.0f,	// bottom-right
			xPos, yPos + height, 0.0f,			0.0f, 0.0f, 1.0f,	// top-left
			xPos + width, yPos + height, 0.0f,	1.0f, 1.0f, 0.0f,	// top-right
		};

		// Define the indices for the two triangles
		indices = { 0, 1, 2, 1, 3, 2 };

		// Initialize VAO, VBO, EBO
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
	
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	std::vector<float> getVertices() const {
		return vertices;
	}

	std::vector<GLuint> getIndices() const {
		return indices;
	}

private:
	std::vector<float> vertices;
	std::vector<GLuint> indices;
};


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
		std::vector<float> vertices = plane.getVertices();
		std::vector<GLuint> indices = plane.getIndices();

		// Offset indices by current number of vertices to avoid collisions
		GLuint vertexOffset = currentVertexCount;
		for (auto& index : indices)
		{
			index += vertexOffset;
		}

		// Add plane data to the batch
		batchedVertices.insert(batchedVertices.end(), vertices.begin(), vertices.end());
		batchedIndices.insert(batchedIndices.end(), indices.begin(), indices.end());

		// Update current counts
		currentVertexCount += vertices.size() / 6;
		currentIndexCount += indices.size();

		std::cout << "Vertex Data Size: " << batchedVertices.size() << std::endl;
		std::cout << "Index Data Size: " << batchedIndices.size() << std::endl;
	}

	void draw(glm::mat4 projection, glm::mat4 view)
	{
		// Update the buffers with the batched data before drawing
		updateBuffers();

		shader.use();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);

		// Bind VAO, VBO, and EBO
		glBindVertexArray(VAO);

		// Draw all planes using a single draw call
		glDrawElements(GL_TRIANGLES, currentIndexCount, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}

private:
	GLuint VAO, VBO, EBO;
	Shader shader;
	std::vector<float> batchedVertices;
	std::vector<GLuint> batchedIndices;
	GLuint currentVertexCount = 0;
	GLuint currentIndexCount = 0;

	void init()
	{
		shader = Shader("../shaders/spritebatchShader.vert", "../shaders/spritebatchShader.frag");

		// Create the VAO, VBO, and EBO
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
	}

	void updateBuffers() 
	{
		// Update the VBO and EBO with batched data
		glBindVertexArray(VAO);

		// Upload batched vertex data
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, batchedVertices.size() * sizeof(float), &batchedVertices[0], GL_STATIC_DRAW);

		// Upload batched index data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, batchedIndices.size() * sizeof(GLuint), &batchedIndices[0], GL_STATIC_DRAW);

		// Set vertex attribute pointers (assuming the vertex format is consistent)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);

		glBindBuffer(GL_ARRAY_BUFFER, 0);  // Unbind VBO
		glBindVertexArray(0);  // Unbind VAO
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

	// Add multiple planes to the batch
	Plane plane1(0.0f, 0.0f, 64.0f, 64.5f);
	Plane plane2(0.0f, 74.0f, 64.0f, 64.0f);
	Plane plane3(0.0f, 144.0f, 64.0f, 64.0f);

	batcher.addPlane(plane1);
	batcher.addPlane(plane2);
	batcher.addPlane(plane3);

	std::cout << "Plane Position: (" << plane1.xPos << ", " << plane1.yPos << ")" << std::endl;
	std::cout << "Plane Position: (" << plane2.xPos << ", " << plane2.yPos << ")" << std::endl;
	std::cout << "Plane Position: (" << plane3.xPos << ", " << plane3.yPos << ")" << std::endl;

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
