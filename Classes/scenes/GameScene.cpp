#include "GameScene.h"
#include "MenuItemLabelTint.h"
#include "Bubble.h"
#include "Global.h"
using namespace cocos2d;

#define FIX_POS(_pos, _min, _max) \
    if (_pos < (_min)) _pos = (_min); else if (_pos > (_max)) _pos = (_max)
#define PLAYER_RADIUS 30
// maximum supported keys to be pressed at once: 2
#define KEYBOARD_SCHEDULE_KEY "KEYBD_BALLMOVE"
#define KEYBOARD_SCHEDULE_KEY_2 "KEYBD_BALLMOVE_SECOND"

bool Gameplay::init()
{
    if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) return false;

    // The 'back' menu
    auto backItem = MenuItemImage::create(
        "images/back.png", "images/back.png", CC_CALLBACK_1(Gameplay::goBack, this));
    backItem->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backItem->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    auto menu = Menu::create(backItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    // The score displayer
    _scoreDisplayer = onclr::label("0 s", 80, false);
    _scoreDisplayer->setNormalizedPosition(Vec2(0.5, 0.5));
    _scoreDisplayer->setColor(Color3B::BLACK);
    _scoreDisplayer->setOpacity(128);
    this->addChild(_scoreDisplayer);
    _score = 0.0f;

    // The player
    _player = Bubble::create(PLAYER_RADIUS, Color4F(0, 0, 0, 1));
    // Don't use normalized positions here since we need to set absolute position later
    _player->setPosition(Vec2(onclr::vsize.width / 2, onclr::vsize.height / 2));
    this->addChild(_player);

#if IS_ON_PC
    // Enable 'tilting' by keyboard
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [this](EventKeyboard::KeyCode key, Event* event) {
        float x = 0, y = 0;
        if (key == EventKeyboard::KeyCode::KEY_UP_ARROW) y = 1;
        else if (key == EventKeyboard::KeyCode::KEY_DOWN_ARROW) y = -1;
        else if (key == EventKeyboard::KeyCode::KEY_LEFT_ARROW) x = -1;
        else if (key == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) x = 1;
        else return;
        std::string s_key;
        if (this->getScheduler()->isScheduled(KEYBOARD_SCHEDULE_KEY, this)) {
            s_key = KEYBOARD_SCHEDULE_KEY_2;
            pressedKeys[1] = key;
        } else {
            s_key = KEYBOARD_SCHEDULE_KEY;
            pressedKeys[0] = key;
        }
        this->getScheduler()->schedule([=](float dt) {
            this->moveBall(x, y, dt);
        }, this, 0, false, s_key);
    };
    listener->onKeyReleased = [this](EventKeyboard::KeyCode key, Event* event) {
        if (pressedKeys[0] == key)
            this->getScheduler()->unschedule(KEYBOARD_SCHEDULE_KEY, this);
        else
            this->getScheduler()->unschedule(KEYBOARD_SCHEDULE_KEY_2, this);
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _player);
#else   // ohh... a real phone or tablet!
    // Enable accelerating
    // Copied from cpp-tests (cocos2d-x-3.2alpha0)
    auto listener = EventListenerAcceleration::create([this](Acceleration* acc, Event* event) {
        this->moveBall(acc->x, acc->y, 0 /*acc->timestamp * 1e-9*/);
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _player);

    // Turn on accelerometer
    Device::setAccelerometerEnabled(true);
#endif

    // Turn on score scheduler
    this->getScheduler()->schedule(std::bind(&Gameplay::tick, this, std::placeholders::_1),
        this, 1, false, "GAME_SCORE_TICKER");

    return true;
}

void Gameplay::moveBall(float acc_x, float acc_y, float dt)
{
    auto curpos = _player->getPosition();
#if IS_ON_PC
    curpos.x += acc_x * 216.0f * dt;
    curpos.y += acc_y * 216.0f * dt;
#else
    curpos.x += acc_x * 12.0f;      // TODO: adjust sensitivity in the settings page
    curpos.y += acc_y * 12.0f;      // Maybe sensitivity is between 8 and 24 (decide later)
#endif
    FIX_POS(curpos.x, PLAYER_RADIUS, onclr::vsize.width - PLAYER_RADIUS);
    FIX_POS(curpos.y, PLAYER_RADIUS, onclr::vsize.height - PLAYER_RADIUS);
    _player->setPosition(curpos);
}

void Gameplay::tick(float dt)
{
    _score += dt;
    _scoreDisplayer->setString(__String::createWithFormat("%d s", (int)_score)->getCString());
}

// copied from HexBizarre/GameScene
void Gameplay::goBack(Ref *sender)
{
#if !IS_ON_PC
    // Turn off accelerometer
    Device::setAccelerometerEnabled(false);
#endif
    // Run animation
    auto cover = LayerColor::create(Color4B::WHITE);
    this->addChild(cover, INT_MAX);
    cover->setOpacity(0);
    cover->runAction(Sequence::create(
        FadeIn::create(0.4),
        CallFunc::create([]() { Director::getInstance()->popScene(); }), nullptr));
}
