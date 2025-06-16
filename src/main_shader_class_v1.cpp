#include <common/shader.h>
#include <common/resource_manager.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//#define STB_IMAGE_IMPLEMENTATION
//#include "stb_image.h"

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
		
	Shader simpleShader("../shaders/simpleShader.vert", "../shaders/simpleShader.frag", true);

	/******************************************BUILD AND COMPILE SHADER PROGRAM******************************************/


	/******************************************VERTEX DATA AND ATTRIBUTES******************************************/

	//default
	float vertices[] =
	{
		// positions         // colors			// texture coordinates
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,	1.0f, 1.0f,				// top right
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,	1.0f, 0.0f,				// bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,	0.0f, 0.0f,				// bottom left
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,	0.0f, 1.0f,				// top left
	};

	// multiplied by 2
	//float vertices[] =
	//{
	//	// positions         // colors			// texture coordinates
	//	 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,	2.0f, 2.0f,				// top right
	//	 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,	2.0f, 0.0f,				// bottom right
	//	-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,	0.0f, 0.0f,				// bottom left
	//	-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,	0.0f, 2.0f,				// top left
	//};


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

	ResourceManager manager;
	Texture* texture1 = manager.loadTexture("../assets/container.jpg");
	Texture* texture2 = manager.loadTexture("../assets/awesomeface.png");
		

	/******************************************LOAD IMAGE USING STB_IMAGE******************************************/

	
	/******************************************GLM MATRICE TRANSFORMATIONS******************************************/

	/*glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	glm::mat4 trans = glm::mat4(1.0f);
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;
	std::cout << vec.x << vec.y << vec.z << std::endl;*/


	//glm::mat4 trans = glm::mat4(1.0f);
	//trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(0.0, 0.0, 1.0));		//Rotate 90 degrees along z axis
	//trans = glm::scale(trans, glm::vec3(0.5, 0.5, 0.5));

	/******************************************GLM MATRICE TRANSFORMATIONS******************************************/

	simpleShader.use();
	simpleShader.setInt("myTexture1", 0);
	simpleShader.setInt("myTexture2", 1);
	simpleShader.setFloat("visibility", 0.75f);



	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		//std::cout << "Visibility multiplier: " << visibility << std::endl;

		simpleShader.use();
		simpleShader.setFloat("visibility", visibility);

		glm::mat4 trans = glm::mat4(1.0f);		// identity matrix
		trans = glm::translate(trans, glm::vec3(0.5f, -0.5f, 0.0f));
		trans = glm::rotate(trans, (float)glfwGetTime(), glm::vec3(0.0f, 0.0f, 1.0f));

		unsigned int transformLoc = glGetUniformLocation(simpleShader.ID, "transform");		// Finding uniformTransform's location
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));

		// render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// activate and bind textures
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1->getTextureId());

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2->getTextureId());

		simpleShader.use();
		//simpleShader.setFloat("hOffset", 0.5f);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 3);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		trans = glm::mat4(1.0f);	// reset to identity matrix
		trans = glm::translate(trans, glm::vec3(-0.5f, 0.5f, 0.0f));
		float scaleAmount = static_cast<float>(sin(glfwGetTime()));
		trans = glm::scale(trans, glm::vec3(scaleAmount, scaleAmount, scaleAmount));
		glUniformMatrix4fv(transformLoc, 1, GL_FALSE, &trans[0][0]);

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

