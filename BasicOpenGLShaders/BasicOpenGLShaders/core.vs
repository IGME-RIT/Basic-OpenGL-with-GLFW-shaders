// NOTE: before proceeding further, it is recommended you learn about the programmable graphics pipeline.
// To know more about the programmable graphics pipeline please visit this website:
// http://duriansoftware.com/joe/An-intro-to-modern-OpenGL.-Chapter-1:-The-Graphics-Pipeline.html

// The vertex shader is used to transform the attributes of vertices such as color, texture, position
// and the direction from the original color space to the display space. It allows the original objects
// to be distorted or reshaped in any manner

#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec3 ourColor;

void main()
{
	gl_Position = vec4(position, 1.0);
	ourColor = color;
}