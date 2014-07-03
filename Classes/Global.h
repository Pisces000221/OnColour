#ifndef __OnColour__Global_h__
#define __OnColour__Global_h__

#include "cocos2d.h"
#include <string>

namespace onclr {

#define SCENE_FUNC(__type__) \
    static cocos2d::Scene* createScene() \
    { \
        auto scene = cocos2d::Scene::create(); \
        auto layer = __type__::create(); \
        scene->addChild(layer); \
        return scene; \
    }

#define IS_ON_PC (CC_TARGET_PLATFORM == CC_PLATFORM_WINDOWS) \
    || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)

// Used everywhere
#define RAND_0_1 ((float)rand() / RAND_MAX)
#define RAND_BTW(_min, _max) (RAND_0_1 * (_max - _min) + _min)
#define RAND_BTW_INT(_min, _max) (rand() % (_max - _min) + _min)    // [_min, max)
#define RAND_RATE(_val, _min, _max, _min2, _max2) \
    (((_val - _min) / (_max - _min)) * (_max2 - _min2) + _min2)
#define GREY_3B(_g) cocos2d::Color3B(_g, _g, _g)
#define GREY_4B(_g, _a) cocos2d::Color4B(_g, _g, _g, _a)
#define GREY_4F(_g, _a) cocos2d::Color4F(_g, _g, _g, _a)

// Used mainly in GameScene
#define FIX_POS(_pos, _min, _max) \
    if (_pos < (_min)) _pos = (_min); else if (_pos > (_max)) _pos = (_max)
#define ENSURE_UNSCHEDULED(_scheduler, _key) \
    if (_scheduler->isScheduled(_key, this)) _scheduler->unschedule(_key, this)

// Used mainly in StartupScene and in goBack() method
// Requires cocos2d.h and using namespace cocos2d
// Runs an animation to go to the next scene
#define GO_TO_SCENE(__layer_type__) do { \
    Scene *nextScene = __layer_type__::createScene(); \
    Director::getInstance()->pushScene(TransitionFade::create(0.8, nextScene, Color3B::WHITE)); \
    LayerColor *cover = LayerColor::create(Color4B::WHITE); \
    cover->setOpacity(0); \
    this->addChild(cover, INT_MAX); \
    cover->runAction(Sequence::create( \
        DelayTime::create(0.8), \
        CallFunc::create([cover]() { cover->setOpacity(255); }), \
        FadeOut::create(0.4), \
        RemoveSelf::create(), nullptr)); } while (0)
// Runs an animation and go back
#define GO_BACK_ANIMATED do { \
    LayerColor *cover = LayerColor::create(Color4B::WHITE); \
    this->getScene()->addChild(cover, INT_MAX); \
    cover->setOpacity(0); \
    cover->runAction(Sequence::create( \
        FadeIn::create(0.4), \
        CallFunc::create([]() { Director::getInstance()->popScene(); }), nullptr)); } while (0)

// Used in Photon, SineVelPhoton, Bomb
#define PHOTON_RADIUS (RAND_BTW(onclr::photon_minradius, onclr::photon_maxradius) * onclr::bubble_scale)
#define PHOTON_COLOURVAL RAND_BTW_INT(onclr::photon_mincolourval, onclr::photon_maxcolourval)
#define PHOTON_HUGTIME(__radius__) RAND_RATE(__radius__, \
    onclr::photon_minradius, onclr::photon_maxradius, \
    onclr::photon_minhugtime, onclr::photon_maxhugtime)
#define SINPHOTON_PERIOD RAND_BTW(onclr::sinevel_photon_minperiod, onclr::sinevel_photon_maxperiod)
#define SINPHOTON_AMPLITUDE RAND_BTW(onclr::sinevel_photon_minam, onclr::sinevel_photon_maxam)

extern cocos2d::Size vsize;
extern float ratio;
extern float bubble_scale;
void init();
extern const cocos2d::Size mapsize;
extern const float photongen_mintime;
extern const float photongen_maxtime;
extern const float player_radius;
extern const float player_colour_initial;
extern const float player_colour_lost;
extern const float player_colour_warning;
extern const float player_colour_fever;
extern const float photon_minradius;
extern const float photon_maxradius;
extern const int photon_mincolourval;
extern const int photon_maxcolourval;
extern const int photon_minvelocity;
extern const int photon_maxvelocity;
extern const float max_difficulty_rate;
extern const float difficulty_rate_increment;
extern const float photon_minhugtime;
extern const float photon_maxhugtime;
extern const float sinevel_photon_minperiod;
extern const float sinevel_photon_maxperiod;
extern const float sinevel_photon_minam;
extern const float sinevel_photon_maxam;
extern const float normal_photon_possib;
extern const float sinevel_photon_possib;
extern const float bomb_possib;

cocos2d::Label *label(std::string str, int fontsize, bool isbold = false,
    cocos2d::Color3B colour = cocos2d::Color3B::WHITE,
    cocos2d::TextHAlignment alignment = cocos2d::TextHAlignment::LEFT, int linewidth = 0);

}

#endif
