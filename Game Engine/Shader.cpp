#include "Shader.h"

Shader::Shader() : ShaderBase()
{
}

Shader::Shader(int otherAssetID) : ShaderBase()
{
	this->assetID = otherAssetID;

	CreateShaderData();
}

Shader::~Shader()
{
}

void Shader::CreateShaderData()
{

	// local buffer to store error strings when compiling.
	char buff[1024];
	memset(buff, 0, 1024);
	GLint compileResult = 0;

	//create vertex shader "name" and store it in "vs"
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);

	// open .glsl file and put it in a string
	ifstream shaderFile("VertexShader.glsl");
	std::string shaderText((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();

	// glShaderSource requires a double pointer.
	// get the pointer to the c style string stored in the string object.
	const char* shaderTextPtr = shaderText.c_str();

	// ask GL to use this string a shader code source
	glShaderSource(vs, 1, &shaderTextPtr, nullptr);

	// try to compile this shader source.
	glCompileShader(vs);

	// check for compilation error
	glGetShaderiv(vs, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		// query information about the compilation (nothing if compilation went fine!)
		glGetShaderInfoLog(vs, 1024, nullptr, buff);
		// print to Visual Studio debug console output
		OutputDebugStringA(buff);
	}

	////geometry shader
	//GLuint gs = glCreateShader(GL_GEOMETRY_SHADER);
	//shaderFile.open("GeometryShader.glsl");
	//shaderText.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	//shaderFile.close();
	//shaderTextPtr = shaderText.c_str();
	//glShaderSource(gs, 1, &shaderTextPtr, nullptr);
	//glCompileShader(gs);
	//// query information about the compilation (nothing if compilation went fine!)
	//compileResult = GL_FALSE;
	//glGetShaderiv(gs, GL_COMPILE_STATUS, &compileResult);
	//if (compileResult == GL_FALSE) {
	//	// query information about the compilation (nothing if compilation went fine!)
	//	memset(buff, 0, 1024);
	//	glGetShaderInfoLog(gs, 1024, nullptr, buff);
	//	// print to Visual Studio debug console output
	//	OutputDebugStringA(buff);
	//}


	// repeat process for Fragment Shader (or Pixel Shader)
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	shaderFile.open("Fragment.glsl");
	shaderText.assign((std::istreambuf_iterator<char>(shaderFile)), std::istreambuf_iterator<char>());
	shaderFile.close();
	shaderTextPtr = shaderText.c_str();
	glShaderSource(fs, 1, &shaderTextPtr, nullptr);
	glCompileShader(fs);
	// query information about the compilation (nothing if compilation went fine!)
	compileResult = GL_FALSE;
	glGetShaderiv(fs, GL_COMPILE_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		// query information about the compilation (nothing if compilation went fine!)
		memset(buff, 0, 1024);
		glGetShaderInfoLog(fs, 1024, nullptr, buff);
		// print to Visual Studio debug console output
		OutputDebugStringA(buff);
	}

	//link shader program (connect vs and ps)
	gShaderProgram = glCreateProgram();
	glAttachShader(gShaderProgram, fs);
	//glAttachShader(gShaderProgram, gs);
	glAttachShader(gShaderProgram, vs);
	glLinkProgram(gShaderProgram);

	// check once more, if the Vertex Shader and the Fragment Shader can be used
	// together
	compileResult = GL_FALSE;
	glGetProgramiv(gShaderProgram, GL_LINK_STATUS, &compileResult);
	if (compileResult == GL_FALSE) {
		// query information about the compilation (nothing if compilation went fine!)
		memset(buff, 0, 1024);
		glGetProgramInfoLog(gShaderProgram, 1024, nullptr, buff);
		// print to Visual Studio debug console output
		OutputDebugStringA(buff);
	}
	// in any case (compile sucess or not), we only want to keep the 
	// Program around, not the shaders.
	glDetachShader(gShaderProgram, vs);
	//glDetachShader(gShaderProgram, gs);
	glDetachShader(gShaderProgram, fs);
	glDeleteShader(vs);
	//glDeleteShader(gs);
	glDeleteShader(fs);


	//////////////////////////////////////////////////////
	//////////////////////////////////////////////////////
	//////////////////////////////////////////////////////

	////vao
	//glGenVertexArrays(1, &gVertexAttribute);
	//glBindVertexArray(gVertexAttribute);
	//glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	//glEnableVertexAttribArray(2);

	////vbo
	//glGenBuffers(1, &gVertexBuffer);
	//glBindBuffer(GL_ARRAY_BUFFER, gVertexBuffer);
	//glBufferData(GL_ARRAY_BUFFER,mesh.Vertices.size()*sizeof(Vertex), &mesh.Vertices[0], GL_STATIC_DRAW);

	//// query which "slot" corresponds to the input vertex_position in the Vertex Shader 
	//GLint vertexPos = glGetAttribLocation(gShaderProgram, "vertex_position");
	//// if this returns -1, it means there is a problem, and the program will likely crash.
	//// examples: the name is different or missing in the shader

	//if (vertexPos == -1) {
	//	OutputDebugStringA("Error, cannot find 'vertex_position' attribute in Vertex shader\n");
	//	return;
	//}

	//// tell OpenGL about layout in memory (input assembler information)
	//glVertexAttribPointer(
	//	vertexPos,				// location in shader
	//	3,						// how many elements of type (see next argument)
	//	GL_FLOAT,				// type of each element
	//	GL_FALSE,				// integers will be normalized to [-1,1] or [0,1] when read...
	//	sizeof(Vertex), // distance between two vertices in memory (stride)
	//	BUFFER_OFFSET(0)		// offset of FIRST vertex in the list.
	//);

	//// repeat the process for the second attribute.
	//// query which "slot" corresponds to the input vertex_color in the Vertex Shader 
	//GLuint vertexColor = glGetAttribLocation(gShaderProgram, "vertex_normal");
	//glVertexAttribPointer(
	//	vertexColor,
	//	3,
	//	GL_FLOAT,
	//	GL_FALSE, sizeof(Vertex), // distance between two vertexColor 
	//	BUFFER_OFFSET(sizeof(float) * 3)	// note, the first color starts after the first vertex.
	//);

	//// repeat the process for the third attribute.
	//// query which "slot" corresponds to the input uv coord in the Vertex Shader 
	//GLuint uvPos = glGetAttribLocation(gShaderProgram, "uv_coord");
	//glVertexAttribPointer(
	//	uvPos,
	//	2,
	//	GL_FLOAT,
	//	GL_FALSE, sizeof(Vertex), // distance between two uv coord 
	//	BUFFER_OFFSET(sizeof(float) * 6)	// note, the first uv starts after the first color.
	//);

	////gElementBuffer
	//unsigned int gElementBuffer;
	//glGenBuffers(1, &gElementBuffer);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gElementBuffer);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);


}
