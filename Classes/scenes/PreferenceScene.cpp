#include "PreferenceScene.h"
#include "Global.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;

#define MIN_SENSITIVITY 0.5
#define MAX_SENSITIVITY 1.5

bool PreferenceLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) return false;

    // Read previous set values
    auto ud = UserDefault::getInstance();
    _sliderValues[0] = ud->getFloatForKey("Senvitivity", 1);
    _sliderValues[1] = ud->getFloatForKey("Sound_Vol", 1);
    _sliderValues[2] = ud->getFloatForKey("FX_Vol", 1);
    CCLOG("sensitivity, soundvol, fxvol = %f, %f, %f",
        _sliderValues[0], _sliderValues[1], _sliderValues[2]);

    // WTF... we create a Scale9Sprite, and we're ready to link?!!!
    // Ehh... I think GCC has ignored that for us unless we use it...
    auto a = extension::Scale9Sprite::create();
    auto backItem = MenuItemImage::create(
        "images/back.png", "images/back.png",
        CC_CALLBACK_1(PreferenceLayer::goBack, this));
    backItem->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backItem->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    auto menu = Menu::create(backItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 6);

    auto title = onclr::label("Preferences", 48, true);
    title->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    title->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
    title->setColor(Color3B::BLACK);
    title->setOpacity(0);
    this->addChild(title);
    title->runAction(Sequence::create(
        DelayTime::create(0.8),
        FadeIn::create(0.3), nullptr));

    // Ball speed on PC and tilt sensitivity on tablets and phones
#if IS_ON_PC
    auto label_1 = onclr::label("Ball speed", 28);
#else
    auto label_1 = onclr::label("Tilt sensitivity", 28);
#endif
    label_1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label_1->setPosition(Vec2(6, onclr::vsize.height - 88));
    label_1->setColor(Color3B::BLACK);
    this->addChild(label_1);
    auto slider_1 = ui::Slider::create();
    slider_1->setColor(Color3B(0xfc, 0x25, 0xef));
    slider_1->loadBarTexture("images/slider_bar.png");
    slider_1->loadSlidBallTextures("images/circle.png", "images/circle.png", "");
    slider_1->loadProgressBarTexture("images/slider_progress.png");
    slider_1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    slider_1->setPosition(Vec2(onclr::vsize.width - 24, onclr::vsize.height - 88));
    slider_1->setScale(0.85);
    slider_1->setPercent(RAND_RATE(
            _sliderValues[0], MIN_SENSITIVITY, MAX_SENSITIVITY, 0.0, 100.0));
    slider_1->addEventListener([this](Ref *sender, ui::Slider::EventType type) {
        _sliderValues[0] = RAND_RATE(
            ((ui::Slider *)sender)->getPercent(),
            0.0, 100.0, MIN_SENSITIVITY, MAX_SENSITIVITY);
    });
    this->addChild(slider_1);

    // Music volume
    auto label_2 = onclr::label("Music volume", 28);
    label_2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label_2->setPosition(Vec2(6, onclr::vsize.height - 128));
    label_2->setColor(Color3B::BLACK);
    this->addChild(label_2);
    auto slider_2 = ui::Slider::create();
    slider_2->setColor(Color3B(0x25, 0xfc, 0x80));
    slider_2->loadBarTexture("images/slider_bar.png");
    slider_2->loadSlidBallTextures("images/circle.png", "images/circle.png", "");
    slider_2->loadProgressBarTexture("images/slider_progress.png");
    slider_2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    slider_2->setPosition(Vec2(onclr::vsize.width - 24, onclr::vsize.height - 128));
    slider_2->setScale(0.85);
    slider_2->setPercent(_sliderValues[1] * 100.0);
    slider_2->addEventListener([this](Ref *sender, ui::Slider::EventType type) {
        _sliderValues[1] = ((ui::Slider *)sender)->getPercent() / 100.0;
    });
    this->addChild(slider_2);
    // Effects' volume
    auto label_3 = onclr::label("FX volume", 28);
    label_3->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label_3->setPosition(Vec2(6, onclr::vsize.height - 168));
    label_3->setColor(Color3B::BLACK);
    this->addChild(label_3);
    auto slider_3 = ui::Slider::create();
    slider_3->setColor(Color3B(0x25, 0x80, 0xfc));
    slider_3->loadBarTexture("images/slider_bar.png");
    slider_3->loadSlidBallTextures("images/circle.png", "images/circle.png", "");
    slider_3->loadProgressBarTexture("images/slider_progress.png");
    slider_3->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    slider_3->setPosition(Vec2(onclr::vsize.width - 24, onclr::vsize.height - 168));
    slider_3->setScale(0.85);
    slider_3->setPercent(_sliderValues[2] * 100.0);
    slider_3->addEventListener([this](Ref *sender, ui::Slider::EventType type) {
        _sliderValues[2] = ((ui::Slider *)sender)->getPercent() / 100.0;
    });
    this->addChild(slider_3);

    return true;
}

void PreferenceLayer::goBack(Ref *sender)
{
    auto ud = UserDefault::getInstance();
    ud->setFloatForKey("Senvitivity", _sliderValues[0]);
    ud->setFloatForKey("Sound_Vol", _sliderValues[1]);
    ud->setFloatForKey("FX_Vol", _sliderValues[2]);
    ud->flush();
    GO_BACK_ANIMATED;
}
