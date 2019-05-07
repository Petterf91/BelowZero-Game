#include "TextureLib.h"

TextureLib::TextureLib()
{
}
TextureLib::~TextureLib()
{
}

void TextureLib::addAlbedo(std::string filePath)
{
	Texture newMat = Texture(filePath);
	albedo.push_back(newMat);
}

const Texture * TextureLib::getAlbedo(int key)
{
	return &albedo.at(key);
}