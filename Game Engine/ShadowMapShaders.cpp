#include "ShadowMapShaders.h"

ShadowMapShader::ShadowMapShader()
{
	CreateShaderData();
}

ShadowMapShader::ShadowMapShader(int otherAssetID)
{
	assetID = otherAssetID;
	CreateShaderData();
}

ShadowMapShader::~ShadowMapShader()
{
}

void ShadowMapShader::CreateShaderData()
{
	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = 0;

	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	std::ifstream shaderFile("ShadowMapVS.glsl");
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
	shaderFile.open("ShadowMapFS.glsl");
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

	ShadowMapShaderProgram = glCreateProgram();
	glAttachShader(ShadowMapShaderProgram, fs);
	glAttachShader(ShadowMapShaderProgram, vs);
	glLinkProgram(ShadowMapShaderProgram);


	compileResult = GL_FALSE;
	glGetProgramiv(ShadowMapShaderProgram, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE) 
	{
		memset(buff, 0, 1024);
		glGetProgramInfoLog(ShadowMapShaderProgram, 1024, nullptr, buff);
		OutputDebugStringA(buff);
	}

	glDetachShader(ShadowMapShaderProgram, vs);
	glDetachShader(ShadowMapShaderProgram, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

}
