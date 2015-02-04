#ifndef __LINECONTAINER_H__
#define __LINECONTAINER_H__


#include "cocos2d.h"

using namespace cocos2d;

typedef enum lineTypes{

	LINE_TEMP,
	LINE_DASHED,
	LINE_NONE

} LineType;


class LineContainer : public Node {

public:
    
    CC_SYNTHESIZE(float, _energy, Energy);
	CC_SYNTHESIZE(Point, _pivot, Pivot);
	CC_SYNTHESIZE(Point, _tip, Tip);
	CC_SYNTHESIZE(float, _lineLength, LineLength);
	CC_SYNTHESIZE(LineType, _lineType, LineType);
	
    LineContainer();
	static LineContainer * create();

    void update (float dt);
    void setEnergyDecrement(float value);
    void reset (void);


private:
	
	float _lineAngle;
    float _energyLineX;
    float _energyHeight;
    float _energyDecrement;
    
    int _dash;
    int _dashSpace;
	Size _screenSize;

	void draw(Renderer *renderer, const Mat4& transform, uint32_t flags);
	void drawEnergyBar(const Mat4 &transform, uint32_t flags);
	void drawPivotLine(const Mat4 &transform, uint32_t flags);
	CustomCommand _customEnergyBar;
	CustomCommand _customPivotLine;
	

};

#endif // __LINECONTAINER_H__

		
		