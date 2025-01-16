#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);

const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

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

	int  success;
	char infoLog[512];

	/******************************************VERTEX SHADER******************************************/
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);				//Creating vertex shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);					//Attaching shader source code to the shader object
	glCompileShader(vertexShader);												//Compiling the shader

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);		//Check shader compile errors
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	/******************************************VERTEX SHADER******************************************/

	/******************************************FRAGMENT SHADER******************************************/
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);	//Creating fragment shader
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);		//Attaching shader source code to the shader object
	glCompileShader(fragmentShader);									//Compiling the shader

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);		//Check shader compile errors
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}
	/******************************************FRAGMENT SHADER******************************************/


	/******************************************SHADER PROGRAM******************************************/
	unsigned int shaderProgram = glCreateProgram();				//Creating a shader program
	glAttachShader(shaderProgram, vertexShader);				//Attaching vertex shader's source code to the shader object
	glAttachShader(shaderProgram, fragmentShader);				//Attaching fragment shader's source code to the shader object
	glLinkProgram(shaderProgram);								//Linking the program

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);	// check for linking errors
	if (!success) 
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertexShader);			//Deleting vertex shader after linking
	glDeleteShader(fragmentShader);			//Deleting fragment shader after linking
	/******************************************SHADER PROGRAM******************************************/

	/******************************************VERTEX DATA AND ATTRIBUTES******************************************/
	float vertices[] =
	{
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};

	unsigned int indices[] =
	{
		0,1,3,			//first triangle
		1,2,3			//second triangle
	};

	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); //Wireframe mode (GL_LINE) | Default (GL_FILL)
	/******************************************VERTEX DATA AND ATTRIBUTES******************************************/

	while (!glfwWindowShouldClose(window))
	{
		//Inputs
		processInput(window);

		//Rendering
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		//glDrawArrays(GL_TRIANGLES, 0, 6);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0);

		//Check and call events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glDeleteProgram(shaderProgram);

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
}

