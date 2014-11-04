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

	//texture names
	const std::string spriteSheetPlist = "sprite_sheet.plist";
	const std::string spriteSheetPng = "sprite_sheet.png";

	static FileNameOrganicer* getInstance();

};



#endif