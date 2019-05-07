#include "ShaderProgramLib.h"

ShaderProgramLib::ShaderProgramLib()
{
}

ShaderProgramLib::~ShaderProgramLib()
{
	for (ShaderBase* shader_ptr : shaders)
		delete shader_ptr;
	shaders.clear();
}

void ShaderProgramLib::addGeometryPassShaders()
{
	//add your shader here
	GeometryShaders* newShaderProgram = new GeometryShaders();
	shaders.push_back(newShaderProgram);
}

void ShaderProgramLib::addLightpassShaders()
{
	//add your shader here
	LightpassShaders* newShaderProgram = new LightpassShaders();
	shaders.push_back(newShaderProgram);
}

void ShaderProgramLib::addShadowMapShaders()
{
	//add your shader here
	ShadowMapShader* newShaderProgram = new ShadowMapShader();
	shaders.push_back(newShaderProgram);
}

void ShaderProgramLib::addAnimationShaders()
{
	AnimationShaders* newShaderProgram = new AnimationShaders();
	shaders.push_back(newShaderProgram);
}

void ShaderProgramLib::addTerrainShaders()
{
	TerrainShaders* newShaderProgram = new TerrainShaders();
	shaders.push_back(newShaderProgram);
}

void ShaderProgramLib::addMainMenuShaders()
{
	MainMenuShader* newShaderProgram = new MainMenuShader();
	shaders.push_back(newShaderProgram);
}

void ShaderProgramLib::addRefractionShaders()
{
	RefractionShader* newShaderProgram = new RefractionShader();
	shaders.push_back(newShaderProgram);
}

void ShaderProgramLib::addUIShaders()
{
	UIShaders* newShaderProgram = new UIShaders();
	shaders.push_back(newShaderProgram);
}

void ShaderProgramLib::addVFXFireShaders()
{
	VFXFireShaders* newShaderProgram = new VFXFireShaders();
	shaders.push_back(newShaderProgram);
}

void ShaderProgramLib::addVFXSnowShaders()
{
	VFXSnowShaders* newShaderProgram = new VFXSnowShaders();
	shaders.push_back(newShaderProgram);
}

void ShaderProgramLib::addVFXFlareShaders()
{
	VFXFlareShaders* newShaderProgram = new VFXFlareShaders();
	shaders.push_back(newShaderProgram);
}

void ShaderProgramLib::addVFXLighterShaders()
{
	VFXLighterShaders* newShaderProgram = new VFXLighterShaders();
	shaders.push_back(newShaderProgram);
}
