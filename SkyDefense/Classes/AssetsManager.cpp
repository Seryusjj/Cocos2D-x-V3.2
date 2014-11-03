#include "AssetsManager.h"

AssetsManager* AssetsManager::instance = nullptr;

AssetsManager::AssetsManager() {
	bg = nullptr;

}

AssetsManager* AssetsManager::getInstance(){
	if (instance == nullptr){
		instance = new AssetsManager();
	}
	return instance;
}


Sprite* AssetsManager::getBackground(){
	if (bg == nullptr){
		bg = Sprite::create("bg.png");
	}
	return bg;
}