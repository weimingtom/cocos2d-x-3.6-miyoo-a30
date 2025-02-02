#pragma once

#include "2d/CCNode.h"
#include "2d/CCLabel.h"
#include <deque>
#include <vector>
//#include "tjsCommHead.h"
#include <deque>

class TVPConsoleWindow : public cocos2d::Node {
	TVPConsoleWindow();
public:
	static TVPConsoleWindow* create(int fontSize, cocos2d::Node *parent);

	void setFontSize(float size);

private:
	float _fontSize;

	std::deque<cocos2d::Label*> _dispLabels;
	std::vector<cocos2d::Label*> _unusedLabels;
	unsigned int _maxQueueSize;
};
