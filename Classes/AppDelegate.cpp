#include "AppDelegate.h"
#include "SplashScene.h"
#include "Global.h"
using namespace cocos2d;

#define USE_BG_MUSIC 0

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if IS_ON_PC
        glview = GLView::createWithRect("On Colour for PC", Rect(0, 0, 720, 480));
#else
        glview = GLView::createWithRect("On Colour", Rect(0, 0, 480, 320));
#endif
        director->setOpenGLView(glview);
    }
    director->setDisplayStats(UserDefault::getInstance()->getBoolForKey("Debug_Info_Vis", true));
    director->setAnimationInterval(1.0 / 60);
    // Turn off at the very beginning. We'll turn it on later
    Device::setAccelerometerEnabled(false);
    // Initialize global variables
    onclr::init();
    // Everything ready, let's go!
    auto scene = Splash::createScene();
    director->runWithScene(scene);
    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
#if USE_BG_MUSIC
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
#endif
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
#if USE_BG_MUSIC
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
#endif
}
