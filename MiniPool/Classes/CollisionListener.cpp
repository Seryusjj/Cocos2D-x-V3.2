#include "CollisionListener.h"
#include "GameLayer.h"

CollisionListener::CollisionListener(){
	b2ContactListener::b2ContactListener();
}


void CollisionListener::BeginContact(b2Contact* contact) {
	b2Body * bodyA = contact->GetFixtureA()->GetBody();
	b2Body * bodyB = contact->GetFixtureB()->GetBody();

	B2Sprite * spriteA = (B2Sprite *)bodyA->GetUserData();
	B2Sprite * spriteB = (B2Sprite *)bodyB->GetUserData();

	if (spriteA && spriteB) {

		//track collision between balls and pockets
		if (spriteA->getType() == kSpritePocket) {
			spriteB->setVisible(false);

		}
		else if (spriteB->getType() == kSpritePocket) {
			spriteA->setVisible(false);


		}
		else if (spriteA->getType() == kSpriteBall &&
			spriteB->getType() == kSpriteBall) {
			if (spriteA->mag() > 10 || spriteB->mag() > 10) {
				SimpleAudioEngine::getInstance()->playEffect("ball.wav");
			}
		}
		else if ((spriteA->getType() == kSpriteBall &&
			spriteB->getType() == kSpritePlayer) ||
			(spriteB->getType() == kSpriteBall &&
			spriteA->getType() == kSpritePlayer)) {
			if (spriteA->mag() > 10 || spriteB->mag() > 10) {
				SimpleAudioEngine::getInstance()->playEffect("ball.wav");
			}
		}
	}
}


void CollisionListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)  {

	b2Body * bodyA = contact->GetFixtureA()->GetBody();
	b2Body * bodyB = contact->GetFixtureB()->GetBody();

	B2Sprite * spriteA = (B2Sprite *)bodyA->GetUserData();
	B2Sprite * spriteB = (B2Sprite *)bodyB->GetUserData();


	if (spriteA && spriteB) {

		//track collision between player and cue ball
		if (spriteA->getType() == kSpriteCue && spriteA->mag() > 2) {
			if (spriteB->getType() == kSpritePlayer && spriteA->isVisible()) {
				SimpleAudioEngine::getInstance()->playEffect("hit.wav");
				spriteA->setVisible(false);
				spriteB->getGame()->setCanShoot(false);
			}
		}
		else if (spriteB->getType() == kSpriteCue && spriteA->mag() > 2) {
			if (spriteA->getType() == kSpritePlayer && spriteB->isVisible()) {
				SimpleAudioEngine::getInstance()->playEffect("hit.wav");
				spriteB->setVisible(false);
				spriteA->getGame()->setCanShoot(false);
			}
		}

	}

}
