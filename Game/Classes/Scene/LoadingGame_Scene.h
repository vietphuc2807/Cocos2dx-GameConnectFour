#pragma once
#ifndef __LOADING_GAME_SCENE_H__
#define __LOADING_GAME_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class LoadingGameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(LoadingGameScene);

private:
	void UIGameSetup();

    void update(float dt);

    bool isSceneChanged = false;

    cocos2d::Sprite* backgroundLoadingGame;

    cocos2d::ui::LoadingBar* stickLoading;
};

#endif // __LOADING_GAME_SCENE_H__#pragma once
