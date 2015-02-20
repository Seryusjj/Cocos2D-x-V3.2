#include "B2Sprite.h"
#include "GameLayer.h"


B2Sprite::B2Sprite(GameLayer * game, int type)
{
	_game = game;
	_type = type;
}


void B2Sprite::setSpritePosition(Point position){
	Sprite::setPosition(position);
	//update box2d body if any
	if (_body) {
		_body->SetTransform(b2Vec2(
			position.x / PTM_RATIO,
			position.y / PTM_RATIO),
			_body->GetAngle());
	}
}
void B2Sprite::update(float dt){
	if (_body && isVisible()) {
		setPositionX(_body->GetPosition().x * PTM_RATIO);
		setPositionY(_body->GetPosition().y * PTM_RATIO);
		setRotation(CC_RADIANS_TO_DEGREES(-1 * _body->GetAngle()));
	}
}
void B2Sprite::hide(){
	if (_body) {
		_body->SetLinearVelocity(b2Vec2_zero);
		_body->SetAngularVelocity(0);
		_body->SetTransform(b2Vec2_zero, 0.0);
		_body->SetAwake(false);
	}
}
void B2Sprite::reset(){}

//get squared magnitude of sprite's vector
float B2Sprite::mag(){
	if (_body) {
		return pow(_body->GetLinearVelocity().x, 2) +
			pow(_body->GetLinearVelocity().y, 2);
	}
	return 0.0;
}