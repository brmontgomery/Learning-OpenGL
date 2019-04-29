#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stb-master/stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shaders.h"
#include "Camera.h"
#include "Models.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// lighting
glm::vec3 lampPos(1.2f, 1.0f, -3.0f);

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif


	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "The Grand Experiment", NULL, NULL);
	int curWidth = SCR_WIDTH;
	int curHeight = SCR_HEIGHT;
	if (window == NULL) {
		std::cout << "Failed to create GLFW window!" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);


	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);


	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);


	// build and compile our shader zprogram
	// ------------------------------------
	Shader fancyShader("Shaders/lightingShader.vert", "Shaders/lightingShader.frag");
	Shader flatShader("Shaders/flatShader.vert", "Shaders/flatShader.frag");

	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	Model ourModel("resources/models/nanosuit/crysis_nano_suit_2/scene.gltf");
	
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);
		glfwGetFramebufferSize(window, &curWidth, &curHeight);

		// render
		// ------
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// be sure to activate shader when setting uniforms/drawing objects
		fancyShader.use();
		fancyShader.setVec3("viewPos", camera.Position);
		fancyShader.setFloat("material.shininess", 32.0f);

		flatShader.use();
		flatShader.setVec3("viewPos", camera.Position);

		/*
		   Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index
		   the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
		   by defining light types as classes and set their values in there, or by using a more efficient uniform approach
		   by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
		*/
		// directional light
		fancyShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		fancyShader.setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		fancyShader.setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		fancyShader.setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		fancyShader.setVec3("pointLights[0].position", pointLightPositions[0]);
		fancyShader.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		fancyShader.setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		fancyShader.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		fancyShader.setFloat("pointLights[0].constant", 1.0f);
		fancyShader.setFloat("pointLights[0].linear", 0.09);
		fancyShader.setFloat("pointLights[0].quadratic", 0.032);
		// point light 2
		fancyShader.setVec3("pointLights[1].position", pointLightPositions[1]);
		fancyShader.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		fancyShader.setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		fancyShader.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		fancyShader.setFloat("pointLights[1].constant", 1.0f);
		fancyShader.setFloat("pointLights[1].linear", 0.09);
		fancyShader.setFloat("pointLights[1].quadratic", 0.032);
		// point light 3
		fancyShader.setVec3("pointLights[2].position", pointLightPositions[2]);
		fancyShader.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		fancyShader.setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		fancyShader.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		fancyShader.setFloat("pointLights[2].constant", 1.0f);
		fancyShader.setFloat("pointLights[2].linear", 0.09);
		fancyShader.setFloat("pointLights[2].quadratic", 0.032);
		// point light 4
		fancyShader.setVec3("pointLights[3].position", pointLightPositions[3]);
		fancyShader.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		fancyShader.setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		fancyShader.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		fancyShader.setFloat("pointLights[3].constant", 1.0f);
		fancyShader.setFloat("pointLights[3].linear", 0.09);
		fancyShader.setFloat("pointLights[3].quadratic", 0.032);

		// spotLight
		fancyShader.setVec3("spotLight.position", camera.Position);
		fancyShader.setVec3("spotLight.direction", camera.Front);
		fancyShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		fancyShader.setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		fancyShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		fancyShader.setFloat("spotLight.constant", 1.0f);
		fancyShader.setFloat("spotLight.linear", 0.09);
		fancyShader.setFloat("spotLight.quadratic", 0.032);
		fancyShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		fancyShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		fancyShader.setMat4("projection", projection);
		fancyShader.setMat4("view", view);
		flatShader.setMat4("projection", projection);
		flatShader.setMat4("view", view);

		// world transformation
		glm::mat4 model = glm::mat4(1.0f);
		fancyShader.setMat4("model", model);
		flatShader.setMat4("model", model);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// glfw: terminate, clearing all previously allocated GLFW resources.
	// ------------------------------------------------------------------
	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
	bool sprint = 0;

	//exit program
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	//forward
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)) {
			sprint = 1;
		}
		camera.ProcessKeyboard(FORWARD, deltaTime, sprint);
	}

	//backwards
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime, sprint);

	//left
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime, sprint);

	//right
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime, sprint);

	//up
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.ProcessKeyboard(UP, deltaTime, sprint);

	//down
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.ProcessKeyboard(DOWN, deltaTime, sprint);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}


