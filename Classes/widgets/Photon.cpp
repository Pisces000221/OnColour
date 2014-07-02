#include "Photon.h"
#include "Global.h"

int Photon::total = 20116;
cocos2d::Color3B Photon::colours[3] = {
    cocos2d::Color3B(255, 0, 0),
    cocos2d::Color3B(0, 255, 0),
    cocos2d::Color3B(0, 0, 255)
};

Photon::Photon()
: _moveTime(0)
{ }

void Photon::initStartPos()
{
    _startPos = _position;
}

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
    _moveTime += dt;
    setPosition(_startPos +
        cocos2d::Vec2(sin(_direction), cos(_direction)) * _moveTime * _v);
}

Photon *Photon::randomGen()
{
    float radius = PHOTON_RADIUS;
    Photon *b = Photon::create(radius, Photon::colours[rand() % Photon::colour_ct]);
    b->setColourValue(PHOTON_COLOURVAL);
    b->setHugTime(PHOTON_HUGTIME(radius));
    return b;
}
