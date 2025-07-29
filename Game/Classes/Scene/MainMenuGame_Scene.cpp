#include "Scene/MainMenuGame_Scene.h"
#include "Scene/Player1SetUp_Scene.h"
#include "Scene/AISetUp_Scene.h"
#include "Manager/StatsManager.h"
#include "audio/include/AudioEngine.h"
#include "Manager/SoundManager.h"

USING_NS_CC;

Scene* MainMenuGameScene::createScene()
{
	return MainMenuGameScene::create();
}

bool MainMenuGameScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	// Preload file âm thanh nen menu game va sfx
	AudioEngine::preload("Sound/Click.mp3");
	AudioEngine::preload("Sound/SL_head.mp3");

	// Cai dat nguoi dung cho am luong nhac va sfx
	float musicVolume = UserDefault::getInstance()->getFloatForKey("music_volume", 0.5f);
	float sfxVolume = UserDefault::getInstance()->getFloatForKey("sfx_volume", 0.5f);

	// Khoi tao am thanh nhac nen menu game 
	SoundManager::getInstance()->playBackgroundMusic("Sound/SL_head.mp3", musicVolume);
	SoundManager::getInstance()->setBackgroundMusicVolume(musicVolume);

	// UI 
	MainMenuGameScene::UIGameSetup();

	// Cap nhat spam la cay
	this->schedule([=](float dt) {
		SpawnLeaf();
		}, 1.3f, "spawn_leaf_key");

	return true;
}

void MainMenuGameScene::UIGameSetup()
{
	auto Size = Director::getInstance()->getVisibleSize();
	auto Origin = Director::getInstance()->getVisibleOrigin();

	// Background 
	backgroundMainMenu = Sprite::create("UI/MainMenuGame.png");
	backgroundMainMenu->setPosition(Vec2(Size.width / 2 + Origin.x, Size.height / 2 + Origin.y));

	backgroundMainMenu->setScale(0.67f);

	backgroundMainMenu->getTexture()->setAntiAliasTexParameters();

	this->addChild(backgroundMainMenu);

	// InforButton 
	auto inforButton = ui::Button::create("UI/InforButton1.png", "UI/InforButton2.png");
	inforButton->setPosition(Vec2(Size.width - 865 + Origin.x, Size.height - 1707 + Origin.y));
	backgroundMainMenu->addChild(inforButton);

	inforButton->addClickEventListener([=](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		this->showInfoPopup();
		});

	// OnePlayer setup
	auto onePlayerButton = ui::Button::create("UI/OnePlayerButton1.png", "UI/OnePlayerButton.png");
	onePlayerButton->setPosition(Vec2(Size.width - 265 + Origin.x, Size.height - 180 + Origin.y));
	backgroundMainMenu->addChild(onePlayerButton);

	onePlayerButton->addClickEventListener([=](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		this->showPauseMenuPopupOne();
		});

	// TwoPlayer 
	auto twoPlayerButton = ui::Button::create("UI/TwoPlayerButton1.png", "UI/TwoPlayerButton2.png");
	twoPlayerButton->setPosition(Vec2(Size.width - 270 + Origin.x, Size.height - 450 + Origin.y));
	backgroundMainMenu->addChild(twoPlayerButton);

	twoPlayerButton->addClickEventListener([=](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		this->showPauseMenuPopupTwo();
		});

	// Stats 
	auto statsButton = ui::Button::create("UI/statsButton.png", "UI/statsButton1.png");
	statsButton->setPosition(Vec2(Size.width - 320 + Origin.x, Size.height - 750 + Origin.y));
	backgroundMainMenu->addChild(statsButton);

	statsButton->addClickEventListener([=](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		this->showStatsPopup();
		});

	// Options 
	auto optionsButton = ui::Button::create("UI/OptionsButton1.png", "UI/OptionsButton.png");
	optionsButton->setPosition(Vec2(Size.width - 273 + Origin.x, Size.height - 1020 + Origin.y));
	backgroundMainMenu->addChild(optionsButton);

	optionsButton->addClickEventListener([=](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		this->showOptionsPopup();
		});
}

void MainMenuGameScene::SpawnLeaf() {
	auto Size = Director::getInstance()->getVisibleSize();
	auto Origin = Director::getInstance()->getVisibleOrigin();

	// Sprite la cay
	leaf = Sprite::create("UI/Leaf.png");
	float startX = RandomHelper::random_real(0.0f, Size.width);
	float scale = RandomHelper::random_real(0.3f, 0.8f);
	leaf->setScale(scale);
	leaf->setPosition(Vec2(startX, Size.height + 1000));
	backgroundMainMenu->addChild(leaf);

	// Animation roi la cay
	float duration = RandomHelper::random_real(6.0f, 8.0f);

	ccBezierConfig bezier;
	bezier.controlPoint_1 = Vec2(startX + RandomHelper::random_real(-270.0f, 270.0f), Size.height * 0.66f);
	bezier.controlPoint_2 = Vec2(startX + RandomHelper::random_real(-270.0f, 270.0f), Size.height * 0.33f);
	bezier.endPosition = Vec2(startX + RandomHelper::random_real(-170.0f, 170.0f), -100.0f);

	auto bezierTo = BezierTo::create(duration, bezier);

	float rotateAngle = (RandomHelper::random_int(0, 1) == 0 ? 1 : -1) * RandomHelper::random_real(360.0f, 1080.0f);
	auto rotate = RotateBy::create(duration, rotateAngle);

	auto fallAndRotate = Spawn::createWithTwoActions(bezierTo, rotate);	

	auto fade = FadeOut::create(0.5f);
	auto remove = RemoveSelf::create();

	auto sequence = Sequence::create(fallAndRotate, fade, remove, nullptr);
	leaf->runAction(sequence);
}

void MainMenuGameScene::showInfoPopup() {
	auto Size = Director::getInstance()->getVisibleSize();
	auto Origin = Director::getInstance()->getVisibleOrigin();

	// Tao popup thong tin game 
	auto dimLayer = LayerColor::create(Color4B(0, 0, 0, 180));
	this->addChild(dimLayer, 100);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
		};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, dimLayer);

	auto popup = Sprite::create("UI/InforGame.png");
	popup->setPosition(Vec2(Size.width / 2 + Origin.x, Size.height / 2 + Origin.y));
	popup->setScale(0.7f);
	dimLayer->addChild(popup);

	// Close Button 
	auto closeButton = ui::Button::create("UI/CloseButton1.png", "UI/CloseButton2.png");
	closeButton->setPosition(Vec2(popup->getContentSize().width - 212, popup->getContentSize().height - 210));
	popup->addChild(closeButton);

	closeButton->addClickEventListener([=](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		dimLayer->removeFromParent();
		});
}

void MainMenuGameScene::showPauseMenuPopupOne() {
	auto Size = Director::getInstance()->getVisibleSize();
	auto Origin = Director::getInstance()->getVisibleOrigin();

	// Tao popup pause 
	auto dimLayer = LayerColor::create(Color4B(0, 0, 0, 200));
	this->addChild(dimLayer, 100);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
		};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, dimLayer);

	auto popup = Sprite::create("UI/PauseMenuGame.png");
	popup->setPosition(Vec2(Size.width / 2 + Origin.x, Size.height / 2 + Origin.y));
	popup->setScale(0.6f);
	dimLayer->addChild(popup);

	// NewGame button 
	auto newGameButton = ui::Button::create("UI/NewGameButton1.png", "UI/NewGameButton.png");
	newGameButton->setPosition(Vec2(Size.width / 2 + 95 + Origin.x, Size.height / 2 + 75 + Origin.y));
	popup->addChild(newGameButton);

	newGameButton->addClickEventListener([=](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");

		dimLayer->removeFromParent();
		Director::getInstance()->replaceScene(TransitionFade::create(0.3f, AISetUpScene::createScene()));
		});

	// MainMenu button 
	auto mainMenuButton = ui::Button::create("UI/MainMenuButton1.png", "UI/MainMenuButton.png");
	mainMenuButton->setPosition(Vec2(Size.width / 2 + 107 + Origin.x, Size.height / 2 - 650 + Origin.y));
	popup->addChild(mainMenuButton);

	mainMenuButton->addClickEventListener([=](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		dimLayer->removeFromParent();
		});
} // con nut resume lam sau (luu du lieu game)

void MainMenuGameScene::showPauseMenuPopupTwo() {
	auto Size = Director::getInstance()->getVisibleSize();
	auto Origin = Director::getInstance()->getVisibleOrigin();

	// Tao popup pause 
	auto dimLayer = LayerColor::create(Color4B(0, 0, 0, 200));
	this->addChild(dimLayer, 100);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event) {
		return true; 
		};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, dimLayer);

	auto popup = Sprite::create("UI/PauseMenuGame.png");
	popup->setPosition(Vec2(Size.width / 2 + Origin.x, Size.height / 2 + Origin.y));
	popup->setScale(0.6f);
	dimLayer->addChild(popup);

	// NewGame button 
	auto newGameButton = ui::Button::create("UI/NewGameButton1.png", "UI/NewGameButton.png");
	newGameButton->setPosition(Vec2(Size.width / 2 + 95 + Origin.x, Size.height / 2 + 75 + Origin.y));
	popup->addChild(newGameButton);

	newGameButton->addClickEventListener([=](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");

		dimLayer->removeFromParent();
		Director::getInstance()->replaceScene(TransitionFade::create(0.3f, Player1SetUpScene::createScene()));
		});

	// MainMenu button 
	auto mainMenuButton = ui::Button::create("UI/MainMenuButton1.png", "UI/MainMenuButton.png");
	mainMenuButton->setPosition(Vec2(Size.width / 2 + 107 + Origin.x, Size.height / 2 - 650 + Origin.y));
	popup->addChild(mainMenuButton);

	mainMenuButton->addClickEventListener([=](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		dimLayer->removeFromParent();
		});
} // con nut resume lam sau (luu du lieu game)

void MainMenuGameScene::showStatsPopup() {
	auto Size = Director::getInstance()->getVisibleSize();
	auto Origin = Director::getInstance()->getVisibleOrigin();
	// Tao popup thong ke 
	auto dimLayer = LayerColor::create(Color4B(0, 0, 0, 180));
	this->addChild(dimLayer, 100);
	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
		};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, dimLayer);
	auto popup = Sprite::create("UI/StatsGame.png");
	popup->setPosition(Vec2(Size.width / 2 + Origin.x, Size.height / 2 + Origin.y));
	popup->setScale(0.6f);
	dimLayer->addChild(popup);

	// Thong ke
	int wins = StatsManager::getInstance()->getWins();
	int losses = StatsManager::getInstance()->getLoses();
	int draws = StatsManager::getInstance()->getDraws();
	int totalGames = wins + losses + draws;
		
	// Tao cac label thong ke
	auto createNumberLabel = [&](const std::string& text, float offsetY) {
		auto label = Label::createWithTTF(text, "fonts/arial.ttf", 65);
		label->setAnchorPoint(Vec2(0, 0.5f));
		label->setColor(Color3B::BLACK);
		label->enableOutline(Color4B::BLACK, 1);
		label->setPosition(Vec2(popup->getContentSize().width / 2 + 350, popup->getContentSize().height / 2 + offsetY));
		popup->addChild(label);
		};

	createNumberLabel(std::to_string(totalGames), 350);  // Total games
	createNumberLabel(std::to_string(wins), 200);         // Wins
	createNumberLabel(std::to_string(losses), 36);      // Losses
	createNumberLabel(std::to_string(draws), -120);      // Draws

	// Back button 
	auto backButton = ui::Button::create("UI/BackButton1.png", "UI/BackButton2.png");
	backButton->setPosition(Vec2(popup->getContentSize().width / 2, popup->getContentSize().height / 2 - 450));
	popup->addChild(backButton);

	backButton->addClickEventListener([=](Ref* sender) {
		dimLayer->removeFromParent();
		});

}

void MainMenuGameScene::showOptionsPopup() {
	auto Size = Director::getInstance()->getVisibleSize();
	auto Origin = Director::getInstance()->getVisibleOrigin();

	// Tao popup options
	auto dimLayer = LayerColor::create(Color4B(0, 0, 0, 180));
	this->addChild(dimLayer, 100);

	auto touchListener = EventListenerTouchOneByOne::create();
	touchListener->setSwallowTouches(true);
	touchListener->onTouchBegan = [](Touch* touch, Event* event) {
		return true;
		};
	_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, dimLayer);

	auto popup = Sprite::create("UI/OptionsGame.png");
	popup->setPosition(Vec2(Size.width / 2 + Origin.x, Size.height / 2 + Origin.y));
	popup->setScale(0.6f);
	dimLayer->addChild(popup);

	auto popupSize = popup->getContentSize();

	// Am luong goc
	float originalMusicVolume = UserDefault::getInstance()->getFloatForKey("music_volume", 0.5f);
	float originalSFXVolume = UserDefault::getInstance()->getFloatForKey("sfx_volume", 0.5f);

	float* tempMusicVolume = new float(originalMusicVolume);
	float* tempSFXVolume = new float(originalSFXVolume);

	// Sound music slider 
	auto musicSlider = ui::Slider::create();
	musicSlider->loadBarTexture("UI/BarBackground.png");
	musicSlider->loadSlidBallTextures("UI/Ball.png");
	musicSlider->loadProgressBarTexture("UI/LoadBar.png");
	musicSlider->setPosition(Vec2(Size.width / 2 + 460 + Origin.x, Size.height / 2 + 330 + Origin.y));
	musicSlider->setScale(1.6f);
	musicSlider->setScale9Enabled(true);
	musicSlider->setContentSize(cocos2d::Size(363, 38));
	musicSlider->setPercent(static_cast<int>(originalMusicVolume * 100));
	popup->addChild(musicSlider);

	musicSlider->addEventListener([=](Ref* sender, ui::Slider::EventType type) {
		*tempMusicVolume = musicSlider->getPercent() / 100.0f;
		SoundManager::getInstance()->setBackgroundMusicVolume(*tempMusicVolume);
		});

	// Sound SFX slider 
	auto sfxSlider = ui::Slider::create();
	sfxSlider->loadBarTexture("UI/BarBackground.png");
	sfxSlider->loadSlidBallTextures("UI/Ball.png");
	sfxSlider->loadProgressBarTexture("UI/LoadBar.png");
	sfxSlider->setPosition(Vec2(Size.width / 2 + 460 + Origin.x, Size.height / 2 + 120 + Origin.y));
	sfxSlider->setScale(1.6f);
	sfxSlider->setScale9Enabled(true);
	sfxSlider->setContentSize(cocos2d::Size(363, 38));
	sfxSlider->setPercent(static_cast<int>(originalSFXVolume * 100));
	popup->addChild(sfxSlider);

	sfxSlider->addEventListener([=](Ref* sender, ui::Slider::EventType type) {
		*tempSFXVolume = sfxSlider->getPercent() / 100.0f;
		});

	// Back button 
	auto backButton = ui::Button::create("UI/BackButton1.png", "UI/BackButton2.png");
	backButton->setPosition(Vec2(Size.width / 2 - 10 + Origin.x, Size.height / 2 - 378 + Origin.y));
	popup->addChild(backButton);

	backButton->addClickEventListener([=](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");

		SoundManager::getInstance()->setBackgroundMusicVolume(originalMusicVolume);

		dimLayer->removeFromParent();
		delete tempMusicVolume;
		delete tempSFXVolume;
		});

	// OK button 
	auto okButton = ui::Button::create("UI/OkButton1.png", "UI/OkButton2.png");
	okButton->setPosition(Vec2(Size.width / 2 + 585 + Origin.x, Size.height / 2 - 378 + Origin.y));
	popup->addChild(okButton);

	okButton->addClickEventListener([=](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");

		UserDefault::getInstance()->setFloatForKey("music_volume", *tempMusicVolume);
		UserDefault::getInstance()->setFloatForKey("sfx_volume", *tempSFXVolume);
		UserDefault::getInstance()->flush();

		dimLayer->removeFromParent();
		delete tempSFXVolume;
		delete tempMusicVolume;
		});
}



