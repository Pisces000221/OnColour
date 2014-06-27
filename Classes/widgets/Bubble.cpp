#include "Bubble.h"
using namespace cocos2d;

bool Bubble::init(float radius, cocos2d::Color4F colour)
{
    if (!DrawNode::init()) return false;
    this->setContentSize(Size(radius * 2, radius * 2));
    this->drawDot(Vec2(radius, radius), radius, colour);
    this->setAnchorPoint(Vec2(0.5, 0.5));
    return true;
}
