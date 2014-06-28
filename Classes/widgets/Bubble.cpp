#include "Bubble.h"
using namespace cocos2d;

bool Bubble::init(float radius, cocos2d::Color3B colour)
{
    if (!Node::init()) return false;
    _radius = radius;
    this->setContentSize(Size(radius * 2, radius * 2));
    _sprite = Sprite::create("images/circle.png");
    _sprite->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    _sprite->setPosition(Vec2::ZERO);
    _sprite->setScale(2 * radius / _sprite->getContentSize().width);
    this->addChild(_sprite);
    this->setAnchorPoint(Vec2(0.5, 0.5));
    this->setColor(colour);
    return true;
}

void Bubble::setColor(cocos2d::Color3B colour) { _sprite->setColor(colour); }
