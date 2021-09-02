#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (location = 0) out vec3 FragPos;
layout (location = 1) out vec3 Normal;
layout (location = 2) out vec2 TexCoords;

#ifdef VULKAN
	layout(set = 0, binding = 0) uniform UniformBuffer { 
		mat4 model;
		mat4 view;
		mat4 projection;
	}; 
#else
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
#endif

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;  
    TexCoords = aTexCoords;
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}