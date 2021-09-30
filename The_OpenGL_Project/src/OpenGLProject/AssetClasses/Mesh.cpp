#include "OpenGLProject/AssetClasses/Mesh.h"

Mesh::Mesh(const std::vector<float> vertices, const std::vector<unsigned int> indices, const std::vector<TextureInfo> textures)
	: vertices(vertices), indices(indices), textures(textures)
{
	setupMesh();
}

void Mesh::Submit(const Ref<Shader>& shader, const glm::mat4& transform, Ref<Texture2DLibrary> s_Texture2DLibrary) {
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		std::string number;
		std::string name = textures[i].type;

		if (name == "texture_diffuse")
			number = std::to_string(diffuseNr++);
		else if (name == "texture_specular")
			number = std::to_string(specularNr++);

		shader->Bind();
		shader->SetFloat(("material." + name + number).c_str(), i);

		s_Texture2DLibrary->Get(textures[i].name)->Bind();
	}

	// draw mesh
	Renderer::Submit(shader, vertexArray, transform);
}

void Mesh::setupMesh()
{
	vertexArray = VertexArray::Create();

	Ref<VertexBuffer> vertexBuffer = VertexBuffer::Create(vertices.data(), vertices.size());
	BufferLayout layout = {
		{ ShaderDataType::Float3, "a_Position" },
		{ ShaderDataType::Float3, "a_Normal" },
		{ ShaderDataType::Float2, "a_TexCoords" }
	};
	vertexBuffer->SetLayout(layout);
	vertexArray->AddVertexBuffer(vertexBuffer);

	Ref<IndexBuffer> indexBuffer = IndexBuffer::Create(&indices[0], indices.size());
	vertexArray->SetIndexBuffer(indexBuffer);
}