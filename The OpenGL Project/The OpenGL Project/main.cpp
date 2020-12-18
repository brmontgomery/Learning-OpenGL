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
#include <array>
#include <chrono> 
#include <random>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

// camera
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;



class RectangleList {
private:
	class Rectangle {
	private:
		glm::vec3 color;
		std::array<glm::vec3, 4> rectVerts;
	public:
		Rectangle() {
			color = { 1.0f, 1.0f, 1.0f };
			rectVerts = { glm::vec3{ 1.0f, -1.0f, 0.0f }, glm::vec3{ 1.0f, -1.0f, 0.0f }, glm::vec3{ 1.0f, -1.0f, 0.0f }, glm::vec3{ 1.0f, -1.0f, 0.0f } };
		};
		Rectangle(float newHeight) {
			color = { 1.0f, 1.0f, 1.0f };
			rectVerts = { glm::vec3{ -1.0f, newHeight, 0.0f }, glm::vec3{ 1.0f, newHeight, 0.0f }, glm::vec3{ 1.0f, -1.0f, 0.0f }, glm::vec3{ 1.0f, -1.0f, 0.0f } };
		};
		~Rectangle() {};

		bool operator<(const Rectangle &other) {
			if (rectVerts[0].y < other.getVert(0).y) {
				return true;
			}
			return false;
		}

		void setRectangleHeight(float newHeight) {
			rectVerts[0].y = newHeight;
			rectVerts[1].y = newHeight;
		}

		void setColor(glm::vec3 newColor) {
			color = newColor;
		}

		void setRectangleXPos(float newXPos, float width) {
			rectVerts[0].x = newXPos - (width / 2);
			rectVerts[1].x = newXPos + (width / 2);
			rectVerts[2].x = newXPos - (width / 2);
			rectVerts[3].x = newXPos + (width / 2);
		}

		glm::vec3 getVert(int i) const {
			if (i >= 0 && i < 4) {
				return rectVerts[i];
			}
			return glm::vec3();
		}
		glm::vec3 getColor() const {
			return color;
		}
	};

	std::array<Rectangle, 100> rectList;
	float useScreenWidth = 1.96f;
	float useScreenHeight = 1.8f;
	float rectWidth = (2.0f/rectList.size());

public:
	RectangleList() {
		for (int i = 0; i < rectList.size(); i++) {
			rectList[i] = Rectangle(((useScreenHeight / rectList.size()) * (i + 1)) - 0.88f);
		}
		shuffle();

		for (int i = 0; i < rectList.size(); i++) {
			//setLocation based off of order in array
			rectList[i].setRectangleXPos(((useScreenWidth / rectList.size()) * i + 1) - (1.98f), rectWidth);
		}
	}

	//shuffle
	void shuffle() {
		unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

		std::shuffle(rectList.begin(), rectList.end(), std::default_random_engine(seed));
	}

	void swap(int first, int second, int &lastFirst, int &lastSecond) {
		rectList[first].setColor(glm::vec3(1.0f, 1.0f, 0.0f));
		rectList[second].setColor(glm::vec3(1.0f, 1.0f, 0.0f));
		rectList[lastFirst].setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		rectList[lastSecond].setColor(glm::vec3(1.0f, 1.0f, 1.0f));

		Rectangle temp = rectList[first];
		rectList[first] = rectList[second];
		rectList[second] = temp;

		rectList[first].setRectangleXPos(((useScreenWidth / rectList.size()) * first + 1) - (1.98f), rectWidth);
		rectList[second].setRectangleXPos(((useScreenWidth / rectList.size()) * second + 1) - (1.98f), rectWidth);

		lastFirst = first;
		lastSecond = second;
	}

	bool less(int first, int second) {
		if (rectList[first] < rectList[second]) {
			return true;
		}
		return false;
	}

	bool is_sorted(int lastSwap1, int lastSwap2) {
		for (int i = 0; i < rectList.size()-1; i++) {
			if (!less(i, i + 1)) {
				return false;
			}
		}
		rectList[lastSwap1].setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		rectList[lastSwap2].setColor(glm::vec3(1.0f, 1.0f, 1.0f));
		return true;
	}

	int size() {
		return rectList.size();
	}

	//draw
	std::vector<float> getVertices() {
		std::vector<float> temp;
		for (int i = 0; i < rectList.size(); i++) {
			for (int j = 0; j < 3; j++) {
				temp.push_back(rectList[i].getVert(j).x);
				temp.push_back(rectList[i].getVert(j).y);
				temp.push_back(rectList[i].getVert(j).z);
				temp.push_back(rectList[i].getColor().x);
				temp.push_back(rectList[i].getColor().y);
				temp.push_back(rectList[i].getColor().z);
			}
			for (int j = 1; j < 4; j++) {
				temp.push_back(rectList[i].getVert(j).x);
				temp.push_back(rectList[i].getVert(j).y);
				temp.push_back(rectList[i].getVert(j).z);
				temp.push_back(rectList[i].getColor().x);
				temp.push_back(rectList[i].getColor().y);
				temp.push_back(rectList[i].getColor().z);
			}
		}
		return temp;
	}

	
};

bool sort(RectangleList &rectList, int &low, int &high, int &sortLoop1, int &sortLoop2, int &lastSwap1, int &lastSwap2);

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
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Insertion Sort", NULL, NULL);
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
	glfwSetKeyCallback(window, key_callback);


	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);


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
	Shader twoDShader("Shaders/2DShader.vert", "Shaders/2DShader.frag");

	// init the Rectangle List
	// ------------------------------------
	RectangleList rectList;
	std::vector<float> vertices = rectList.getVertices();


	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	//sort variables
	bool sorted = false;
	int sortLow = 0, sortHigh = rectList.size(), sortLoop1 = sortLow, sortloop2 = sortLoop1, lastSwap1 = 0, lastSwap2 = 0;
	
	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		// per-frame time logic
		// --------------------
		float currentFrame = 0.0f;
		while (deltaTime < 0.016f) {
			currentFrame = glfwGetTime();
			deltaTime = currentFrame - lastFrame;
		}
		lastFrame = currentFrame;

		// input
		// -----
		processInput(window);
		if (!sorted) {
			sorted = sort(rectList, sortLow, sortHigh, sortLoop1, sortloop2, lastSwap1, lastSwap2);
			vertices = rectList.getVertices();
			glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
		}

		glfwGetFramebufferSize(window, &curWidth, &curHeight);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// render the triangle
		twoDShader.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 6);

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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

	//up
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {}
		//restart
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
	
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	
}

bool sort(RectangleList &rectList, int &low, int &high, int &sortLoop1, int &sortLoop2, int &lastSwap1, int &lastSwap2) {
	bool exit = false;
	bool nextLoop = true;

	while (!exit) {
		if (sortLoop1 <= high) {
			if (nextLoop) {
				sortLoop2 = sortLoop1;
				nextLoop = false;
			}
			if (sortLoop2 > 0) {
				if (rectList.less(sortLoop2, sortLoop2 - 1)) { 
					rectList.swap(sortLoop2, sortLoop2 - 1, lastSwap1, lastSwap2); 
					exit = true; 
				}
				else { 
					sortLoop2--; 
				}
			}
			else {
				sortLoop1++;
				nextLoop = true;
			}
		}
		else {
			sortLoop1++;
		}
	}
	if (rectList.is_sorted(lastSwap1, lastSwap2)) {
		return true;
	}
	return false;
}

//switches shaders
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	
}


