#pragma once

#include <string>
#include <vector>

#include "OpenGLProject/AssetClasses/Shaders.h"
#include "OpenGLProject/Graphics/VertexArray.h"
#include "OpenGLProject/AssetClasses/Textures.h"

#include "OpenGLProject/Graphics/Renderer.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "GLFW/glfw3.h"
#include "glad/glad.h"

struct TextureInfo {
	std::string type;
	std::string name;
};

class Mesh {
	public:
		//mesh data
		std::vector<float> vertices;
		std::vector<unsigned int> indices;
		std::vector<TextureInfo> textures;

		//functions
		Mesh(const std::vector<float> vertices, const std::vector<unsigned int> indeces, const std::vector<TextureInfo> textures);
		Ref<VertexArray> getVertexArray() { return vertexArray; }
		void Submit(const Ref<Shader>& shader, const glm::mat4& transform, Ref<Texture2DLibrary> s_Texture2DLibrary);

	private:
		//render data
		Ref<VertexArray> vertexArray;
		//functions
		void setupMesh();
};