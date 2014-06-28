#ifndef __OnColour__GameScene_h__
#define __OnColour__GameScene_h__

#include "cocos2d.h"
#include "Global.h"
#include "Bubble.h"
#include "BorderedBubble.h"
#include "Photon.h"

class Gameplay : public cocos2d::Layer
{
public:
    virtual bool init();
    CREATE_FUNC(Gameplay);
    // modified createScene() method
    static cocos2d::Scene* createScene()
    {
        auto scene = cocos2d::Scene::create();
        auto layer = Gameplay::create();
        scene->addChild(layer);
        layer->init2();
        return scene;
    }

protected:
    // Used to add minor widgets like the 'back' menu to the scene
    // Maybe we should use onEnter instead?
    void init2();

    BorderedBubble *_player;
    cocos2d::Vector<Photon *> _photons;
    cocos2d::Label *_scoreDisplayer;
    float _score;
    float _timeToLastPhotonGen;
    cocos2d::EventKeyboard::KeyCode pressedKeys[2];

    void goBack(cocos2d::Ref *sender);
    void moveBall(float acc_x, float acc_y, float dt);
    void tick(float dt);
};

#endif
