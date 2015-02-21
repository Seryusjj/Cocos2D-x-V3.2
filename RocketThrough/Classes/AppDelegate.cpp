
/*
 
 Background music:
 8bit Dungeon Level by Kevin MacLeod (incompetech.com)
 
 */

#include "AppDelegate.h"





AppDelegate::AppDelegate()
{

}

AppDelegate::~AppDelegate()
{
}

void AppDelegate::initGLContextAttrs()
{
	//set OpenGL context attributions,now can only set six attributions:
	//red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs(glContextAttrs);
}

bool AppDelegate::applicationDidFinishLaunching()
{
	// initialize director
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	if (!glview) {
		glview = GLViewImpl::create("My Game");
		director->setOpenGLView(glview);
	}


	//modify the display area to test
	//delete this when finish
	// 480 x 854
	glview->setFrameSize(480, 854);//sony xeria u
	//glview->setFrameSize(320,480 );//iphone
	//glview->setFrameSize(768, 1024);//desing res


	Size screenSize = glview->getFrameSize();
	Size designSize = Size(768, 1024);

	glview->setDesignResolutionSize(designSize.width, designSize.height, ResolutionPolicy::EXACT_FIT);

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


	director->setContentScaleFactor(screenSize.height / designSize.height );
	FileUtils::getInstance()->setSearchPaths(resDirOrders);


	SimpleAudioEngine::getInstance()->preloadBackgroundMusic(FileUtils::getInstance()->fullPathForFilename("background.mp3").c_str());
	SimpleAudioEngine::getInstance()->preloadEffect(FileUtils::getInstance()->fullPathForFilename("pickup.wav").c_str());
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