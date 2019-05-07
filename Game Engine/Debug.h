#pragma once

#include <iostream>
#include <iomanip>
#include <string>

#include <GL/gl3w.h>    // This example is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
#include "glfw\include\GLFW\glfw3.h"

// Debug macros
#define ASSERT(x) if(!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

static void GLClearError()
{
	while (glGetError() != GL_NO_ERROR);
}

static bool GLLogCall(const char* function, const char* file, int line)
{
	// Error code can be checked in glew.h.
	// The error needs to be translated into hexadecimal numbers so we can search in the header. 

	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] (" << std::hex << error << "): " <<
			function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}