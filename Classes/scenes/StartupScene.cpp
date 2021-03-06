#include "StartupScene.h"
#include "GameScene.h"
#include "PreferenceScene.h"
#include "TutorialScene.h"
#include "AboutScene.h"
#include "MenuItemLabelTint.h"
#include "Bubble.h"
#include "Global.h"
using namespace cocos2d;

bool Startup::init()
{
    if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) return false;
    float s_ratio = (onclr::ratio - 1) * 0.4 + 1;

    // The title/banner label
    auto titleLabel = onclr::label("On Colour", 60 * s_ratio, false);
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
    auto bubble = Bubble::create(20 * onclr::ratio, Color3B(c[0], c[1], c[2]));
    bubble->setAnchorPoint(Vec2(0, 0.7));
    Vec2 titlePos = titleLabel->getNormalizedPosition();
    Size titleSize = titleLabel->getContentSize();
    bubble->setPosition(Vec2(titlePos.x * onclr::vsize.width + titleSize.width / 2 + 6,
        titlePos.y * onclr::vsize.height + titleSize.height / 2 - 1));
    bubble->setScale(0);
    bubble->pop(1.5);
    this->addChild(bubble);

    auto newGameMenu = MenuItemLabelTint::create(
        onclr::label("New game", 36 * s_ratio, false, Color3B::BLACK), CC_CALLBACK_1(Startup::newGame, this));
    newGameMenu->setNormalizedPosition(Vec2(0.5, 0.618));
    newGameMenu->setOpacity(0);
    auto preferencesMenu = MenuItemLabelTint::create(
        onclr::label("Preferences", 36 * s_ratio, false, Color3B::BLACK), CC_CALLBACK_1(Startup::showPreferences, this));
    preferencesMenu->setNormalizedPosition(Vec2(0.5, 0.5));
    preferencesMenu->setOpacity(0);
    auto tutorialMenu = MenuItemLabelTint::create(
        onclr::label("How to play", 36 * s_ratio, false, Color3B::BLACK), CC_CALLBACK_1(Startup::showTutorial, this));
    tutorialMenu->setNormalizedPosition(Vec2(0.5, 0.382));
    tutorialMenu->setOpacity(0);
    auto aboutMenu = MenuItemLabelTint::create(
        onclr::label("About", 36 * s_ratio, false, Color3B::BLACK), CC_CALLBACK_1(Startup::showAbout, this));
    aboutMenu->setNormalizedPosition(Vec2(0.5, 0.264));
    aboutMenu->setOpacity(0);
    auto menu = Menu::create(newGameMenu, preferencesMenu, tutorialMenu, aboutMenu, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    newGameMenu->runAction(FADE_IN_DELAY(3, 0.6));
    preferencesMenu->runAction(FADE_IN_DELAY(3.1, 0.6));
    tutorialMenu->runAction(FADE_IN_DELAY(3.2, 0.6));
    aboutMenu->runAction(FADE_IN_DELAY(3.3, 0.6));

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

void Startup::showTutorial(Ref *sender)
{
    GO_TO_SCENE(Tutorial);
}

void Startup::showAbout(Ref *sender)
{
    GO_TO_SCENE(About);
}
