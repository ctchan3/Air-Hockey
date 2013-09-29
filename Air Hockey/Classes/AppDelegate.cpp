//
//  Air_HockeyAppDelegate.cpp
//  Air Hockey
//
//  Created by To Chan on 15/9/13.
//  Copyright __MyCompanyName__ 2013. All rights reserved.
//

#include "AppDelegate.h"

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "GameLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching()
{
    // initialize director
    CCDirector *pDirector = CCDirector::sharedDirector();
    //pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    
    pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
    
    CCSize screenSize = pDirector->getWinSize();
    CCLOG("%f %f", screenSize.width, screenSize.height);
    //CCSize screenSize = pEGLView->getFrameSize();
    //CCEGLView::sharedOpenGLView() -> setDesignResolutionSize(768, 1024, kResolutionExactFit);
    if (screenSize.width > 768) {
        CCLOG("hd");
        CCFileUtils::sharedFileUtils()->addSearchPath("hd");
        pDirector->setContentScaleFactor(2);
    } else {
        CCLOG("SD");
        //or else, use standard definition
        CCFileUtils::sharedFileUtils()->addSearchPath("sd");
        pDirector->setContentScaleFactor(1);
    }

    SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("hit.wav").c_str());
    SimpleAudioEngine::sharedEngine()->preloadEffect(CCFileUtils::sharedFileUtils()->fullPathForFilename("score.wav").c_str());
AVAudioSession
    // turn on display FPS
    pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    CCScene *pScene = GameLayer::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground()
{
    CCDirector::sharedDirector()->stopAnimation();
    SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->pauseAllEffects();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    CCDirector::sharedDirector()->startAnimation();
    SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
    SimpleAudioEngine::sharedEngine()->resumeAllEffects();
}
