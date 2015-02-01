﻿#ifndef __GAMESPRITE_H__
#define __GAMESPRITE_H__

#include "cocos2d.h"

using namespace cocos2d;

class GameSprite : public Sprite {
public:
    
    CC_SYNTHESIZE(float, _radius, Radius);
    
	static GameSprite* createWithFrameName(const char * pszSpriteFrameName);
		
};

#endif // __GAMESPRITE_H__


		