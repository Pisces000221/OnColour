#include "ToggleBubble.h"
using namespace cocos2d;

bool ToggleBubble::init(float radius, Color3B colour, const ccMenuCallback &callback)
{
    if (!Bubble::init(radius, colour)) return false;
    if (!callback) return false;

    _initialColour = colour;
    _call = callback;
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = [this](Touch *touch, Event *event) {
        Vec2 p = convertTouchToNodeSpace(touch);
        bool thisTouchIsInside =
            Rect(0, 0, _contentSize.width, _contentSize.height).containsPoint(p);
        if (thisTouchIsInside) {
            _initialScale = this->getScale();
            this->runAction(ScaleTo::create(0.1f, _initialScale * 0.8f));
            _lastTouchIsInside = true;
        }
        return thisTouchIsInside;
    };
    listener->onTouchMoved = [this](Touch *touch, Event *event) {
        Vec2 p = convertTouchToNodeSpace(touch);
        bool thisTouchIsInside =
            Rect(0, 0, _contentSize.width, _contentSize.height).containsPoint(p);
        if (_lastTouchIsInside && !thisTouchIsInside) {
            this->runAction(ScaleTo::create(0.1f, _initialScale));
            _lastTouchIsInside = false;
        } else if (!_lastTouchIsInside && thisTouchIsInside) {
            this->runAction(ScaleTo::create(0.1f, _initialScale * 0.8f));
            _lastTouchIsInside = true;
        }
    };
    listener->onTouchEnded = [this](Touch *touch, Event *event) {
        Vec2 p = convertTouchToNodeSpace(touch);
        bool thisTouchIsInside =
            Rect(0, 0, _contentSize.width, _contentSize.height).containsPoint(p);
        if (thisTouchIsInside) {
            this->pop(0, 0.7, _initialScale);
            this->setOn(!_on);
            _call(this);
        }
    };
    listener->onTouchCancelled = [this](Touch *touch, Event *event) {
        this->runAction(ScaleTo::create(0.1f, _initialScale));
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    return true;
}

void ToggleBubble::setOn(bool on)
{
    _on = on;
    Color3B c = on ? _initialColour : Color3B(192, 192, 192);
    _sprite->runAction(TintTo::create(0.1f, c.r, c.g, c.b));
}

void ToggleBubble::setColor(cocos2d::Color3B colour)
{
    Bubble::setColor(colour);
    _initialColour = colour;
}
