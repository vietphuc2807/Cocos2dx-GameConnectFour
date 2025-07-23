#include "GameplayScene/Popups.h"
#include "Manager/GameManager.h";
#include "Manager/SoundManager.h";
#include "ui/CocosGUI.h";
#include "Utils.h"
#include "GameNormalScene.h"
#include "Scene/MainMenuGame_Scene.h"

USING_NS_CC;

using namespace cocos2d::ui;

Scene* Popups::createScene()
{
	return Popups::create();
}

bool Popups::init()
{
	if (!Scene::init())
	{
		return false;
	}

	

	return true;
}

// Ham hien popup danh cho hai nguoi choi 
void Popups::showPopupWin(Scene* parent, int currentPlayer)
{
	

	// ----- 2. Lớp Nền Đen -----
	auto dimLayer = LayerColor::create(Color4B(0, 0, 0, 180));
	dimLayer->setContentSize(Director::getInstance()->getVisibleSize());
	dimLayer->setPosition(Vec2::ZERO);
	parent->addChild(dimLayer, 99);

	// ----- 3. Tạo banner -----
	auto banner = Sprite::create("UI/Player_Win.png");
	banner->setPosition(Director::getInstance()->getVisibleSize() / 2);
	banner->setScale(1.0f); // scale thoải mái
	parent->addChild(banner, 100);

	// particle Win 
	
	auto p1 = ParticleSystemQuad::create("particles/firework.plist");
	p1->setPosition(Vec2(550, 800));
	p1->setScale(2.0f);
	p1->setStartColorVar(Color4F(1.0f, 1.0f, 1.0f, 0.0f));
	p1->setStartColor(Color4F(1.0f, 0.0f, 0.0f, 1.0f));
	parent->addChild(p1, 99);
	

	// Win sound
	SoundManager::getInstance()->playSFX("Sound/Win_sound.mp3");

	// ----- 4. Dòng chữ tên người thắng -----
	std::string playerName = (currentPlayer == 1)
		? GameManager::getInstance()->player1.name
		: GameManager::getInstance()->player2.name;

	std::string winText = playerName + " wins!";

	auto alert = Label::createWithTTF(winText, "fonts/Marker Felt.ttf", 80);
	alert->setColor(Color3B::BLACK);
	alert->setPosition(Vec2(400, 400));
	banner->addChild(alert, 100);


	auto playAgainBtn = ui::Button::create("UI/Play_Again.png");
	playAgainBtn->ignoreContentAdaptWithSize(false);
	playAgainBtn->setContentSize(Size(135, 138));
	playAgainBtn->setAnchorPoint(Vec2(0.5f, 0.5f));
	playAgainBtn->setPosition(Vec2(465, 850));
	parent->addChild(playAgainBtn, 101);

	playAgainBtn->addClickEventListener([](Ref* sender) 
		{
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, GameNormalScene::createScene()));
		});

	auto backMenuBtn = ui::Button::create("UI/Home.png");
	backMenuBtn->ignoreContentAdaptWithSize(false);
	backMenuBtn->setContentSize(Size(136, 140));
	backMenuBtn->setAnchorPoint(Vec2(0.5f, 0.5f));
	backMenuBtn->setPosition(Vec2(620, 850));
	parent->addChild(backMenuBtn, 101);

	backMenuBtn->addClickEventListener([](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MainMenuGameScene::createScene()));
		});

	return;


}


// Ham Hien Popup Khi 2 Player Draw
void Popups::showPopupDraw(Scene* parent, int currentPlayer)
{
	auto dimsLayer = LayerColor::create(Color4B(0, 0, 0, 180));
	dimsLayer->setContentSize(Director::getInstance()->getVisibleSize());
	dimsLayer->setPosition(Vec2::ZERO);
	parent->addChild(dimsLayer, 99);


	auto banners = Sprite::create("UI/Draw.png");
	banners->setPosition(Director::getInstance()->getVisibleSize() / 2);
	banners->setScale(1.0f); // scale thoải mái
	parent->addChild(banners, 100);

	auto alerts = Label::createWithTTF("It's a Draw!", "fonts/Marker Felt.ttf", 80);
	alerts->setPosition(Vec2(400, 400));
	alerts->setColor(Color3B::BLACK);
	banners->addChild(alerts, 100);


	// ----- Nút "Play Again" -----
	auto playAgainBtns = ui::Button::create("UI/Play_Again.png");
	playAgainBtns->ignoreContentAdaptWithSize(false);
	playAgainBtns->setContentSize(Size(135, 138));
	playAgainBtns->setAnchorPoint(Vec2(0.5f, 0.5f));
	playAgainBtns->setPosition(Vec2(465, 850));
	parent->addChild(playAgainBtns, 101);

	playAgainBtns->addClickEventListener([](Ref* sender)
		{
			SoundManager::getInstance()->playSFX("Sound/Click.mp3");
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, GameNormalScene::createScene()));
		});

	auto backMenuBtns = ui::Button::create("UI/Home.png");
	backMenuBtns->ignoreContentAdaptWithSize(false);
	backMenuBtns->setContentSize(Size(136, 140));
	backMenuBtns->setAnchorPoint(Vec2(0.5f, 0.5f));
	backMenuBtns->setPosition(Vec2(620, 850));
	parent->addChild(backMenuBtns, 101);

	backMenuBtns->addClickEventListener([](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MainMenuGameScene::createScene()));
		});
}

// Ham hien popup khi thang robot
void Popups::showPopupWinAI(Scene* parent , int currentPlayer) 
{
	
	auto dimsLayer2 = LayerColor::create(Color4B(0, 0, 0, 180));
	dimsLayer2->setContentSize(Director::getInstance()->getVisibleSize());
	dimsLayer2->setPosition(Vec2::ZERO);
	parent->addChild(dimsLayer2, 99);


	auto banners = Sprite::create("UI/Player_WinAI.png");
	banners->setPosition(Director::getInstance()->getVisibleSize() / 2);
	banners->setScale(1.0f); // scale thoải mái
	parent->addChild(banners, 100);

	//Win Sound
	SoundManager::getInstance()->playSFX("Sound/Win_sound.mp3");

	// particle Win 

	auto p1 = ParticleSystemQuad::create("particles/firework.plist");
	p1->setPosition(Vec2(550, 800));
	p1->setScale(2.0f);
	p1->setStartColorVar(Color4F(1.0f, 1.0f, 1.0f, 0.0f));
	p1->setStartColor(Color4F(1.0f, 0.0f, 0.0f, 1.0f));
	parent->addChild(p1, 99);


	// ----- Nút "Play Again" -----
	auto playAgainBtns = ui::Button::create("UI/Play_Again.png");
	playAgainBtns->ignoreContentAdaptWithSize(false);
	playAgainBtns->setContentSize(Size(135, 138));
	playAgainBtns->setAnchorPoint(Vec2(0.5f, 0.5f));
	playAgainBtns->setPosition(Vec2(465, 800));
	parent->addChild(playAgainBtns, 101);

	playAgainBtns->addClickEventListener([](Ref* sender)
		{
			SoundManager::getInstance()->playSFX("Sound/Click.mp3");
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, GameNormalScene::createScene()));
		});

	auto backMenuBtns = ui::Button::create("UI/Home.png");
	backMenuBtns->ignoreContentAdaptWithSize(false);
	backMenuBtns->setContentSize(Size(136, 140));
	backMenuBtns->setAnchorPoint(Vec2(0.5f, 0.5f));
	backMenuBtns->setPosition(Vec2(620, 800));
	parent->addChild(backMenuBtns, 101);

	backMenuBtns->addClickEventListener([](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MainMenuGameScene::createScene()));
		});




}

// Ham hien popup thua khi thua robot
void Popups::showPopupLoseAI(Scene* parent , int currentPlayer) 
{
	auto dimsLayer2 = LayerColor::create(Color4B(0, 0, 0, 180));
	dimsLayer2->setContentSize(Director::getInstance()->getVisibleSize());
	dimsLayer2->setPosition(Vec2::ZERO);
	parent->addChild(dimsLayer2, 99);


	auto banners = Sprite::create("UI/Player_Defeat.png");
	banners->setPosition(Director::getInstance()->getVisibleSize() / 2);
	banners->setScale(1.0f); // scale thoải mái
	parent->addChild(banners, 100);

	//Lose Sound
	SoundManager::getInstance()->playSFX("Sound/Lose_sound.mp3");

	


	/*auto gb1 = Sprite::create("UI/glassbreak.png");
	gb1->setPosition(Vec2(200, 250));
	gb1->setScale(1.5f);
	parent->addChild(gb1, 99);

	SoundManager::getInstance()->playSFX("Sound/GlassBreak.mp3");*/

	for (int i = 0; i < 3; ++i) {
		float delay = i * 0.2f;

		parent->runAction(Sequence::create
		(
			DelayTime::create(delay),
			CallFunc::create([parent, i]()
				{
				auto sprite = Sprite::create("UI/glassbreak.png");

				// vi tri cua 3 spirte
				Vec2 positions[3] = 
				{
					Vec2(300, 250),
					Vec2(800, 1000),
					Vec2(300, 1500)
				};

				sprite->setPosition(positions[i]);
				parent->addChild(sprite);

				SoundManager::getInstance()->playSFX("Sound/GlassBreak.mp3");
				}),
			nullptr
		));
	}



	


	// ----- Nút "Play Again" -----
	auto playAgainBtns = ui::Button::create("UI/Play_Again.png");
	playAgainBtns->ignoreContentAdaptWithSize(false);
	playAgainBtns->setContentSize(Size(135, 138));
	playAgainBtns->setAnchorPoint(Vec2(0.5f, 0.5f));
	playAgainBtns->setPosition(Vec2(465, 850));
	parent->addChild(playAgainBtns, 101);

	playAgainBtns->addClickEventListener([](Ref* sender)
		{
			SoundManager::getInstance()->playSFX("Sound/Click.mp3");
			Director::getInstance()->replaceScene(TransitionFade::create(0.5f, GameNormalScene::createScene()));
		});

	auto backMenuBtns = ui::Button::create("UI/Home.png");
	backMenuBtns->ignoreContentAdaptWithSize(false);
	backMenuBtns->setContentSize(Size(136, 140));
	backMenuBtns->setAnchorPoint(Vec2(0.5f, 0.5f));
	backMenuBtns->setPosition(Vec2(620, 850));
	parent->addChild(backMenuBtns, 101);

	backMenuBtns->addClickEventListener([](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		Director::getInstance()->replaceScene(TransitionFade::create(0.5f, MainMenuGameScene::createScene()));
		});

}

// rainbow //
void HSVtoRGB(float h, float s, float v, float& r, float& g, float& b)
{
	int i = int(h / 60.0f) % 6;
	float f = h / 60.0f - i;
	float p = v * (1 - s);
	float q = v * (1 - f * s);
	float t = v * (1 - (1 - f) * s);
	switch (i)
	{
	case 0: r = v; g = t; b = p; break;
	case 1: r = q; g = v; b = p; break;
	case 2: r = p; g = v; b = t; break;
	case 3: r = p; g = q; b = v; break;
	case 4: r = t; g = p; b = v; break;
	case 5: r = v; g = p; b = q; break;
	}
}