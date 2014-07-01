#include "Bomb.h"
#include "Global.h"

bool Bomb::init(float radius)
{
    if (!Photon::init(radius, cocos2d::Color3B::BLACK)) return false;
    auto bomb_p = cocos2d::Sprite::create("images/warning.png");
    bomb_p->setNormalizedPosition(cocos2d::Vec2(0.5, 0.5));
    bomb_p->setScale(radius / bomb_p->getContentSize().width * 1.44);
    bomb_p->runAction(cocos2d::RepeatForever::create(
        cocos2d::RotateBy::create(1, RAND_BTW(30, 180))));
    this->addChild(bomb_p, 5);
    return true;
}

Bomb *Bomb::randomGen()
{
    float b_radius = RAND_BTW(onclr::photon_minradius, onclr::photon_maxradius);
    Bomb *b = Bomb::create(b_radius * 0.5); // bombs cannot be TOO large
    return b;
}
