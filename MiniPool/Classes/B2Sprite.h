#ifndef __MiniPool__b2Sprite__
#define __MiniPool__b2Sprite__



#include "cocos2d.h"
#include "Box2D/Box2D.h"


USING_NS_CC;

class GameLayer;

enum {
	kSpritePlayer,
	kSpriteBall,
	kSpriteCue,
	kSpritePocket
};


class B2Sprite : public Sprite
{
public:

	B2Sprite(GameLayer * game, int type);

	CC_SYNTHESIZE(b2Body *, _body, Body);
	CC_SYNTHESIZE(GameLayer *, _game, Game);
	CC_SYNTHESIZE(int, _type, Type);

	virtual void setSpritePosition(Point position);
	virtual void update(float dt);
	virtual void hide();
	virtual void reset();
	virtual float mag();
};

#endif /* defined(__MiniPool__b2Sprite__) */