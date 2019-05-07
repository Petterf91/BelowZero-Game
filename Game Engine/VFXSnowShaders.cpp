#include "VFXSnowShaders.h"


VFXSnowShaders::VFXSnowShaders()
{
	CreateShaderData();
}

VFXSnowShaders::VFXSnowShaders(int otherAssetID)
{
}

VFXSnowShaders::~VFXSnowShaders()
{
}

void VFXSnowShaders::CreateShaderData()
{
	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = 0;

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	std::ifstream shaderFile("VFXSnowVS.glsl");
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
	shaderFile.open("VFXSnowFS.glsl");
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

	vfxSnowShaderProgram = glCreateProgram();
	glAttachShader(vfxSnowShaderProgram, fs);
	glAttachShader(vfxSnowShaderProgram, vs);
	glLinkProgram(vfxSnowShaderProgram);


	compileResult = GL_FALSE;
	glGetProgramiv(vfxSnowShaderProgram, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE)
	{
		memset(buff, 0, 1024);
		glGetProgramInfoLog(vfxSnowShaderProgram, 1024, nullptr, buff);
		OutputDebugStringA(buff);
	}

	glDetachShader(vfxSnowShaderProgram, vs);
	glDetachShader(vfxSnowShaderProgram, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
}
