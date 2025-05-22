#include <common/shader.h>
#include <common/sprite_batch.h>
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

	Shader* simpleShader = new Shader("../shaders/spritebatchShader.vert", "../shaders/spritebatchShader.frag");
	SpriteBatch spriteBatch1 = SpriteBatch();


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

			Plane plane1(x, y, planeW, planeH);
			plane1.setSize(glm::vec2(64, 64));
			plane1.setScale(glm::vec2(1,1));
			spriteBatch1.addPlane(plane1);
		}
	}

	

	// Set up the projection matrix (orthographic)
	glm::mat4 projection = glm::ortho(0.0f, SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::mat4(1.0f);  // Identity matrix for simplicity



	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		spriteBatch1.draw(simpleShader, projection, view, texture1);

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
