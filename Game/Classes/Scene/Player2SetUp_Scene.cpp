#include "Scene/Player1SetUp_Scene.h"
#include "Scene/Player2SetUp_Scene.h"
#include "Scene/SelectModeScene.h"
#include "Manager/GameManager.h"
#include "Manager/SoundManager.h"


USING_NS_CC;

using namespace cocos2d::ui;

Scene* Player2SetUpScene::createScene()
{
	return Player2SetUpScene::create();
}
bool Player2SetUpScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	// UI 
	UIGameSetup();

	return true;
}

void Player2SetUpScene::UIGameSetup()
{
    auto Size = Director::getInstance()->getVisibleSize();
    auto Origin = Director::getInstance()->getVisibleOrigin();

    background = Sprite::create("UI/Background2PlayerSetUp.png");
    background->setPosition(Vec2(Size.width / 2 + Origin.x, Size.height / 2 + Origin.y));
    background->setScale(0.7f);
    this->addChild(background);

    nameField = TextField::create("Enter Player Name", "fonts/Marker Felt.ttf", 55);
    nameField->setAnchorPoint(Vec2(0, 0.5f));
    nameField->setTextColor(Color4B::BLACK);
    nameField->setPosition(Vec2(Size.width / 2 - 360, Size.height - 150));
    nameField->setMaxLength(7);
    nameField->setMaxLengthEnabled(true);
    background->addChild(nameField);

    selectedFruitIcon = Sprite::create();
    selectedFruitIcon->setOpacity(0);
    background->addChild(selectedFruitIcon);

    createFruitButtons();

    // Back button 
    auto backButton = Button::create("UI/BackButton1.png");
    backButton->setPosition(Vec2(Size.width / 2 - 180, Size.height - 1400));
    background->addChild(backButton);
    backButton->addClickEventListener([](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
        Director::getInstance()->replaceScene(TransitionFade::create(0.3f, Player1SetUpScene::createScene()));
        });

    // Next button 
    auto nextButton = Button::create("UI/NextButton1.png");
    nextButton->setPosition(Vec2(Size.width / 2 + 640, Size.height - 1390));
    nextButton->setScale(0.9f);
    background->addChild(nextButton);
    nextButton->addClickEventListener([this](Ref* sender) {
        SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		// Kiem tra ten nguoi choi va trai cay da chon
        if (nameField->getString().empty() || selectedFruit.empty()) {
            auto Size = Director::getInstance()->getVisibleSize();
            auto Origin = Director::getInstance()->getVisibleOrigin();

            auto alert = Label::createWithTTF("Please enter a name and select a fruit !", "fonts/Marker Felt.ttf", 55);
            alert->setPosition(Vec2(Size.width / 2 + 480, Size.height - 1000));
            alert->setColor(Color3B::RED);
            background->addChild(alert, 1);
            this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([alert]() {
                alert->removeFromParent();
                }), nullptr));
            return;
        }
		// Luu thong tin nguoi choi 2 vao GameManager
        GameManager::getInstance()->player2.name = nameField->getString();
        GameManager::getInstance()->player2.fruit = selectedFruit;

        Director::getInstance()->replaceScene(TransitionFade::create(0.3f, SelectModeScene::createScene())); 
        });
}

void Player2SetUpScene::createFruitButtons()
{
    auto Size = Director::getInstance()->getVisibleSize();
    auto Origin = Director::getInstance()->getVisibleOrigin();

    std::string fruit1 = GameManager::getInstance()->player1.fruit;
	// Vi tri cac nut trai cay
    std::vector<std::pair<std::string, Vec2>> fruitButtons = {
        {"UI/Kiwi.png", Vec2(Size.width / 2 - 175, Size.height - 560)},
        {"UI/Orange.png", Vec2(Size.width / 2 + 245, Size.height - 556)},
        {"UI/Watermelon.png", Vec2(Size.width / 2 + 675, Size.height - 560)},
        {"UI/Lemon.png", Vec2(Size.width / 2 - 181, Size.height - 817)},
        {"UI/PassionFruit.png", Vec2(Size.width / 2 + 247, Size.height - 819)},
        {"UI/Tomato.png", Vec2(Size.width / 2 + 675, Size.height - 817)}
    };
	// Tao cac nut moi loai trai cay
    for (const auto& fruit : fruitButtons)
    {
        std::string fruitPath = fruit.first;
        std::string fruitName = fruitPath.substr(fruitPath.find_last_of("/") + 1);
        fruitName = fruitName.substr(0, fruitName.find("."));

        auto fruitButton = Button::create(fruit.first);
        fruitButton->setScale(1.0f);
        fruitButton->setPosition(fruit.second);

		// Vo hieu hoa trai cay ma nguoi choi 1 da chon 
		if (fruitName == fruit1) {
			fruitButton->setEnabled(false);
			fruitButton->setOpacity(128); 
		}

        fruitButton->addClickEventListener([this, fruit, fruitName](Ref* sender) {
            selectedFruit = fruitName;
            selectedFruitIcon->setTexture(fruit.first);
            selectedFruitIcon->setScale(0.8f);
            selectedFruitIcon->setOpacity(255);
            selectedFruitIcon->setPosition(Vec2(
                Director::getInstance()->getVisibleSize().width / 2 + 630,
                Director::getInstance()->getVisibleSize().height - 140));
            });
        background->addChild(fruitButton);
    }
}