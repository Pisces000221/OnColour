#include "GameScene.h"
#include "MenuItemLabelTint.h"
#include "Bubble.h"
#include "BorderedBubble.h"
#include "Photon.h"
#include "SineVelPhoton.h"
#include "Bomb.h"
#include "Global.h"
#include "SimpleAudioEngine.h"
using namespace cocos2d;
using namespace CocosDenshion;

const Color3B Gameplay::_warnColours[] = {
    Color3B(255, 0, 0), Color3B(0, 255, 0), Color3B(0, 0, 255)
};
const char *Gameplay::_warnMessage[] = {
    "* Off colour: red *", "* Off colour: green *", "* Off colour: blue *"
};

// maximum supported keys to be pressed at once: 2
#define KEYBOARD_SCHEDULE_KEY "KEYBD_BALLMOVE"
#define KEYBOARD_SCHEDULE_KEY_2 "KEYBD_BALLMOVE_SECOND"
#define TICK_SCHEDULE_KEY "GAME_TICKER"
#define FEVER_ANIMATION_TAG 492357816
#define FEVER_ANIMATION_TAG_2 1035369

#define WARNER_POS(i) cocos2d::Vec2( \
    onclr::vsize.width * 0.5, onclr::vsize.height * 0.5 - 32 * i - 40)

void Gameplay::init2()
{
    auto scene = this->getScene();
    // The background
    auto bg = LayerColor::create(Color4B(255, 255, 255, 255));
    scene->addChild(bg, -1);    // the very, very bottom ('end' in bring-to-front/send-to-end)
    moveBall(0, 0, 0);          // move the view to show the player again

    // The 'back' menu
    auto backItem = MenuItemImage::create(
        "images/back.png", "images/back.png", CC_CALLBACK_1(Gameplay::goBack, this));
    backItem->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
    backItem->setNormalizedPosition(Vec2::ANCHOR_TOP_LEFT);
    backItem->setScale((onclr::ratio - 1) * 0.4 + 1);
    auto menu = Menu::create(backItem, NULL);
    menu->setPosition(Vec2::ZERO);
    scene->addChild(menu, 6);

    // The score displayer
    _scoreDisplayer = onclr::label("0 s", 80, false);
    _scoreDisplayer->setNormalizedPosition(Vec2(0.5, 0.5));
    _scoreDisplayer->setColor(Color3B::BLACK);
    _scoreDisplayer->setOpacity(128);
    scene->addChild(_scoreDisplayer, 8);
    // The warner
    for (int i = 0; i < 3; i++) {
        _warnerPlaceAvailable[i] = true;
        _warner[i] = onclr::label(_warnMessage[i], 32, false);
        _warner[i]->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
        _warner[i]->setPosition(Vec2::ZERO);
        _warner[i]->setOpacity(0);
        _warner[i]->setColor(_warnColours[i]);
        scene->addChild(_warner[i], 8);
    }
}

bool Gameplay::init()
{
    if (!Layer::init()) return false;
    this->setContentSize(onclr::mapsize);

    // Preload sound effects
    SimpleAudioEngine::getInstance()->setEffectsVolume(UserDefault::getInstance()->getFloatForKey("FX_Vol"));
    SimpleAudioEngine::getInstance()->preloadEffect("sounds/pop.wav");

    // Reset score, time, etc.
    _score = 0.0f;
    _timeToLastPhotonGen = _photonHugTime = 0.0f;
    _photonHugID = 0;
    _r = _g = _b = onclr::player_colour_initial;
    _isInFeverMode = _gamePaused = false;
    _difficulty_rate = 1;

    // Load sensitivity & comfortable position preferences
    _sensitivity = UserDefault::getInstance()->getFloatForKey("Senvitivity", 1);
    _comPosEnabled = UserDefault::getInstance()->getBoolForKey("Comfortable_Pos_Mode", false);
    _comPosInitialized = false;

    // The player
    _playerRadius = onclr::player_radius * onclr::bubble_scale;
    _player = BorderedBubble::create(
        _playerRadius, 3 * onclr::bubble_scale, GREY_3B(onclr::player_colour_initial));
    // Don't use normalized positions here since we need to set absolute position later
    // What's more, its parent's size is onclr::mapsize!
    _player->setPosition(Vec2(onclr::mapsize.width / 2, onclr::mapsize.height / 2));
    this->addChild(_player, 1000);  // use very big z-order in order not to be covered bu other bubbles

#if IS_ON_PC
    // Enable 'tilting' by keyboard
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = [this](EventKeyboard::KeyCode key, Event* event) {
        if (_gamePaused) return;
        float x = 0, y = 0;
        if (key == EventKeyboard::KeyCode::KEY_UP_ARROW) y = 1;
        else if (key == EventKeyboard::KeyCode::KEY_DOWN_ARROW) y = -1;
        else if (key == EventKeyboard::KeyCode::KEY_LEFT_ARROW) x = -1;
        else if (key == EventKeyboard::KeyCode::KEY_RIGHT_ARROW) x = 1;
        // DEBUG-USE: press 'A' to set all colour values back to 255.
        // For testing fever mode.
        else if (key == EventKeyboard::KeyCode::KEY_A) { _r = _g = _b = 255; return; }
        // DEBUG-USE: press 'B' to set all colour under the fireline
        // For testing commit 'Prevent off colour but nowhere to get that colour'
        else if (key == EventKeyboard::KeyCode::KEY_B) { _r = _g = _b = 64; return; }
        // DEBUG-USE: press 'C' to print player's colour at present
        else if (key == EventKeyboard::KeyCode::KEY_C) { CCLOG("%.2f, %.2f, %.2f", _r, _g, _b); return; }
        // DEBUG-USE: press 'D' to increase difficulty
        else if (key == EventKeyboard::KeyCode::KEY_D) { increaseDifficultyRate(); return; }
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
        if (_gamePaused) return;
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
        if (_gamePaused) return;
        if (_comPosEnabled) {
            if (_comPosInitialized) this->moveBall(acc->x - _comPosX, acc->y - _comPosY, 0);
            else { _comPosX = acc->x; _comPosY = acc->y; _comPosInitialized = true; }
        } else {
            this->moveBall(acc->x, acc->y, 0);
        }
    });
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, _player);

    // Turn on accelerometer
    Device::setAccelerometerEnabled(true);
#endif

    // Touch to pause/resume
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->setSwallowTouches(true);
    _touchListener->onTouchBegan = [this](Touch *touch, Event *event) {
        return true;
    };
    _touchListener->onTouchMoved = [this](Touch *touch, Event *event) {
    };
    _touchListener->onTouchEnded = [this](Touch *touch, Event *event) {
        this->pauseOrResume();
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);

    // Turn on score scheduler
    //http://blog.csdn.net/qq575787460/article/details/8531397
    // Father-tricking CC_CALLBACK_1...
    this->getScheduler()->schedule(std::bind(&Gameplay::tick, this, std::placeholders::_1),
        this, 0, false, TICK_SCHEDULE_KEY);
    // To test Cocos2d-x PR #7270
    //this->getScheduler()->schedule(schedule_selector(Gameplay::tick), this, 0, false);

    return true;
}

void Gameplay::moveBall(float acc_x, float acc_y, float dt)
{
    auto curpos = _player->getPosition();
#if IS_ON_PC
    curpos.x += acc_x * 216.0f * _sensitivity * dt;
    curpos.y += acc_y * 216.0f * _sensitivity * dt;
#else
    curpos.x += acc_x * 12.0f * _sensitivity;
    curpos.y += acc_y * 12.0f * _sensitivity;
#endif
    FIX_POS(curpos.x, onclr::player_radius, onclr::mapsize.width - onclr::player_radius);
    FIX_POS(curpos.y, onclr::player_radius, onclr::mapsize.height - onclr::player_radius);
    _player->setPosition(curpos);
    // Update the location of the container, that is, 'this'.
    curpos.x = -curpos.x + onclr::vsize.width / 2;
    curpos.y = -curpos.y + onclr::vsize.height / 2;
    FIX_POS(curpos.x, onclr::vsize.width - onclr::mapsize.width, 0);
    FIX_POS(curpos.y, onclr::vsize.height - onclr::mapsize.height, 0);
    this->setPosition(curpos);
}

void Gameplay::tick(float dt)
{
    this->movePhotonsAndShowPointers(dt);
    if (_gamePaused) return;
    if (_isInFeverMode) _score += dt * 2; else _score += dt;
    char s[16]; sprintf(s, "%d s", (int)_score);
    _scoreDisplayer->setString(s);
    // Update colour
    float deltaclr = onclr::player_colour_lost * dt;
    _r -= deltaclr; _g -= deltaclr; _b -= deltaclr;
    if (_r <= 0 || _g <= 0 || _b <= 0) {
        endGame();
        if (_r <= 0) _r = 0;
        if (_g <= 0) _g = 0;
        if (_b <= 0) _b = 0;
    }
    _player->setColor(Color3B((int)_r, (int)_g, (int)_b));
    // Check warnings
    for (int i = 0; i < 3; i++) {
        bool isWarn = _warnColours[i].r * onclr::player_colour_warning / 255.0 > _r
         || _warnColours[i].g * onclr::player_colour_warning / 255.0 > _g
         || _warnColours[i].b * onclr::player_colour_warning / 255.0 > _b;
        if (!_warnerShown[i] && isWarn) {
            _warner[i]->runAction(FadeTo::create(0.3, 128));
            _warnerShown[i] = true;
            _warner[i]->setPosition(getWarnerPlaceAvailable());
        } else if (_warnerShown[i] && !isWarn) {
            _warner[i]->runAction(FadeOut::create(0.3));
            _warnerShown[i] = false;
            freeWarnerPlace(_warner[i]->getPosition());
        }
    }
    // Check if is in fever mode
    this->checkFever();
    // Check if is generating bubble
    _timeToLastPhotonGen -= dt;
    if (_timeToLastPhotonGen <= 0) {
        _timeToLastPhotonGen = RAND_BTW(onclr::photongen_mintime, onclr::photongen_maxtime);
        // Generate a photon.
        this->generatePhoton();
    }
    this->checkHugs(dt);
}

void Gameplay::generatePhoton()
{
    Photon *b = nullptr;
    float a = RAND_0_1;
    if (a < onclr::normal_photon_possib)
        b = Photon::randomGen();
    else if (a < onclr::normal_photon_possib + onclr::sinevel_photon_possib) {
        auto bb = SineVelPhoton::randomGen();
        bb->setPeriod(bb->getPeriod() / _difficulty_rate);
        bb->setAmplitude(bb->getAmplitude() * _difficulty_rate);
        b = bb;
    } else
        b = Bomb::randomGen();
    // Prevent off colour but nowhere to get that colour
    // If we're running out of red and blue, we'll get a magenta photon here
    Color3B c = Color3B::BLACK;
    for (int i = 0; i < 3; i++)
        if (_warnerShown[i]) {
            c.r += _warnColours[i].r;
            c.g += _warnColours[i].g;
            c.b += _warnColours[i].b;
        }
    if (c != Color3B::BLACK) {  // Oh!! We ran out of one or even more colour(s)...
        b = Photon::randomGen();    // we just need a normal photon
        if (c == Color3B::WHITE)    // Wow!! Nearly black... Running out of all colours
            if (rand() % 3) c.r = 0;
            else if (rand() % 2) c.g = 0;
            else c.b = 0;           // Remove one of the colours, otherwise it'll be invisible
        b->setColor(c);         // ... and put the needed colour there
        b->setColourValue(std::max(156, b->getColourValue()));
    }
    float b_radius = b->getRadius();
    // Generate a random position tangent to the border
    // > The radiation is emitted tangent to these trajectories. (Scientific American)
    if (rand() % 2) {   // top/bottom border
        b->setPosition(Vec2(
            RAND_BTW(-b_radius, onclr::mapsize.width + b_radius),
            rand() % 2 ? onclr::mapsize.height + b_radius : -b_radius));
    } else {            // left/right border
        b->setPosition(Vec2(
            rand() % 2 ? onclr::mapsize.width + b_radius : -b_radius,
            RAND_BTW(-b_radius, onclr::mapsize.height + b_radius)));
    }
    b->initStartPos();
    // Generate a random direction that goes into the screen
    float destX = RAND_BTW(b_radius, onclr::mapsize.width - b_radius);
    float destY = RAND_BTW(b_radius, onclr::mapsize.height - b_radius);
    b->setVelocity(_difficulty_rate * RAND_BTW(onclr::photon_minvelocity, onclr::photon_maxvelocity),
        atan2f(destX - b->getPositionX(), destY - b->getPositionY()));  // why it's x/y??
    // MY CODE WORKS, I DON'T KNOW WHY
    _photons.pushBack(b);
    this->addChild(b);
}

void Gameplay::removePhoton(Photon *photon)
{
    if (photon->getUserData())
        ((Sprite *)photon->getUserData())->runAction(Sequence::create(
            FadeOut::create(0.3f),
            RemoveSelf::create(), nullptr));
    _photons.eraseObject(photon);
}

// Move photons and show pointers
// A little complicated code...
// Let it go. The code never bothered me anyway.
void Gameplay::movePhotonsAndShowPointers(float dt)
{
    for (auto &photon : _photons) {
        photon->move(dt);
        Vec2 p = photon->getPosition();
        Vec2 p0 = -_position + Vec2(onclr::vsize.width * 0.5, onclr::vsize.height * 0.5);
        float r = photon->getRadius();
        if (p.x < -r || p.x > onclr::mapsize.width + r
         || p.y < -r || p.y > onclr::mapsize.height + r) {
            // Timber!!
            this->removePhoton(photon);
        } else if (p.x < -_position.x - r || p.x > -_position.x + onclr::vsize.width + r
         || p.y < -_position.y - r || p.y > -_position.y + onclr::vsize.height + r) {
            // We need a pointer to point at it
            // Ensure that there IS a pointer to use
            if (photon->getUserData() == 0x0) {
                auto pointer = Sprite::create("images/pointer.png");
                photon->setUserData(pointer);
                pointer->setColor(photon->getColor());
                pointer->setOpacity(0); // we~ will~ we~ will~ fade you!
                pointer->runAction(FadeTo::create(0.3, photon->getColourValue()));
                pointer->setTag(photon->getColourValue());
                this->getScene()->addChild(pointer);
            }
            Sprite *pointer = (Sprite *)photon->getUserData();
            if (pointer->getUserData()) {
                // The user data of pointers is used to determine whether to fade it
                // We use it as a boolean value
                pointer->setUserData(0x0);
                // The tag sof pointers are used for storing the initial opacity
                pointer->runAction(FadeTo::create(0.3f, pointer->getTag()));
            }
            // Determine the position of the pointer
            Vec2 delta_p = p - p0;
            pointer->setRotation(-atan2f(delta_p.y, delta_p.x) / M_PI * 180);
            float pw = pointer->getContentSize().width;
            Vec2 pointer_p = Vec2::ZERO;
            Vec2 pointer_anchor = Vec2::ZERO;
            // 0, 1, 2, 3 = BL, BR, TR, TL
            // [3] ---------- [2]
            //  |              |
            //  |    SCREEN    |
            //  |              |
            // [0] ---------- [1]
            Vec2 cornerInMap[4] = {
                -_position,
                -_position + Vec2(onclr::vsize.width, 0),
                -_position + Vec2(onclr::vsize.width, onclr::vsize.height),
                -_position + Vec2(0, onclr::vsize.height)
            };
            if (Vec2::isSegmentIntersect(cornerInMap[0], cornerInMap[1], p, p0)) {
                pointer_p = Vec2::getIntersectPoint(cornerInMap[0], cornerInMap[1], p, p0);
                pointer_anchor = Vec2::ANCHOR_MIDDLE_BOTTOM;
            } else if (Vec2::isSegmentIntersect(cornerInMap[1], cornerInMap[2], p, p0)) {
                pointer_p = Vec2::getIntersectPoint(cornerInMap[1], cornerInMap[2], p, p0);
                pointer_anchor = Vec2::ANCHOR_MIDDLE_RIGHT;
            } else if (Vec2::isSegmentIntersect(cornerInMap[2], cornerInMap[3], p, p0)) {
                pointer_p = Vec2::getIntersectPoint(cornerInMap[2], cornerInMap[3], p, p0);
                pointer_anchor = Vec2::ANCHOR_MIDDLE_TOP;
            } else {
                pointer_p = Vec2::getIntersectPoint(cornerInMap[3], cornerInMap[0], p, p0);
                pointer_anchor = Vec2::ANCHOR_MIDDLE_LEFT;
            }
            Vec2 final_p = pointer_p + _position -
                (pointer_anchor - Vec2::ANCHOR_MIDDLE) * pw;
            FIX_POS(final_p.x, pw / 2, onclr::vsize.width - pw / 2);
            FIX_POS(final_p.y, pw / 2, onclr::vsize.height - pw / 2);
            pointer->setPosition(final_p);
        } else {
            // It's in the screen
            Sprite *pointer = (Sprite *)photon->getUserData();
            if (pointer && !pointer->getUserData()) {
                pointer->setUserData((void *)0x5f3759df);
                pointer->runAction(FadeOut::create(0.3f));
            }
        }
    }
}

void Gameplay::checkHugs(float dt)
{
    bool hugged = false;
    for (auto &photon : _photons)
        if (photon->isBomb() && semi_huggy(photon)) {
            // BOOOOOM!!
            _r = 0; _g = 0; _b = 0;
            return;
        }
    if (_photonHugID > 0) {
        for (auto &photon : _photons) if (photon->getID() == _photonHugID) {
            if (!huggy(photon)) { // got out
                hugged = false;
                _player->reset(0.7);
            } else {
                hugged = true;
                _photonHugTime += dt;
                Color3B c = photon->getColor();
                if (_photonHugTime > photon->getHugTime()) {
                    _photonHugTime = photon->getHugTime();
                    // Let's eat this ;)
                    photon->runAction(Sequence::create(
                        ScaleTo::create(0.25, 0),
                        RemoveSelf::create(), nullptr));
                    _player->runAction(EaseElasticOut::create(
                        ScaleTo::create(0.5, 1)));
                    _player->reset(0.2);
                    SimpleAudioEngine::getInstance()->playEffect("sounds/pop.wav");
                    // If we eat something, we get energy, huh?
                    int cval = photon->getColourValue();
                    _r += (float)(c.r * cval) / 255.0;
                    _g += (float)(c.g * cval) / 255.0;
                    _b += (float)(c.b * cval) / 255.0;
                    if (_r > 255) _r = 255;
                    if (_g > 255) _g = 255;
                    if (_b > 255) _b = 255;
                    // ... And we get weight and size, don't we?
                    this->increaseDifficultyRate();
                    // Remove finally. Otherwise it causes wrong results
                    this->removePhoton(photon);
                }
                float deltaclr = onclr::player_colour_lost * dt;
                // The colour lose 2/3 slower when hugging
                // Magic of love?
                _r += (float)c.r * deltaclr / 255.0;
                _g += (float)c.g * deltaclr / 255.0;
                _b += (float)c.b * deltaclr / 255.0;
                _player->setBorderProgress(1 - _photonHugTime / photon->getHugTime());
            }
            break;
        }
    } else for (auto &photon : _photons) {
        if (huggy(photon)) {
            _photonHugID = photon->getID();
            _photonHugTime = 0.0;
            hugged = true; break;
        }
    }
    if (!hugged) _photonHugID = -1;
}

bool Gameplay::huggy(Photon *photon)
{
    float r = (photon->getRadius() - _playerRadius);
    return photon->getPosition().distanceSquared(_player->getPosition()) <= r * r;
}
bool Gameplay::semi_huggy(Photon *photon)
{
    float r = photon->getRadius() + _playerRadius;
    return photon->getPosition().distanceSquared(_player->getPosition()) <= r * r;
}

void Gameplay::checkFever()
{
    bool curIsInFeverMode = _r >= onclr::player_colour_fever &&
        _g >= onclr::player_colour_fever && _b >= onclr::player_colour_fever;
    if (curIsInFeverMode && !_isInFeverMode) {
        CCLOG("FEVER!!!!");
        _isInFeverMode = true;
        auto action = RepeatForever::create(Sequence::create(
            DelayTime::create(0.6),
            CallFunc::create([this] {
                auto a2 = TintTo::create(0.6, rand() % 255, rand() % 255, rand() % 255);
                a2->setTag(FEVER_ANIMATION_TAG_2);
                _scoreDisplayer->runAction(a2);
            }), nullptr));
        action->setTag(FEVER_ANIMATION_TAG);
        _scoreDisplayer->runAction(action);
    } else if (!curIsInFeverMode && _isInFeverMode) {
        CCLOG("Back to normal");
        _isInFeverMode = false;
        _scoreDisplayer->stopActionByTag(FEVER_ANIMATION_TAG_2);
        _scoreDisplayer->stopActionByTag(FEVER_ANIMATION_TAG);
        _scoreDisplayer->runAction(TintTo::create(0.6, 0, 0, 0));
    }
    _isInFeverMode = curIsInFeverMode;
}

void Gameplay::increaseDifficultyRate()
{
    if (_difficulty_rate < onclr::max_difficulty_rate)
        _difficulty_rate += onclr::difficulty_rate_increment;
}

// copied from HexBizarre/GameScene
void Gameplay::goBack(Ref *sender)
{
#if !IS_ON_PC
    // Turn off accelerometer
    Device::setAccelerometerEnabled(false);
#endif
    // Run animation and go back (see Global.h)
    GO_BACK_ANIMATED;
}

void Gameplay::pauseOrResume()
{
    auto scheduler = this->getScheduler();
    if (_gamePaused) {
        scheduler->resumeTarget(this);
        _gamePaused = false;
    } else {
        _scoreDisplayer->setString("Paused");
        scheduler->pauseTarget(this);
        ENSURE_UNSCHEDULED(scheduler, KEYBOARD_SCHEDULE_KEY);
        ENSURE_UNSCHEDULED(scheduler, KEYBOARD_SCHEDULE_KEY_2);
        _gamePaused = true;
    }
}

void Gameplay::endGame()
{
    _gamePaused = true;
    _eventDispatcher->removeEventListener(_touchListener);
    ENSURE_UNSCHEDULED(this->getScheduler(), KEYBOARD_SCHEDULE_KEY);
    ENSURE_UNSCHEDULED(this->getScheduler(), KEYBOARD_SCHEDULE_KEY_2);
    _scoreDisplayer->setString("Oops!!");
    _scoreDisplayer->runAction(FadeTo::create(1, 255));
    _player->runAction(EaseElasticIn::create(ScaleTo::create(0.5, 0)));
    char s[64]; sprintf(s, "Final score: %d", (int)_score);
    for (int i = 0; i < 3; i++) {
        freeWarnerPlace(_warner[i]->getPosition());
        if (_warnerPlaceAvailable[0]) {
            _warner[i]->setColor(Color3B::BLACK);
            _warner[i]->runAction(Sequence::create(
                FadeOut::create(0.3), FadeIn::create(0.3), nullptr));
            _warner[i]->setString(s);
            break;
        }
    }
}

Vec2 Gameplay::getWarnerPlaceAvailable()
{
    for (int i = 0; i < 3; i++)
        if (_warnerPlaceAvailable[i]) {
            _warnerPlaceAvailable[i] = false;
            return WARNER_POS(i);
        }
    return Vec2::ZERO;
}

void Gameplay::freeWarnerPlace(cocos2d::Vec2 p)
{
    for (int i = 0; i < 3; i++)
        if (p.fuzzyEquals(WARNER_POS(i), 5)) {
            _warnerPlaceAvailable[i] = true;
            break;
        }
}
