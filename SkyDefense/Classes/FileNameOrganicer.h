#ifndef __FILE_NAME_ORGANICER_H__
#define __FILE_NAME_ORGANICER_H__


#include "cocos2d.h"
#include <iostream>

using namespace cocos2d;

class FileNameOrganicer
{
private:
	FileNameOrganicer(){};
	static FileNameOrganicer* instance;
public:
	//absolute path to load the archives
	const char* backgroundMusicPreloadPath = FileUtils::getInstance()->fullPathForFilename("background.mp3").c_str();
	const char* bombFailSoundPreloadPath = FileUtils::getInstance()->fullPathForFilename("bombFail.wav").c_str();
	const char* bombReleaseSoundPreloadPath = FileUtils::getInstance()->fullPathForFilename("bombRelease.wav").c_str();
	const char* boomSoundPreloadPath = FileUtils::getInstance()->fullPathForFilename("boom.wav").c_str();
	const char* healthSoundPreloadPath = FileUtils::getInstance()->fullPathForFilename("health.wav").c_str();
	const char* fireSoundPreloadPath = FileUtils::getInstance()->fullPathForFilename("fire_truck.wav").c_str();

	//file names to be used
	const char* backgroundMusic = "background.mp3";
	const char* bombFailSound = "bombFail.wav";
	const char* bombReleaseSound = "bombRelease.wav";
	const char* boomSound = "boom.wav";
	const char* healthSound = "health.wav";
	const char* fireSound = "fire_truck.wav";

	//texture source names
	const std::string spriteSheetPlist = "sprite_sheet.plist";
	const std::string spriteSheetPng = "sprite_sheet.png";

	//texture names
	const std::string backgroundTexure = "bg.png";
	const std::string cityDarkTexure = "city_dark.png";
	const std::string cityLightTexture = "city_light.png";
	const std::string treesTexture = "trees.png";
	const std::string cloudTexture = "cloud.png";
	const std::string bombTexture = "bomb.png";
	const std::string sparckleTexture = "sparkle.png";
	const std::string haloTexture = "halo.png";
	const std::string shockwaveTexture = "shockwave.png";
	const std::string meteorTexture = "meteor.png";
	const std::string healthTexture = "health.png";

	//ui texture name
	const std::string fontUISource = "font.fnt";
	const std::string introMessageUITexture = "logo.png";
	const std::string gameOverUITexture = "gameover.png";
	const std::string healtUITexture = "health_icon.png";

	static FileNameOrganicer* getInstance();

};



#endif