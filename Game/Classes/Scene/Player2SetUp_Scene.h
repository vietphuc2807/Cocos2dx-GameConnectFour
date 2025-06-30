#pragma once
#ifndef __PLAYER2_SETUP_SCENE_H__
#define __PLAYER2_SETUP_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class Player2SetUpScene : public cocos2d::Scene
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	CREATE_FUNC(Player2SetUpScene);

private:
	cocos2d::ui::TextField* nameField;
	cocos2d::Sprite* selectedFruitIcon = nullptr;
	cocos2d::Sprite* background;

	std::string selectedFruit;

	void UIGameSetup();

	void createFruitButtons();
};

#endif // __PLAYER2_SETUP_SCENE_H__#pragma once
