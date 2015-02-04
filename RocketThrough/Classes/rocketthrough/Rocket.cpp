#include "Rocket.h"


Rocket::Rocket() :
_targetRotation(0),
_dr(0),
_ar(0),
_vr(0),
_rotationSpring(0.2f),
_rotationDamping(0.5f),
_pivot(Point(-1, -1)),
_rotationOrientation(ROTATE_NONE)
{
	this->reset();
}

Rocket::~Rocket(){}

Rocket* Rocket::create() {
	Rocket * sprite = new Rocket();

	if (sprite && sprite->initWithSpriteFrameName("rocket.png")) {
		sprite->autorelease();
		sprite->setRadius(sprite->getBoundingBox().size.height * 0.5f);
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return NULL;
}


void Rocket::reset() {

	_speed = 50;
	_pivot = Point(-1, 1);
	_rotationOrientation = ROTATE_NONE;
	this->setRotation(-90);
	_targetRotation = -90;
	float angle = CC_DEGREES_TO_RADIANS(this->getRotation());
	_vector = Point(_speed * cos(angle),
		-_speed * sin(angle));

}


void Rocket::update(float dt) {
	Point position = this->getPosition();
	if (_rotationOrientation == ROTATE_NONE){
		position.x += _vector.x * dt;
		position.y += _vector.y * dt;
	}
	else
	{
		//rotate point arround a pivot by a certain amount
		Point rotatedPoint = position.rotateByAngle(_pivot, _angularSpeed * dt);
		position = rotatedPoint;
		float rotatedAngle;
		Point clockwise = (position - _pivot).getPerp();
		if (_rotationOrientation == ROTATE_COUNTER){
			rotatedAngle = atan2(-1 * clockwise.y, -1 * clockwise.x);
		}
		else{
			rotatedAngle = atan2(clockwise.y, clockwise.x);
		}

		//update rocket vector
		_vector.x = _speed * cos(rotatedAngle);
		_vector.y = _speed*sin(rotatedAngle);
		this->setRotationFromVector();


		if (this->getRotation() > 0){
			this->setRotation(fmod(this->getRotation(), -360.0f));
		}
		else{
			this->setRotation(fmod(this->getRotation(), 360.0f));
		}
		if (_targetRotation > this->getRotation() + 180){
			_targetRotation -= 360;
		}
		if (_targetRotation > this->getRotation() - 180){
			_targetRotation += 360;
		}

		_dr = _targetRotation - this->getRotation();
		_ar = _dr * _rotationSpring;
		_vr += _ar;
		_vr *= _rotationDamping;
		this->setRotation(this->getRotation() + _vr);

	}


	this->setPosition(position);



}

void Rocket::select(bool flag) {
	if (flag) {
		this->setSpriteFrame(CCSpriteFrameCache::getInstance()->getSpriteFrameByName("rocket_on.png"));
	}
	else {
		this->setSpriteFrame(CCSpriteFrameCache::getInstance()->getSpriteFrameByName("rocket.png"));
	}
}

bool Rocket::collidedWithSides() {

	Size screenSize = Director::getInstance()->getWinSize();

	auto m_tPosition = this->getPosition();

	if (m_tPosition.x > screenSize.width - _radius) {
		m_tPosition.x = screenSize.width - _radius;
		_rotationOrientation = ROTATE_NONE;
		_vector = Point(this->getVector().x * -1, this->getVector().y);
		this->setRotationFromVector();
		return true;
	}

	if (m_tPosition.x < _radius) {
		m_tPosition.x = _radius;
		_rotationOrientation = ROTATE_NONE;
		_vector = Point(this->getVector().x * -1, this->getVector().y);
		this->setRotationFromVector();
		return true;

	}

	if (m_tPosition.y < _radius) {
		m_tPosition.y = _radius;
		_rotationOrientation = ROTATE_NONE;
		_vector = Point(this->getVector().x, this->getVector().y * -1);
		this->setRotationFromVector();
		return true;
	}

	if (m_tPosition.y > screenSize.height - _radius) {
		m_tPosition.y = screenSize.height - _radius;
		_rotationOrientation = ROTATE_NONE;
		_vector = Point(this->getVector().x, this->getVector().y * -1);
		this->setRotationFromVector();
		return true;
	}


	return false;
}
