#include <common/shader.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

float visibility = 0.5f;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// creating a window using glfw
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearningOpenGL", NULL, NULL);
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


	/******************************************BUILD AND COMPILE SHADER PROGRAM******************************************/
		
	Shader simpleShader("../shaders/simpleShader.vert", "../shaders/simpleShader.frag");

	/******************************************BUILD AND COMPILE SHADER PROGRAM******************************************/


	/******************************************VERTEX DATA AND ATTRIBUTES******************************************/

	//default
	//float vertices[] =
	//{
	//	// positions         // colors			// texture coordinates
	//	 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,	1.0f, 1.0f,				// top right
	//	 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,	1.0f, 0.0f,				// bottom right
	//	-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,	0.0f, 0.0f,				// bottom left
	//	-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,	0.0f, 1.0f,				// top left
	//};

	// multiplied by 2
	float vertices[] =
	{
		// positions         // colors			// texture coordinates
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,	2.0f, 2.0f,				// top right
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,	2.0f, 0.0f,				// bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,	0.0f, 0.0f,				// bottom left
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,	0.0f, 2.0f,				// top left
	};


	// zooming in to textures
	//float vertices[] = 
	//{
	//	// positions         // colors			// texture coordinates
	//	 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,	0.55f, 0.55f,				// top right
	//	 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,	0.55f, 0.45f,				// bottom right
	//	-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,	0.45f, 0.45f,				// bottom left
	//	-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,	0.45f, 0.55f,				// top left
	//};

	//3	  +	  3	  +	  2	  =  8 (stride parameter = 8)

	unsigned int indices[] =
	{
		0, 1, 3,	//First triagle
		1, 2, 3		//Second triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coordinate attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	/******************************************VERTEX DATA AND ATTRIBUTES******************************************/

	/******************************************LOAD IMAGE USING STB_IMAGE******************************************/


		unsigned int texture1, texture2;
		int width, height, nrChannels;
		unsigned char* data = {};

		glGenTextures(1, &texture1);
		glBindTexture(GL_TEXTURE_2D, texture1);

		// Texture wrapping/filtering options
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load and generate texture
		stbi_set_flip_vertically_on_load(true);
		data = stbi_load("../assets/container.jpg", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

		glGenTextures(1, &texture2);
		glBindTexture(GL_TEXTURE_2D, texture2);

		// Texture wrapping/filtering options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		data = stbi_load("../assets/awesomeface.png", &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else
		{
			std::cout << "Failed to load texture" << std::endl;
		}
		stbi_image_free(data);

	/******************************************LOAD IMAGE USING STB_IMAGE******************************************/
	
	simpleShader.use();
	simpleShader.setInt("myTexture1", 0);
	simpleShader.setInt("myTexture2", 1);
	simpleShader.setFloat("visibility", 0.75f);

	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		std::cout << "Visibility multiplier: " << visibility << std::endl;

		simpleShader.use();
		simpleShader.setFloat("visibility", visibility);

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate and bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);

		simpleShader.use();
		//simpleShader.setFloat("hOffset", 0.5f);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glfwTerminate();
	return 0;
}


//glfw: whenever the window size changed (by OS or user resize) this callback function executes
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

