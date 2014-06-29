#include "BorderedBubble.h"
using namespace cocos2d;

bool BorderedBubble::init(float radius, float borderwidth, cocos2d::Color3B colour)
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
    _border->setPercentage(prog * 100);
}

void BorderedBubble::reset(float anim_time)
{
    float startProg = _border->getPercentage() / 100.0;
    anim_time *= (1 - startProg);
    _animPastTime = 0;
    this->getScheduler()->schedule([&, anim_time, startProg](float dt) {
        _animPastTime += dt;
        if (_animPastTime >= anim_time) {
            _animPastTime = anim_time;
            this->getScheduler()->unschedule("B.BUBBLE_RESET_TICK", this);
        }
        this->setBorderProgress(startProg + (1 - startProg) * _animPastTime / anim_time);
    }, this, 0, false, "B.BUBBLE_RESET_TICK");
}
