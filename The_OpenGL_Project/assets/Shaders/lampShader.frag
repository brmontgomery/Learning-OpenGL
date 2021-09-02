#version 450 core
layout (location = 0) in vec2 TexCoords;

layout (location = 0) out vec4 FragColor;

uniform struct { sampler2D texture_diffuse1 } Sampler2DStruct;

void main()
{    
    FragColor = texture(Sampler2Dstruct.texture_diffuse1, TexCoords);
}