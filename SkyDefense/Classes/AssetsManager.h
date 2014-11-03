#ifndef __ASSETS_MANAGER_H__
#define __ASSETS_MANAGER_H__


#include "cocos2d.h"

USING_NS_CC;
class AssetsManager
{
public:
	static AssetsManager* getInstance();
	Sprite* getBackground();

private:
	Sprite* bg;
	static  AssetsManager*  instance;
	AssetsManager();
};

#endif

