#include "ConsoleWindow.h"
#include "cocos2d/MainScene.h"
#include <pthread.h>

using namespace cocos2d;

TVPConsoleWindow::TVPConsoleWindow() {

}

TVPConsoleWindow* TVPConsoleWindow::create(int fontSize, cocos2d::Node *parent) {
	TVPConsoleWindow *ret = new TVPConsoleWindow;
	ret->init();
	ret->setAnchorPoint(Vec2::ZERO);
	ret->setFontSize(fontSize);
	ret->autorelease();
	if (parent) {
		ret->setContentSize(parent->getContentSize());
	}
	return ret;
}

extern pthread_t *TVPMainThreadID;

void TVPConsoleWindow::setFontSize(float size) {
	_fontSize = size;
	_maxQueueSize = getContentSize().height / size + 2;
}

