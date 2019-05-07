#pragma once
#include<vector>
#include"Texture.h"
#include<string>

class TextureLib
{
public:
	TextureLib();
	~TextureLib();

	void addAlbedo(std::string filePath);

	const Texture* getAlbedo(int key);

	std::vector<Texture> albedo;

private:
	
};