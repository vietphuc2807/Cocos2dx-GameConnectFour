#ifndef __SELECT_MODE_SCENE_H__
#define __SELECT_MODE_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class SelectModeScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(SelectModeScene);

private:
    std::string selectedMode = "Normal";

    cocos2d::Sprite* selectorArrow = nullptr;

    void UIGameSetup();

    void startGame();
};

#endif // __SELECT_MODE_SCENE_H__

