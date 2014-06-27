#include "GameScene.h"
#include "MenuItemLabelTint.h"
#include "Bubble.h"
#include "Global.h"
using namespace cocos2d;

#define FIX_POS(_pos, _min, _max) \
    if (_pos < (_min)) _pos = (_min); else if (_pos > (_max)) _pos = (_max)
#define PLAYER_RADIUS 30

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

    // The player
    auto player = Bubble::create(PLAYER_RADIUS, Color4F(0, 0, 0, 1));
    // Don't use normalized positions here since we need to set absolute position later
    player->setPosition(Vec2(onclr::vsize.width / 2, onclr::vsize.height / 2));
    this->addChild(player);

    // Enable accelerating
    // Copied from cpp-tests (cocos2d-x-3.2alpha0)
    auto listener = EventListenerAcceleration::create([=](Acceleration* acc, Event* event) {
        auto curpos = player->getPosition();
        curpos.x += acc->x * 12.0f;     // TODO: adjust sensitivity in the settings page
        curpos.y += acc->y * 12.0f;     // Maybe sensitivity is between 8 and 24 (decide later)
        FIX_POS(curpos.x, PLAYER_RADIUS, onclr::vsize.width - PLAYER_RADIUS);
        FIX_POS(curpos.y, PLAYER_RADIUS, onclr::vsize.height - PLAYER_RADIUS);
        player->setPosition(curpos);
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, player);

    // Turn on accelerometer
    Device::setAccelerometerEnabled(true);

    return true;
}

// copied from HexBizarre/GameScene
void Gameplay::goBack(Ref *sender)
{
    // Turn off accelerometer
    Device::setAccelerometerEnabled(false);
    // Run animation
    auto cover = LayerColor::create(Color4B::WHITE);
    this->addChild(cover, INT_MAX);
    cover->setOpacity(0);
    cover->runAction(Sequence::create(
        FadeIn::create(0.4),
        CallFunc::create([]() { Director::getInstance()->popScene(); }), nullptr));
}
