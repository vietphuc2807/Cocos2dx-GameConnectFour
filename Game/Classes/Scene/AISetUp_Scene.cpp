#include "Scene/AISetUp_Scene.h"
#include "Scene/MainMenuGame_Scene.h"
#include "GameplayScene/GameAIScene.h"
#include "Manager/GameManager.h"
#include "Manager/SoundManager.h"

USING_NS_CC;

using namespace cocos2d::ui;

Scene* AISetUpScene::createScene()
{
	return AISetUpScene::create();
}

bool AISetUpScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	
	// UI
	UIGameSetup();

	return true;
}

void AISetUpScene::UIGameSetup() {
    auto size = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    background = Sprite::create("UI/BackgroundAISetUp.png");
	background->setPosition(Vec2(size.width / 2 + origin.x, size.height / 2 + origin.y));
    background->setScale(0.7f);
    this->addChild(background);

    nameField = TextField::create("Enter Player Name", "fonts/Marker Felt.ttf", 55);
    nameField->setAnchorPoint(Vec2(0, 0.5f));
    nameField->setTextColor(Color4B::BLACK);
    nameField->setPosition(Vec2(size.width / 2 - 360, size.height - 150));
    nameField->setMaxLength(7);
    nameField->setMaxLengthEnabled(true);
    background->addChild(nameField);

    selectedFruitIcon = Sprite::create();
    selectedFruitIcon->setOpacity(0);
    background->addChild(selectedFruitIcon);

    createFruitButtons();

    // Back button 
    auto backButton = Button::create("UI/BackButton1.png");
    backButton->setPosition(Vec2(size.width / 2 - 180, size.height - 1400));
    background->addChild(backButton);
    backButton->addClickEventListener([this](Ref*) {
        SoundManager::getInstance()->playSFX("Sound/Click.mp3");
        Director::getInstance()->replaceScene(TransitionFade::create(0.3f, MainMenuGameScene::createScene()));
        });

    // Start button 
    auto startButton = Button::create("UI/StartButton1.png");
    startButton->setPosition(Vec2(size.width / 2 + 640, size.height - 1390));
	startButton->setScale(0.9f);
    background->addChild(startButton);
    startButton->addClickEventListener([this](Ref*) {
        SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		// Kiem tra ten nguoi choi va trai cay da chon chua
        if (nameField->getString().empty() || selectedFruit.empty()) {
            auto alert = Label::createWithTTF("Please enter a name and select a fruit!", "fonts/Marker Felt.ttf", 55);
            alert->setPosition(Vec2(Director::getInstance()->getVisibleSize().width / 2 + 480, 800));
            alert->setColor(Color3B::RED);
            background->addChild(alert, 1);
            this->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([alert]() {
                alert->removeFromParent();
                }), nullptr));
            return;
        }

        GameManager::getInstance()->playerWithAI.name = nameField->getString();
        GameManager::getInstance()->playerWithAI.fruit = selectedFruit;

		// Trai cay cua AI khac nguoi choi 1
        std::vector<std::string> allFruits = { "Kiwi", "Orange", "Watermelon", "Lemon", "PassionFruit", "Tomato" };
        for (auto& fruit : allFruits) {
            if (fruit != selectedFruit) {
                GameManager::getInstance()->playerAI.fruit = fruit;
                break;
            }
        }

        std::vector<std::string> aiNames = { "John", "Mery", "Andy", "James", "Messi", "Neymar", "Wayne", "Lisa", "Pedri", "Rodri", "Yamal" };
        int randomIndex = RandomHelper::random_int(0, static_cast<int>(aiNames.size() - 1));
        GameManager::getInstance()->playerAI.name = aiNames[randomIndex];
        Director::getInstance()->replaceScene(TransitionFade::create(0.3f, GameAIScene::createScene()));

        });
}

void AISetUpScene::createFruitButtons() {
    auto size = Director::getInstance()->getVisibleSize();

	// Vi tri cac nut trai cay
    std::vector<std::pair<std::string, Vec2>> fruits = {
        {"UI/Kiwi.png",         Vec2(size.width / 2 - 175, size.height - 560)},
        {"UI/Orange.png",       Vec2(size.width / 2 + 245, size.height - 556)},
        {"UI/Watermelon.png",   Vec2(size.width / 2 + 675, size.height - 560)},
        {"UI/Lemon.png",        Vec2(size.width / 2 - 181, size.height - 817)},
        {"UI/PassionFruit.png", Vec2(size.width / 2 + 247, size.height - 819)},
        {"UI/Tomato.png",       Vec2(size.width / 2 + 675, size.height - 817)},
    };
	// Tao cac nut trai cay 
    for (const auto& fruit : fruits) {
        auto button = Button::create(fruit.first);
        button->setScale(1.0f);
        button->setPosition(fruit.second);
        button->addClickEventListener([=](Ref*) {
            SoundManager::getInstance()->playSFX("Sound/Click.mp3");

            std::string fruitPath = fruit.first;
            std::string fruitName = fruitPath.substr(fruitPath.find_last_of("/") + 1);
            fruitName = fruitName.substr(0, fruitName.find("."));

            selectedFruit = fruitName;

            selectedFruitIcon->setTexture(fruitPath);
            selectedFruitIcon->setScale(0.8f);
            selectedFruitIcon->setOpacity(255);
            selectedFruitIcon->setPosition(Vec2(size.width / 2 + 630, size.height - 140));
            });
        background->addChild(button);
    }
}

