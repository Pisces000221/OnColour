#include "BorderedBubble.h"
using namespace cocos2d;

bool BorderedBubble::init(float radius, float borderwidth, cocos2d::Color4F colour)
{
    if (!Bubble::init(radius, colour)) return false;
    auto sprite = Sprite::create("images/circle.png");
    sprite->setColor(Color3B::BLACK);
    _border = ProgressTimer::create(sprite);
    _border->setType(ProgressTimer::Type::RADIAL);
    _border->setNormalizedPosition(Vec2(0.5, 0.5));
    _border->setScale(2 * (radius + borderwidth) / _border->getContentSize().width);
    _border->setPercentage(100);
    this->addChild(_border, -1);
    return true;
}

void BorderedBubble::setBorderProgress(float prog)
{
    _border->setPercentage(prog);
}
