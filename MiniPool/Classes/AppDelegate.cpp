#include "AppDelegate.h"


AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

//if you want a different context,just modify the value of glContextAttrs
//it will takes effect on all platforms
void AppDelegate::initGLContextAttrs()
{
    //set OpenGL context attributions,now can only set six attributions:
    //red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
        glview = GLViewImpl::create("My Game");
        director->setOpenGLView(glview);
    }

	glview->setFrameSize(320,480);
	Size screenSize = glview->getFrameSize();
	//designed for the regular iphone
	Size designSize = Size(320, 480);

	glview->setDesignResolutionSize(screenSize.width, screenSize.height, ResolutionPolicy::SHOW_ALL);

	std::vector<std::string> resDirOrders;
	if (screenSize.width > 640) {
		resDirOrders.push_back("ipadhd");
		director->setContentScaleFactor(1280 / designSize.width);
	}
	else if (screenSize.width > 320) {
		resDirOrders.push_back("ipad");
		director->setContentScaleFactor(640 / designSize.width);
	}
	else {
		resDirOrders.push_back("iphone");
		director->setContentScaleFactor(320 / designSize.width);
	}
	FileUtils::getInstance()->setSearchPaths(resDirOrders);



	SimpleAudioEngine::getInstance()->preloadEffect(FileUtils::getInstance()->fullPathForFilename("ball.wav").c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(FileUtils::getInstance()->fullPathForFilename("drop.wav").c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(FileUtils::getInstance()->fullPathForFilename("hit.wav").c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(FileUtils::getInstance()->fullPathForFilename("whitedrop.wav").c_str());

    // turn on display FPS
    //director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    //director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
	auto scene = GameLayer::createScene();

    // run
    director->runWithScene(scene);

    return true;
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
