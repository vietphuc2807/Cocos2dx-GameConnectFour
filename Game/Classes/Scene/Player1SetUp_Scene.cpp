#include "Scene/MainMenuGame_Scene.h"
#include "Scene/Player1SetUp_Scene.h"
#include "Scene/Player2SetUp_Scene.h"
#include "Manager/GameManager.h"
#include "Manager/SoundManager.h"


USING_NS_CC;

using namespace cocos2d::ui;

Scene* Player1SetUpScene::createScene()
{
	return Player1SetUpScene::create();
}

bool Player1SetUpScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	
	// UI 
	UIGameSetup();

	return true;
}

void Player1SetUpScene::UIGameSetup()
{
	auto Size = Director::getInstance()->getVisibleSize();
	auto Origin = Director::getInstance()->getVisibleOrigin();

	// Background  
	background = Sprite::create("UI/Background1PlayerSetUp.png");
	background->setPosition(Vec2(Size.width / 2 + Origin.x, Size.height / 2 + Origin.y));
	
	background->setScale(0.7f);

	background->getTexture()->setAntiAliasTexParameters();

	this->addChild(background);

	// Name field
	nameField = TextField::create("Enter Player Name", "fonts/Marker Felt.ttf", 55);
	nameField->setAnchorPoint(Vec2(0, 0.5f));
	nameField->setTextColor(Color4B::BLACK);
	nameField->setPosition(Vec2(Size.width / 2 - 360 + Origin.x, Size.height - 150 + Origin.y));
	nameField->setMaxLength(7);
	nameField->setMaxLengthEnabled(true);
	background->addChild(nameField);

	// Bieu tuong trai cay duoc chon 
	selectedFruitIcon = Sprite::create();
	selectedFruitIcon->setOpacity(0);
	background->addChild(selectedFruitIcon);

	// Tao fruit buttons
	createFruitButtons();

	// Back button 
	auto backButton = ui::Button::create("UI/BackButton1.png");
	backButton->setPosition(Vec2(Size.width / 2 - 180 + Origin.x, Size.height - 1400 + Origin.y));
	background->addChild(backButton);
	backButton->addClickEventListener([this](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		Director::getInstance()->replaceScene(TransitionFade::create(0.3f, MainMenuGameScene::createScene()));
		});

	// Next button 
	auto nextButton = ui::Button::create("UI/NextButton1.png");
	nextButton->setPosition(Vec2(Size.width / 2 + 640 + Origin.x, Size.height - 1390 + Origin.y));
	nextButton->setScale(0.9f);
	background->addChild(nextButton);
	nextButton->addClickEventListener([this](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		// Kiem tra ten nguoi choi va trai cay da chon
		if (nameField->getString().empty() || selectedFruit.empty())
		{
			auto Size = Director::getInstance()->getVisibleSize();

			auto alert = Label::createWithTTF("Please enter a name and select a fruit !", "fonts/Marker Felt.ttf", 55);
			alert->setPosition(Vec2(Size.width / 2 + 480, Size.height - 1000));
			alert->setColor(Color3B::RED);
			background->addChild(alert, 1);
	
			this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([alert]() {
				alert->removeFromParent();
				}), nullptr));
			return;
		}
		// Luu thong tin nguoi choi 1 vao GameManager
		GameManager::getInstance()->player1.name = nameField->getString();
		GameManager::getInstance()->player1.fruit = selectedFruit;

		Director::getInstance()->replaceScene(TransitionFade::create(0.3f, Player2SetUpScene::createScene()));
		});
}

void Player1SetUpScene::createFruitButtons()
{
	auto Size = Director::getInstance()->getVisibleSize();
	auto Origin = Director::getInstance()->getVisibleOrigin();

	// Vi tri fruit button 
	std::vector<std::pair<std::string, Vec2>> fruitButtons = {
		{"UI/Kiwi.png", Vec2(Size.width / 2 - 175 + Origin.x, Size.height - 560 + Origin.y)},
		{"UI/Orange.png", Vec2(Size.width / 2 + 245 + Origin.x, Size.height - 556 + Origin.y)},
		{"UI/Watermelon.png", Vec2(Size.width / 2 + 675 + Origin.x, Size.height - 560 + Origin.y)},
		{"UI/Lemon.png", Vec2(Size.width / 2 - 181 + Origin.x, Size.height - 817 + Origin.y)},
		{"UI/PassionFruit.png", Vec2(Size.width / 2 + 247 + Origin.x, Size.height - 819 + Origin.y)},
		{"UI/Tomato.png", Vec2(Size.width / 2 + 675 + Origin.x, Size.height - 817 + Origin.y)}
	};

	// Tao nut cho moi loai trai cay
	for (const auto& fruit : fruitButtons)
	{
		auto fruitButton = ui::Button::create(fruit.first);
		fruitButton->setScale(1.0f);
		fruitButton->setPosition(fruit.second);
		fruitButton->addClickEventListener([this, fruit](Ref* sender) {
			SoundManager::getInstance()->playSFX("Sound/Click.mp3");

			std::string fruitPath = fruit.first;
			std::string fruitName = fruitPath.substr(fruitPath.find_last_of("/") + 1);
			fruitName = fruitName.substr(0, fruitName.find("."));

			selectedFruit = fruitName;

			selectedFruitIcon->setTexture(fruitPath);
			selectedFruitIcon->setScale(0.8f);
			selectedFruitIcon->setOpacity(255);
			selectedFruitIcon->setPosition(Vec2(
				Director::getInstance()->getVisibleSize().width / 2 + 630,
				Director::getInstance()->getVisibleSize().height - 140));
			});
		background->addChild(fruitButton);
	}
}