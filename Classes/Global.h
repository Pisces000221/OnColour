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

extern cocos2d::Size vsize;
void init();
const cocos2d::Size mapsize = cocos2d::Size(1200, 800);
const float photongen_mintime = 3.0f;
const float photongen_maxtime = 6.0f;
const float player_radius = 30.0f;
const float photon_minradius = 36.0f;
const float photon_maxradius = 72.0f;
const int photoncolourct = 3;
const cocos2d::Color3B photoncolours[] = {
    cocos2d::Color3B(255, 0, 0),
    cocos2d::Color3B(0, 255, 0),
    cocos2d::Color3B(0, 0, 255)
};
const int photon_mincolourval = 64;
const int photon_maxcolourval = 255;
const int photon_minvelocity = 30;
const int photon_maxvelocity = 120;
const float photon_minhugtime = 3.0f;
const float photon_maxhugtime = 5.0f;

cocos2d::Label *label(const char *str, int fontsize, bool isbold = false,
    cocos2d::Color3B colour = cocos2d::Color3B::WHITE,
    cocos2d::TextHAlignment alignment = cocos2d::TextHAlignment::LEFT, int linewidth = 0);

}

#endif
