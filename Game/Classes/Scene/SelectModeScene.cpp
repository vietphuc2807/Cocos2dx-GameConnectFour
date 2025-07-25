#include "Scene/SelectModeScene.h"
#include "GamePlayScene/GameRiotScene.h"
#include "GamePlayScene/GameNormalScene.h"
#include "Manager/SoundManager.h"

USING_NS_CC;

using namespace cocos2d::ui;

Scene* SelectModeScene::createScene()
{
	return SelectModeScene::create();
}

bool SelectModeScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	// UI 
	UIGameSetup();

	return true;
}

void SelectModeScene::UIGameSetup()
{
	auto Size = Director::getInstance()->getVisibleSize();
	auto Origin = Director::getInstance()->getVisibleOrigin();

	auto background = Sprite::create("UI/ModeBackground.png");
	background->setPosition(Vec2(Size.width / 2 + Origin.x, Size.height / 2 + Origin.y));
	background->setScale(0.67f);
	this->addChild(background);

	// Mui ten
	selectorArrow = Sprite::create("UI/SelectorArrow.png");
	selectorArrow->setPosition(Vec2(Size.width / 2 + Origin.x, Size.height / 2 + Origin.y + 230));
	this->addChild(selectorArrow);

	// Mode normal button
	auto normalButton = Button::create("UI/ModeNormal.png");
	normalButton->setPosition(Vec2(Size.width / 2 + Origin.x + 260, Size.height / 2 + Origin.y + 620));
	normalButton->addClickEventListener([this](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		selectedMode = "Normal";

		selectorArrow->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 + Director::getInstance()->getVisibleOrigin().x,
			Director::getInstance()->getVisibleSize().height / 2 + Director::getInstance()->getVisibleOrigin().y + 230));
		selectorArrow->setVisible(true);
		});
	background->addChild(normalButton);

	// Mode riot button
	auto riotButton = Button::create("UI/ModeRiot.png");
	riotButton->setPosition(Vec2(Size.width / 2 + Origin.x + 230, Size.height / 2 + Origin.y + 270));
	riotButton->addClickEventListener([this](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		selectedMode = "Riot";

		selectorArrow->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 + Director::getInstance()->getVisibleOrigin().x,
			Director::getInstance()->getVisibleSize().height / 2 + Director::getInstance()->getVisibleOrigin().y - 50));
		selectorArrow->setVisible(true);
		});
	background->addChild(riotButton);

	// Start button 
	auto startButton = Button::create("UI/StartButton.png");
	startButton->setPosition(Vec2(Size.width / 2 + Origin.x + 660, Size.height / 2 + Origin.y - 100));
	startButton->addClickEventListener([this](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		startGame();
		});
	background->addChild(startButton);
}

void SelectModeScene::startGame()
{
	if (selectedMode == "Normal")
	{
		Director::getInstance()->replaceScene(TransitionFade::create(0.3f, GameNormalScene::createScene()));
	}
	else if (selectedMode == "Riot")
	{
		Director::getInstance()->replaceScene(TransitionFade::create(0.3f, GameRiotScene::createScene()));
	}
}