#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);


int main() {
	//intitializing GLFW
	glfwInit();

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

	//making 'window' the current context
	glfwMakeContextCurrent(window);

	//initialize glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD!";
		return -1;
	}

	//set viewport
	glViewport(0, 0, 800, 600);

	//register our viewport with resizing callback
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//rendering loop
	while (!glfwWindowShouldClose(window)) {
		//input
		processInput(window);

		//rendering commands
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

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