#pragma once
#ifndef __MAINMENU_GAME_SCENE_H__
#define __MAINMENU_GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class MainMenuGameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(MainMenuGameScene);

private:
	void UIGameSetup();

	void SpawnLeaf();

	void spawnLeafUpdate(float dt);

	void showInfoPopup();

	void showPauseMenuPopupOne();

	void showPauseMenuPopupTwo();
	
	void showStatsPopup();

	void showOptionsPopup();

	cocos2d::Sprite* backgroundMainMenu;

	cocos2d::Sprite* leaf;
};

#endif // __MAINMENU_GAME_SCENE_H__#pragma once
