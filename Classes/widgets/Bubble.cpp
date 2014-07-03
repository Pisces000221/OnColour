#include "Bubble.h"
using namespace cocos2d;

bool Bubble::init(float radius, cocos2d::Color3B colour)
{
    if (!Node::init()) return false;
    _radius = radius;
    this->setContentSize(Size(radius * 2, radius * 2));
    this->setCascadeColorEnabled(true);
    this->setCascadeOpacityEnabled(true);
    if (radius <= 48) _sprite = Sprite::create("images/circle.png");
    else if (radius <= 72) _sprite = Sprite::create("images/circle_1.5x.png");
    else if (radius <= 96) _sprite = Sprite::create("images/circle_2x.png");
    else _sprite = Sprite::create("images/circle_3x.png");
    _sprite->setNormalizedPosition(Vec2(0.5, 0.5));
    _sprite->setScale(2 * radius / _sprite->getContentSize().width);
    this->addChild(_sprite);
    this->setAnchorPoint(Vec2(0.5, 0.5));
    this->setColor(colour);
    return true;
}

void Bubble::setRadius(float r)
{
    _sprite->runAction(ScaleBy::create(0.2, r / _radius));
    _radius = r;
}

void Bubble::pop(float delay, float duration, float scale)
{
    auto a = EaseElasticOut::create(ScaleTo::create(duration, scale));
    if (delay > 1e-6)
        this->runAction(Sequence::create(
            DelayTime::create(delay), a, nullptr));
    else
        this->runAction(a);
}
