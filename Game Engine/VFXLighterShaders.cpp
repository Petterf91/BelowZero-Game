#include "VFXLighterShaders.h"


VFXLighterShaders::VFXLighterShaders()
{
	CreateShaderData();
}

VFXLighterShaders::VFXLighterShaders(int otherAssetID)
{
}

VFXLighterShaders::~VFXLighterShaders()
{
}

void VFXLighterShaders::CreateShaderData()
{
	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = 0;

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	std::ifstream shaderFile("VFXLighterVS.glsl");
	std::string shaderText((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();

	const char* shaderTextPtr = shaderText.c_str();
	glShaderSource(vs, 1, &shaderTextPtr, nullptr);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE)
	{
		glGetShaderInfoLog(vs, 1024, nullptr, buff);
		OutputDebugStringA(buff);
	}

	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	shaderFile.open("VFXLighterFS.glsl");
	shaderText.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();
	shaderTextPtr = shaderText.c_str();
	glShaderSource(fs, 1, &shaderTextPtr, nullptr);
	glCompileShader(fs);
	compileResult = GL_FALSE;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE)
	{
		memset(buff, 0, 1024);
		glGetShaderInfoLog(fs, 1024, nullptr, buff);
		OutputDebugStringA(buff);
	}

	vfxLighterShaderProgram = glCreateProgram();
	glAttachShader(vfxLighterShaderProgram, fs);
	glAttachShader(vfxLighterShaderProgram, vs);
	glLinkProgram(vfxLighterShaderProgram);


	compileResult = GL_FALSE;
	glGetProgramiv(vfxLighterShaderProgram, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE)
	{
		memset(buff, 0, 1024);
		glGetProgramInfoLog(vfxLighterShaderProgram, 1024, nullptr, buff);
		OutputDebugStringA(buff);
	}

	glDetachShader(vfxLighterShaderProgram, vs);
	glDetachShader(vfxLighterShaderProgram, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
}
