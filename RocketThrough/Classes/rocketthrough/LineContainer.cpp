#include "LineContainer.h"

using namespace cocos2d;

LineContainer::LineContainer() {
	_dash = 10;
	_dashSpace = 10;

	//initial line type
	_lineType = LINE_NONE;

	_screenSize = Director::getInstance()->getWinSize();
	//position the bar on the right side 
	_energyLineX = _screenSize.width * 0.95f;

	//total height of the bar = 80% of the screen height
	_energyHeight = _screenSize.height * 0.8f;

	glLineWidth(8.0 * Director::getInstance()->getContentScaleFactor());

	this->reset();
}

LineContainer* LineContainer::create() {
	LineContainer * node = new LineContainer();
	if (node) {
		
		node->autorelease();
		return node;
	}
	CC_SAFE_DELETE(node);
	return NULL;
}

void LineContainer::reset() {
	_energy = 1.0;
	_energyDecrement = 0.015f;
}

void LineContainer::update(float dt) {
	_energy -= dt * _energyDecrement;
	if (_energy < 0) _energy = 0;
}

void LineContainer::setEnergyDecrement(float value) {
	_energyDecrement += value;
	//if (_energyDecrement > 0.07) _energyDecrement = 0.07;
}

void LineContainer::drawEnergyBar(const kmMat4 &transform, uint32_t flags)
{
	//not sure if necessary
	//kmGLPushMatrix();
	//kmGLLoadMatrix(&transform);
	
	//draw energy bar
	DrawPrimitives::setDrawColor4F(0.1,0.1,0.1,1);//light gray
	DrawPrimitives::drawLine(Point(_energyLineX, _screenSize.height * 0.1f),
		Point(_energyLineX, _screenSize.height * 0.9f));
	
	//enrgy line  = 80% of the screen height	
	DrawPrimitives::setDrawColor4F(Color4F::ORANGE.r, Color4F::ORANGE.g, Color4F::ORANGE.b, Color4F::ORANGE.a);
	DrawPrimitives::drawLine(Point(_energyLineX, _screenSize.height * 0.1f),Point(_energyLineX, _screenSize.height * 0.1f + _energy * _energyHeight));
}

void LineContainer::drawPivotLine(const kmMat4 &transform, uint32_t flags)
{
	//not sure if necessary
	//kmGLPushMatrix();
	//kmGLLoadMatrix(&transform);
	switch (_lineType) {
	case LINE_NONE:
		break;
	case LINE_TEMP:
		//_tip should be the rocket sprite (tip of the line) and the pivot the bottom of the line
		DrawPrimitives::setDrawColor4F(1.0, 1.0, 1.0, 1.0);
		DrawPrimitives::drawLine(_tip, _pivot);
		DrawPrimitives::drawCircle(_pivot, 10, CC_DEGREES_TO_RADIANS(360), 10, false);

		break;

	case LINE_DASHED://linea punteada
		DrawPrimitives::setDrawColor4F(1.0, 1.0, 1.0, 1.0);
		DrawPrimitives::drawCircle(_pivot, 10, M_PI, 10, false);

		int segments = _lineLength / (_dash + _dashSpace);

		float t = 0.0f;
		float x_;
		float y_;

		for (int i = 0; i < segments + 1; i++) {

			x_ = _pivot.x + t * (_tip.x - _pivot.x);
			y_ = _pivot.y + t * (_tip.y - _pivot.y);

			DrawPrimitives::drawCircle(Point(x_, y_), 4, M_PI, 6, false);

			t += 1.0f / segments;
		}
		break;
	}
}


/*
In future, cocos2d-x 3.x renderer will be multithreaded with command pool.
draw method called by visit method, to create new command. When command 
is performed by command pool, onDraw is called.
At this moment, commands are performed in single thread,
but in overloaded onDraw method you should assume, that it will be called in another thread to simplify future migration.

*/
void LineContainer::draw(Renderer *renderer, const Mat4& transform, uint32_t flags){
	//_customEnergyBar.init(5); use this or indicate when adding to a layer
	_customEnergyBar.func = CC_CALLBACK_0(LineContainer::drawEnergyBar, this, transform, flags);
	_customPivotLine.func = CC_CALLBACK_0(LineContainer::drawPivotLine, this, transform, flags);
	renderer->addCommand(&_customEnergyBar);
	renderer->addCommand(&_customPivotLine);
	
}

