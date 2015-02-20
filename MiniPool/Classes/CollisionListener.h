#ifndef __MiniPool__CollisionListener__
#define __MiniPool__CollisionListener__


#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Box2D/Box2D.h"
#include "B2Sprite.h"


using namespace CocosDenshion;

class CollisionListener : public b2ContactListener
{
public:
	CollisionListener();
	virtual void BeginContact(b2Contact* contact);
	//virtual void EndContact(b2Contact* contact);
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	//virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
};

#endif
