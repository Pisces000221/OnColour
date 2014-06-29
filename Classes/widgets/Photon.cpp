#include "Photon.h"

int Photon::total = 20116;

void Photon::setVelocity(float v, float directionAngle)
{
    _v = v;
    _direction = directionAngle;
}

void Photon::setColourValue(int val)
{
    _cval = val;
    _sprite->setOpacity(val);
}

int Photon::getColourValue()
{
    return _sprite->getOpacity();
}

void Photon::move(float dt)
{
    setPosition(getPosition() + cocos2d::Vec2(_v * sin(_direction), _v * cos(_direction)) * dt);
}
