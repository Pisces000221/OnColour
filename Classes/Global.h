#ifndef __OnColour__Global_h__
#define __OnColour__Global_h__

#include "cocos2d.h"

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

// Used mainly in GameScene
#define FIX_POS(_pos, _min, _max) \
    if (_pos < (_min)) _pos = (_min); else if (_pos > (_max)) _pos = (_max)
#define ENSURE_UNSCHEDULED(_scheduler, _key) \
    if (_scheduler->isScheduled(_key, this)) _scheduler->unschedule(_key, this)

extern cocos2d::Size vsize;
void init();
extern const cocos2d::Size mapsize;
extern const float photongen_mintime;
extern const float photongen_maxtime;
extern const float player_radius;
extern const float player_colour_lost;
extern const float player_colour_warning;
extern const float photon_minradius;
extern const float photon_maxradius;
extern const int photon_mincolourval;
extern const int photon_maxcolourval;
extern const int photon_minvelocity;
extern const int photon_maxvelocity;
extern const float photon_minhugtime;
extern const float photon_maxhugtime;
extern const float sinevel_photon_minperiod;
extern const float sinevel_photon_maxperiod;
extern const float sinevel_photon_minam;
extern const float sinevel_photon_maxam;
extern const float normal_photon_possib;
extern const float sinevel_photon_possib;

cocos2d::Label *label(const char *str, int fontsize, bool isbold = false,
    cocos2d::Color3B colour = cocos2d::Color3B::WHITE,
    cocos2d::TextHAlignment alignment = cocos2d::TextHAlignment::LEFT, int linewidth = 0);

}

#endif
