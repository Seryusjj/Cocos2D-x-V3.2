#include "AppDelegate.h"


USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate()
{
}

bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
		glview = GLView::create("My Game");
		//#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
		//glview = GLView::createWithFullScreen("My game");
		//#endif
		director->setOpenGLView(glview);

	}


	//modify the display area to test
	//delete this when finish
	glview->setFrameSize(600, 800);


	Size screenSize = glview->getFrameSize();
	Size designSize = Size(768, 1024);

	glview->setDesignResolutionSize(screenSize.width, screenSize.height, ResolutionPolicy::EXACT_FIT);

	float screenRatio = screenSize.height / screenSize.width;
	std::vector<std::string> resDirOrders;
	if (screenSize.width > 768) {
		resDirOrders.push_back("ipadhd");
	}
	else if (screenSize.width > 320) {
		if (screenRatio >= 1.5f) {
			resDirOrders.push_back("iphonehd");
		}
		else {
			resDirOrders.push_back("ipad");
		}
	}
	else {
		resDirOrders.push_back("iphone");
	}

	director->setContentScaleFactor(screenSize.height / designSize.height);
	FileUtils::getInstance()->setSearchPaths(resDirOrders);

	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(FileUtils::getInstance()->fullPathForFilename("background.mp3").c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(FileUtils::getInstance()->fullPathForFilename("pickup.wav").c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(FileUtils::getInstance()->fullPathForFilename("bombRelease.wav").c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(FileUtils::getInstance()->fullPathForFilename("rocket.wav").c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(FileUtils::getInstance()->fullPathForFilename("shipBoom.wav").c_str());

	SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.4f);
	SimpleAudioEngine::getInstance()->setEffectsVolume(0.5f);

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
