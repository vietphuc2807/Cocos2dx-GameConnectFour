#include "Scene/LoadingGame_Scene.h"
#include "Scene/MainMenuGame_Scene.h"

USING_NS_CC;

Scene* LoadingGameScene::createScene()
{
    return LoadingGameScene::create();
}

bool LoadingGameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

	// UI 
	LoadingGameScene::UIGameSetup();
	
	// Cap nhat ham update 
	this->scheduleUpdate();

    return true;
}

void LoadingGameScene::UIGameSetup()
{
	auto Size = Director::getInstance()->getVisibleSize();
	auto Origin = Director::getInstance()->getVisibleOrigin();

	// Background 
	backgroundLoadingGame = Sprite::create("UI/LoadingGame.png");
	backgroundLoadingGame->setPosition(Vec2(Size.width / 2 + Origin.x, Size.height / 2 + Origin.y));
	backgroundLoadingGame->getTexture()->setAntiAliasTexParameters();
	backgroundLoadingGame->setScale(0.67f);

	this->addChild(backgroundLoadingGame);

	// Loading bar 
	stickLoading = ui::LoadingBar::create("UI/StickLoading.png");

	stickLoading->setPosition(Vec2(Size.width / 2 + Origin.x + 265, Size.height / 2 + Origin.y - 290));

	stickLoading->setScale(1.2f);

	stickLoading->setPercent(0);

	backgroundLoadingGame->addChild(stickLoading);
}

void LoadingGameScene::update(float dt)
{
	// Cap nhat thanh loading
	stickLoading->setPercent(stickLoading->getPercent() + 0.7f);

	if (stickLoading->getPercent() >= 100 && !isSceneChanged)
	{
		isSceneChanged = true;

		auto delay = DelayTime::create(1.0f);
		this->runAction(delay);

		auto scene = MainMenuGameScene::createScene();
		Director::getInstance()->replaceScene(TransitionFade::create(0.3f, scene, Color3B::BLACK));
	}
}