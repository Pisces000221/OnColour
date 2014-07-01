#include "SineVelPhoton.h"
#include "Global.h"

cocos2d::Color3B SineVelPhoton::sin_colours[3] = {
    cocos2d::Color3B(255, 255, 0),
    cocos2d::Color3B(0, 255, 255),
    cocos2d::Color3B(255, 0, 255)
};

bool SineVelPhoton::init(float radius, cocos2d::Color3B colour)
{
    if (!Bubble::init(radius, colour)) return false;
    auto sine_p = cocos2d::Sprite::create("images/sine.png");
    sine_p->setNormalizedPosition(cocos2d::Vec2(0.5, 0.5));
    // To prevent sine wave image out-of-bound, do not use x2. Use x1.8- instead.
    // Here we use the golden section.
    sine_p->setScale(radius / sine_p->getContentSize().width * 1.618);
    sine_p->runAction(cocos2d::RepeatForever::create(
        cocos2d::RotateBy::create(1, RAND_BTW(30, 180))));
    this->addChild(sine_p, 5);  // Put on the top
    return true;
}

void SineVelPhoton::move(float dt)
{
    // Do not call Photon::move. We implemented a fresh new one here.
    _moveTime += dt;
    float curPhase = _moveTime;
    while (curPhase > 0) curPhase -= _period;
    curPhase = (curPhase + _period) / _period * 2 * M_PI;
    // I gave up... Creepy integral of sine function
    // We just use infinitesimal method
    setPosition(_position +
        cocos2d::Vec2(sin(_direction), cos(_direction)) * dt * _v
        * (1 + sin(curPhase) * _amplitude));
}

SineVelPhoton *SineVelPhoton::randomGen()
{
    //SineVelPhoton *b = static_cast<SineVelPhoton *>(Photon::randomGen());
    float b_radius = RAND_BTW(onclr::photon_minradius, onclr::photon_maxradius);
    float b_period = RAND_BTW(onclr::sinevel_photon_minperiod, onclr::sinevel_photon_maxperiod);
    float b_amplitude = RAND_BTW(onclr::sinevel_photon_minam, onclr::sinevel_photon_maxam);
    int b_colourval = RAND_BTW_INT(onclr::photon_mincolourval, onclr::photon_maxcolourval);
    int b_colour_idx = rand() % SineVelPhoton::sin_colour_ct;
    float b_hugtime = RAND_RATE(b_radius, 
        onclr::photon_minradius, onclr::photon_maxradius,
        onclr::photon_minhugtime, onclr::photon_maxhugtime);
    SineVelPhoton *b = SineVelPhoton::create(b_radius, SineVelPhoton::sin_colours[b_colour_idx]);
    b->setPeriod(b_period);
    b->setAmplitude(b_amplitude);
    b->setColourValue(b_colourval);
    b->setHugTime(b_hugtime);
    return b;
}
