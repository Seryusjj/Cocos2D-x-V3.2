#include "AppDelegate.h"
#include "GameLayer.h"
#include "SimpleAudioEngine.h"
#include <iostream>
#include <memory>

USING_NS_CC;//uing namespace cocos2d
using namespace CocosDenshion;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{

}



bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    Director* director = Director::getInstance();
    GLView* glview = director->getOpenGLView();


    if(!glview) {
        //glview = GLView::createWithFullScreen("My game");
        glview = GLView::create("My game");
        director->setOpenGLView(glview);
    }



    adaptReslution(glview,director);
    audioPreloading();




    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    Scene* scene = GameLayer::createScene();

    // run
    director->runWithScene(scene);

    return true;
}

void AppDelegate::audioPreloading(){
    SimpleAudioEngine::getInstance()->
            preloadEffect(FileUtils::getInstance()->fullPathForFilename("hit.wav").c_str());
    SimpleAudioEngine::getInstance()->
            preloadEffect(FileUtils::getInstance()->fullPathForFilename("score.wav").c_str());
}

void AppDelegate::adaptReslution(GLView* glview,Director* director){
    //--- multi resolution support ---
    auto fileUtils = FileUtils::getInstance();
    auto screenSize = glview->getFrameSize();
    std::vector<std::string> resDirOrders;

    glview->setDesignResolutionSize(DESING_WIDTH,DESING_HEIGHT,ResolutionPolicy::EXACT_FIT);
    if (screenSize.height <= DESING_HEIGHT || screenSize.width<=DESING_WIDTH){
        //normal res assets
        resDirOrders.push_back("sd");
        director->setContentScaleFactor(1);

    }else{
        //higth res assets retina display on ipad
        resDirOrders.push_back("hd");
        director->setContentScaleFactor(1536/DESING_HEIGHT);
    }
    fileUtils->setSearchPaths(resDirOrders);
}


// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
