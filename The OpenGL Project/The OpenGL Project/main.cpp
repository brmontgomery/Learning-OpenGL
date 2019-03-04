#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shaders.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main() {
	//intitializing GLFW
	glfwInit();

	//window***********************\/

	//setting up window settings
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//creating window
	GLFWwindow* window = glfwCreateWindow(800, 600, "The Grand Experiment", NULL, NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	//register our viewport with resizing callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//making 'window' the current context
	glfwMakeContextCurrent(window);

	//window***********************/\

	//initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD!";
		return -1;
	}

	Shader shader1("Shaders/vertexShader.vert", "Shaders/fragmentShader.frag");

	//vertex data
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		-0.25f, 0.0f, 0.0f,
		 0.0f, -0.5f, 0.0f,
		 0.25f, 0.0f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	unsigned int indeces[] = {
		0, 1, 2,
		1, 3, 5,
		2, 3, 4
	};

	//create buffers
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	//bind array buffers and data
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indeces), indeces, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//rendering loop
	while (!glfwWindowShouldClose(window)) {
		//input
		processInput(window);

		//rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader1.use();

		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, sizeof(indeces), GL_UNSIGNED_INT, 0);

		//check events and swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//deallocate memory
	glfwTerminate();
	return 0;
}

//gets resizing info
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

//checks for input and processes it
void processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}