#include "Texture.h"

Texture::Texture()
{
}

Texture::Texture(std::string filePath)
{
	CreateTextureData(filePath);
}

Texture::~Texture()
{
}

void Texture::CreateTextureData(std::string filePath)
{
	//Texture
	glGenTextures(1, &gTexture);
	glBindTexture(GL_TEXTURE_2D, gTexture);
	// set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load and generate the texture
	const char* fileName = filePath.c_str();
	unsigned char *data = stbi_load(fileName, &width, &height, &nrChannels, 0);

	//Find a way to check if the file is RGBA or RGB
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);
}

glm::vec3 Texture::getRGB(int x, int y)
{
	float * pixels = new float;
	std::vector<float> test;


	glBindTexture(GL_TEXTURE_2D, this->gTexture);
	//glGetTexImage(GL_TEXTURE_2D, 0, GL_RGB, GL_UNSIGNED_BYTE, PBO);
	//glReadPixels(2, 5, this->width, this->height, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);
	glTexSubImage2D(this->gTexture, 0, 12, 10, this->width, this->height, GL_RGB32F, GL_FLOAT, &pixels[0]);

	return glm::vec3(1);
}
