#include "StartupScene.h"
#include "GameScene.h"
#include "PreferenceScene.h"
#include "MenuItemLabelTint.h"
#include "Bubble.h"
#include "Global.h"
using namespace cocos2d;

bool Startup::init()
{
    if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) return false;

    // The title/banner label
    auto titleLabel = onclr::label("On Colour", 60, false);
    titleLabel->setNormalizedPosition(Vec2(0.5, 0.8));
    this->addChild(titleLabel);
    // Random colours
    int c[3] = { 0, 0, 0 };
    for (int i = 0; i < 3; i++) c[i] = rand() % 256;
    titleLabel->runAction(Sequence::create(
        DelayTime::create(1),
        TintTo::create(0.5, 0, 0, 0),
        CallFunc::create([c, titleLabel]() {
            // Tint 'Colour'
            auto letterTintAction = TintTo::create(1, c[0], c[1], c[2]);
            for (int i = 3; i < 9; i++) {
                titleLabel->getLetter(i)->setColor(Color3B::BLACK);
                titleLabel->getLetter(i)->runAction(letterTintAction->clone());
            }
        }), nullptr));

    // The popping bubble
    auto bubble = Bubble::create(20, Color3B(c[0], c[1], c[2]));
    bubble->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    bubble->setPosition(Vec2(onclr::vsize.width - 4, onclr::vsize.height - 4));
    bubble->setScale(0);
    bubble->runAction(Sequence::create(
        DelayTime::create(1.5),
        EaseElasticOut::create(ScaleTo::create(0.7, 1)), nullptr));
    this->addChild(bubble);

    auto newGameMenu = MenuItemLabelTint::create(
        onclr::label("New game", 36, false, Color3B::BLACK), CC_CALLBACK_1(Startup::newGame, this));
    newGameMenu->setNormalizedPosition(Vec2(0.5, 0.618));
    newGameMenu->setOpacity(0);
    auto preferencesMenu = MenuItemLabelTint::create(
        onclr::label("Preferences", 36, false, Color3B::BLACK), CC_CALLBACK_1(Startup::showPreferences, this));
    preferencesMenu->setNormalizedPosition(Vec2(0.5, 0.5));
    preferencesMenu->setOpacity(0);
    auto menu = Menu::create(newGameMenu, preferencesMenu, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    newGameMenu->runAction(Sequence::create(
        DelayTime::create(3),
        FadeIn::create(0.6), nullptr));
    preferencesMenu->runAction(Sequence::create(
        DelayTime::create(3.1),
        FadeIn::create(0.6), nullptr));

    return true;
}

void Startup::newGame(Ref *sender)
{
    GO_TO_SCENE(Gameplay);
}

void Startup::showPreferences(Ref *sender)
{
    GO_TO_SCENE(PreferenceLayer);
}
