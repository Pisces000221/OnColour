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
        scene->addChild(layer, 4);
        layer->init2();
        return scene;
    }

protected:
    // Used to add minor widgets like the 'back' menu to the scene
    // Maybe we should use onEnter instead?
    void init2();
    // Randomly generates a photon
    void generatePhoton();
    void removePhoton(Photon *photon);
    void movePhotonsAndShowPointers(float dt);
    void checkHugs(float dt);
    bool huggy(Photon *photon);
    bool semi_huggy(Photon *photon);
    void checkFever();

    bool _gamePaused;
    BorderedBubble *_player;
    cocos2d::Vector<Photon *> _photons;
    cocos2d::Label *_scoreDisplayer;
    cocos2d::Label *_warner[3];
    bool _warnerShown[3];
    bool _warnerPlaceAvailable[3];
    static const cocos2d::Color3B _warnColours[3];
    static const char *_warnMessage[3];
    float _score;
    float _timeToLastPhotonGen;
    float _sensitivity;
    cocos2d::EventKeyboard::KeyCode pressedKeys[2];
    cocos2d::EventListenerTouchOneByOne *_touchListener;

    // Player-related data
    int _photonHugID;
    float _photonHugTime;
    float _r, _g, _b;
    bool _isInFeverMode;

    void goBack(cocos2d::Ref *sender);
    void pauseOrResume();
    void endGame();
    void moveBall(float acc_x, float acc_y, float dt);
    void tick(float dt);
    cocos2d::Vec2 getWarnerPlaceAvailable();
    void freeWarnerPlace(cocos2d::Vec2 p);
};

#endif
