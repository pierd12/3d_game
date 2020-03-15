#include "SoundManager.h"
#include <string>
#include <iostream>


#pragma comment(lib, "irrKlang.lib")


SoundManager::SoundManager() {
	this->soundeffects = irrklang::createIrrKlangDevice();
	if (!soundeffects)
	{
		std::cout << "Error: Could not create Sound Engine" << std::endl;
		return;
	}
	soundeffects->play2D("./sounds/intro.mp3");
}
SoundManager::~SoundManager() {
	
}
void SoundManager::backgroundSound() {
	voice = soundeffects->play2D("./sounds/game.wav", true,false,true);
	if (voice != 0)
	{
		std::cout << "Error: Could not play file" << std::endl;
		return;
	}
	//voice->setVolume(0.5);
}
void SoundManager::stopbackgroundSound() {
	voice->stop();
	
}
void SoundManager::setVolume(int v) {
	if (isOn)
		voice->setVolume((float)(v/10.0));
}
void SoundManager::attack() {
	if(isOn)
		soundeffects->play2D("./sounds/snake.mp3");
}
void SoundManager::miss(){
	if (isOn)
		soundeffects->play2D("./sounds/miss.mp3");
}
void SoundManager::levelup(){
	if (isOn)
		soundeffects->play2D("./sounds/levelup.ogg");
}
void SoundManager::special(){
	if (isOn) {
		voice->stop();
		voice = soundeffects->play2D("./sounds/speciallevel.ogg");
	}
	else
		voice = soundeffects->play2D("./sounds/speciallevel.ogg");
}
void SoundManager::Ending() {
	if (isOn)
		voice->stop();
	//soundeffects->play2D("./sounds/victory.wav");
	//soundeffects->play2D("./sounds/end.aiff");
}