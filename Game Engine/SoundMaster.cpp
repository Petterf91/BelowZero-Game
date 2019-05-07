#include "SoundMaster.h"

SoundMasterSFML::SoundMasterSFML()
{	
	existSound = false;
}

SoundMasterSFML::~SoundMasterSFML()
{
	//this->Sound->stop();

	if (existSound)
	{
		if(Buffer != nullptr)
			delete this->Buffer;
		if(Sound != nullptr)
			delete this->Sound;
		existSound = false;
	}
}

void SoundMasterSFML::addSound(const std::string & filePath)
{
	this->Sound = new sf::Sound;
	this->Buffer = new sf::SoundBuffer;

	if (!this->Buffer->loadFromFile(filePath))
		std::cout << "Cant Load File" << std::endl;

	this->Sound->setBuffer(*Buffer);
	existSound = true;
}

bool SoundMasterSFML::isPlaying()
{
	if (this->Sound->getStatus() == 2)
		return true;

	return false;
}

void SoundMasterSFML::playSound()
{
	this->Sound->play();
}

void SoundMasterSFML::pauseSound()
{
	this->Sound->pause();
}

void SoundMasterSFML::stopSound()
{
	this->Sound->stop();
}

void SoundMasterSFML::setVolume(float volume)
{
	this->Sound->setVolume(volume);
}

void SoundMasterSFML::setPitch(float pitch)
{
	this->Sound->setPitch(pitch);

}

void SoundMasterSFML::setPosition(glm::vec3 pos)
{
	this->Sound->setPosition(pos.x, pos.y, pos.z);
}

void SoundMasterSFML::setRelativeToListener(bool relative)
{
	this->Sound->setRelativeToListener(relative);
}

void SoundMasterSFML::setAttenuation(float atten)
{
	this->Sound->setAttenuation(atten);

}

void SoundMasterSFML::setMinDistance(float distance)
{
	this->Sound->setMinDistance(distance);
}

void SoundMasterSFML::loop(bool looping)
{
	this->Sound->setLoop(looping);
}
