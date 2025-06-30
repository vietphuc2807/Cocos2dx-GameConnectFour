#ifndef __LOADING_ENGINE_SCENE_H__
#define __LOADING_ENGINE_SCENE_H__

#include "cocos2d.h"

class LoadingEngineScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    CREATE_FUNC(LoadingEngineScene);

private:
    void UIGameSetup();

	cocos2d::Sprite* logoEngine; 
};

#endif // __LOADING_ENGINE_SCENE_H__
