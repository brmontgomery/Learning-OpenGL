#version 330 core
layout(location = 0) in vec3 aPos;

uniform vec3 horizOffset;

void main()
{
gl_Position = vec4(aPos.x + horizOffset.x, -aPos.y + horizOffset.y, aPos.z + horizOffset.z, 1.0);
}