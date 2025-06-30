#pragma once
#ifndef __AI_SETUP_SCENE_H__
#define __AI_SETUP_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class AISetUpScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(AISetUpScene);

private:
	cocos2d::ui::TextField* nameField;
	cocos2d::Sprite* selectedFruitIcon = nullptr;
	cocos2d::Sprite* background;

	std::string selectedFruit;

	void UIGameSetup();

	void createFruitButtons();
};

#endif // __AI_SETUP_SCENE_H__#pragma once
