#include "PreferenceScene.h"
#include "Global.h"
#include "ToggleBubble.h"
#include "MenuItemLabelTint.h"
#include "ConvenientScrollView.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"
using namespace cocos2d;

#define SCHEDULE_SAVE_KEY "PREFERENCE_SCHEDULE_SAVE"

#define MIN_SENSITIVITY 0.5
#define MAX_SENSITIVITY 1.5

#define INIT_SLIDER_TEXTURES(_s) do { \
    _s->loadBarTexture("images/slider_bar.png"); \
    _s->loadSlidBallTextures("images/circle.png", "images/circle.png", ""); \
    _s->loadProgressBarTexture("images/slider_progress.png"); } while (0)

bool PreferenceLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(255, 255, 255, 255))) return false;
    float s_ratio = (onclr::ratio - 1) * 0.7 + 1;

    // Create the scroll view
    // ui::ScrollView is REALLY creepy... I don't know how to use it.
    auto scroll = Layer::create();
    this->addChild(scroll);

    // Read previous set values
    auto ud = UserDefault::getInstance();
    _sliderValues[0] = ud->getFloatForKey("Senvitivity", 1);
    _sliderValues[1] = ud->getFloatForKey("Sound_Vol", 1);
    _sliderValues[2] = ud->getFloatForKey("FX_Vol", 1);
    _toggleValues[0] = ud->getBoolForKey("Comfortable_Pos_Mode", false);
    _toggleValues[1] = ud->getBoolForKey("Debug_Info_Vis", true);
    // cpp-tests/CurrentLanguageTest/CurrentLanguageTest.cpp (12)
    _lang = ud->getIntegerForKey("Language",
        (int)(Application::getInstance()->getCurrentLanguage()));
    CCLOG("sensitivity, soundvol, fxvol = %f, %f, %f",
        _sliderValues[0], _sliderValues[1], _sliderValues[2]);

    // WTF... we create a Scale9Sprite, and we're ready to link?!!!
    // Ehh... I think GCC has ignored that for us unless we use it...
    auto a = extension::Scale9Sprite::create();
    CREATE_GO_BACK_MENUITEM(PreferenceLayer, backItem);
    auto menu = Menu::create(backItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 6);

    auto title = onclr::label("Preferences", 48 * s_ratio, true);
    title->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
    title->setNormalizedPosition(Vec2::ANCHOR_TOP_RIGHT);
    title->setColor(Color3B::BLACK);
    title->setOpacity(0);
    this->addChild(title);
    title->runAction(FADE_IN_DELAY(0.8, 0.3));

    // Ball speed on PC and tilt sensitivity on tablets and phones
#if IS_ON_PC
    auto label_1 = onclr::label("Ball speed", 28 * s_ratio);
#else
    auto label_1 = onclr::label("Tilt sensitivity", 28 * s_ratio);
#endif
    label_1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label_1->setPosition(Vec2(6, onclr::vsize.height - 88 * s_ratio));
    label_1->setColor(Color3B::BLACK);
    scroll->addChild(label_1);
    auto slider_1 = ui::Slider::create();
    slider_1->setColor(Color3B(0xfc, 0x25, 0xef));
    INIT_SLIDER_TEXTURES(slider_1);
    slider_1->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    slider_1->setPosition(Vec2(onclr::vsize.width - 24 * s_ratio,
        onclr::vsize.height - 88 * s_ratio));
    slider_1->setScale(0.85 * s_ratio);
    slider_1->setPercent(RAND_RATE(
            _sliderValues[0], MIN_SENSITIVITY, MAX_SENSITIVITY, 0.0, 100.0));
    slider_1->addEventListener([this](Ref *sender, ui::Slider::EventType type) {
        _sliderValues[0] = RAND_RATE(
            ((ui::Slider *)sender)->getPercent(),
            0.0, 100.0, MIN_SENSITIVITY, MAX_SENSITIVITY);
        this->save();
    });
    scroll->addChild(slider_1);
    label_1->setOpacity(0);
    slider_1->setOpacity(0);
    label_1->runAction(FADE_IN_DELAY(1.1, 0.3));
    slider_1->runAction(FADE_IN_DELAY(1.1, 0.3));

    // Music volume
    auto label_2 = onclr::label("Music volume", 28 * s_ratio);
    label_2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label_2->setPosition(Vec2(6, onclr::vsize.height - 128 * s_ratio));
    label_2->setColor(Color3B::BLACK);
    scroll->addChild(label_2);
    auto slider_2 = ui::Slider::create();
    slider_2->setColor(Color3B(0x25, 0xfc, 0x80));
    INIT_SLIDER_TEXTURES(slider_2);
    slider_2->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    slider_2->setPosition(Vec2(onclr::vsize.width - 24 * s_ratio,
        onclr::vsize.height - 128 * s_ratio));
    slider_2->setScale(0.85 * s_ratio);
    slider_2->setPercent(_sliderValues[1] * 100.0);
    slider_2->addEventListener([this](Ref *sender, ui::Slider::EventType type) {
        _sliderValues[1] = ((ui::Slider *)sender)->getPercent() / 100.0;
        this->save();
    });
    scroll->addChild(slider_2);
    label_2->setOpacity(0);
    slider_2->setOpacity(0);
    label_2->runAction(FADE_IN_DELAY(1.25, 0.3));
    slider_2->runAction(FADE_IN_DELAY(1.25, 0.3));

    // Effects' volume
    auto label_3 = onclr::label("FX volume", 28 * s_ratio);
    label_3->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label_3->setPosition(Vec2(6, onclr::vsize.height - 168 * s_ratio));
    label_3->setColor(Color3B::BLACK);
    scroll->addChild(label_3);
    auto slider_3 = ui::Slider::create();
    slider_3->setColor(Color3B(0x25, 0x80, 0xfc));
    INIT_SLIDER_TEXTURES(slider_3);
    slider_3->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);
    slider_3->setPosition(Vec2(onclr::vsize.width - 24 * s_ratio,
        onclr::vsize.height - 168 * s_ratio));
    slider_3->setScale(0.85 * s_ratio);
    slider_3->setPercent(_sliderValues[2] * 100.0);
    slider_3->addEventListener([this](Ref *sender, ui::Slider::EventType type) {
        _sliderValues[2] = ((ui::Slider *)sender)->getPercent() / 100.0;
    });
    scroll->addChild(slider_3);
    label_3->setOpacity(0);
    slider_3->setOpacity(0);
    label_3->runAction(FADE_IN_DELAY(1.4, 0.3));
    slider_3->runAction(FADE_IN_DELAY(1.4, 0.3));

    auto resetMenu = MenuItemLabelTint::create(
        onclr::label("Reset to default", 32 * s_ratio, false, Color3B::BLACK),
        [this, slider_1, slider_2, slider_3] (Ref *sender) {
            slider_1->setPercent(50); _sliderValues[0] = 1;
            slider_2->setPercent(100); _sliderValues[1] = 1;
            slider_3->setPercent(100); _sliderValues[2] = 1;
            this->save();
        });
    resetMenu->setPosition(Vec2(onclr::vsize.width * 0.5,
        onclr::vsize.height - 218 * s_ratio));
    resetMenu->setOpacity(0);
    resetMenu->runAction(FADE_IN_DELAY(1.6, 0.3));
    // Reuse 'menu', protect the enviRAMent
    menu = Menu::create(resetMenu, nullptr);
    menu->setPosition(Vec2::ZERO);
    scroll->addChild(menu);

    float bubble_w = 0.85 * s_ratio * 20;
    // Toggle between top-down mode and comfortable-position mode
#if IS_ON_PC
    auto label_4 = onclr::label("Comfortable position (not on laptops)", 28 * s_ratio);
    label_4->setColor(GREY_3B(128));
#else
    auto label_4 = onclr::label("Comfortable position", 28 * s_ratio);
    label_4->setColor(Color3B::BLACK);
#endif
    label_4->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label_4->setPosition(Vec2(6, onclr::vsize.height - 268 * s_ratio));
    scroll->addChild(label_4);
    label_4->setOpacity(0);
    label_4->runAction(FADE_IN_DELAY(1.8, 0.3));
    auto toggle_4 = ToggleBubble::create(bubble_w, Color3B(255, 128, 32),
        [this](Ref *sender) {
#if IS_ON_PC
            // Comfortable position, that's not for laptops
            _toggleValues[0] = false;
            static_cast<ToggleBubble *>(sender)->setOn(false);
#else
            _toggleValues[0] = static_cast<ToggleBubble *>(sender)->isOn();
            this->save();
#endif
        });
    toggle_4->setPosition(Vec2(label_4->getContentSize().width + bubble_w + 12,
        onclr::vsize.height - 268 * s_ratio));
    toggle_4->setOn(_toggleValues[0]);
    toggle_4->setOpacity(0);
    toggle_4->runAction(FADE_IN_DELAY(1.8, 0.3));
    scroll->addChild(toggle_4);

    // Toggle debug information visibility
    auto label_5 = onclr::label("Debug information", 28 * s_ratio);
    label_5->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label_5->setPosition(Vec2(6, onclr::vsize.height - 308 * s_ratio));
    label_5->setColor(Color3B::BLACK);
    scroll->addChild(label_5);
    label_5->setOpacity(0);
    label_5->runAction(FADE_IN_DELAY(1.95, 0.3));
    auto toggle_5 = ToggleBubble::create(bubble_w, Color3B(255, 255, 0),
        [this](Ref *sender) {
            _toggleValues[1] = static_cast<ToggleBubble *>(sender)->isOn();
            Director::getInstance()->setDisplayStats(_toggleValues[1]);
            this->save();
        });
    toggle_5->setPosition(Vec2(label_5->getContentSize().width + bubble_w + 12,
        onclr::vsize.height - 308 * s_ratio));
    toggle_5->setOn(_toggleValues[1]);
    toggle_5->setOpacity(0);
    toggle_5->runAction(FADE_IN_DELAY(1.95, 0.3));
    scroll->addChild(toggle_5);

    // Select language
    auto label_6 = onclr::label("Language (after restart): ", 28 * s_ratio);
    label_6->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    label_6->setPosition(Vec2(6, onclr::vsize.height - 368 * s_ratio));
    label_6->setColor(Color3B::BLACK);
    scroll->addChild(label_6);
    label_6->setOpacity(0);
    label_6->runAction(FADE_IN_DELAY(2.1, 0.3));
    // Labguage names are in bold font
    Vector<MenuItem *> toggles_6;
    toggles_6.pushBack(MenuItemLabelTint::create(
        onclr::label("English", 28 * s_ratio, true, Color3B::BLACK), [](Ref *sender) {}));
    toggles_6.pushBack(MenuItemLabelTint::create(
        onclr::label("Simp. Chinese", 28 * s_ratio, true, Color3B::BLACK), [](Ref *sender) {}));
    auto toggle_6 = MenuItemToggle::createWithCallback([this](Ref *sender) {
        _lang = static_cast<MenuItemToggle *>(sender)->getSelectedIndex();
         this->save();
    }, toggles_6);
    toggle_6->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
    toggle_6->setPosition(Vec2(
        12 + label_6->getContentSize().width,
        onclr::vsize.height - 368 * s_ratio));
    toggles_6.at(_lang)->setOpacity(0);
    toggles_6.at(_lang)->runAction(FADE_IN_DELAY(2.15, 0.3));
    toggle_6->setSelectedIndex(_lang);
    menu = Menu::create(toggle_6, nullptr);
    menu->setPosition(Vec2::ZERO);
    scroll->addChild(menu);

    // Update the size of the scroll view
    float miny = toggle_6->getPositionY() - toggle_6->getAnchorPointInPoints().y;
    TURN_INTO_SCROLL_VIEW(_dragStartPos, scroll, _contentSize.height - miny, _contentSize.height);

    // Schedule save
    this->getScheduler()->schedule([](float dt) {
        UserDefault::getInstance()->flush();
    }, this, 1, false, SCHEDULE_SAVE_KEY);

    return true;
}

void PreferenceLayer::save()
{
    auto ud = UserDefault::getInstance();
    ud->setFloatForKey("Senvitivity", _sliderValues[0]);
    ud->setFloatForKey("Sound_Vol", _sliderValues[1]);
    ud->setFloatForKey("FX_Vol", _sliderValues[2]);
    ud->setBoolForKey("Comfortable_Pos_Mode", _toggleValues[0]);
    ud->setBoolForKey("Debug_Info_Vis", _toggleValues[1]);
    ud->setIntegerForKey("Language", _lang);
}

void PreferenceLayer::goBack(Ref *sender)
{
    this->save();
    UserDefault::getInstance()->flush();
    this->getScheduler()->unschedule(SCHEDULE_SAVE_KEY, this);
    GO_BACK_ANIMATED;
}
