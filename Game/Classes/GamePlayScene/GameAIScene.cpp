#include "GameplayScene/GameAIScene.h"
#include "Scene/MainMenuGame_Scene.h"
#include "Manager/GameManager.h";
#include "Manager/SoundManager.h";
#include "GamePlayScene/AI.h"
#include "Utils/Utils.h"
#include <vector>
#include "GamePlayScene/Popups.h"

USING_NS_CC;

using namespace cocos2d::ui;

Scene* GameAIScene::createScene()
{
	return GameAIScene::create();
}

bool GameAIScene::init()
{
	srand(time(nullptr));

	if (!Scene::init())
	{
		return false;
	}
	// Dung nhac nen o menu game 
	SoundManager::getInstance()->stopBackgroundMusic();
	// Bat dau nhac nen cho scene gameplay
	float musicVolume = UserDefault::getInstance()->getFloatForKey("music_volume", 0.5f);
	SoundManager::getInstance()->playBackgroundMusic("Sound/Caketown 1.mp3", musicVolume);
	SoundManager::getInstance()->setBackgroundMusicVolume(musicVolume);
	// Player 1 bat dau
	currentPlayer = 1;
	// Reset lai toan bo ban choi
	memset(board, 0, sizeof(board));
	// Reset lai cac sprite cua quan co
	memset(pieceSprites, 0, sizeof(pieceSprites));
	// UI
	initUI();
	// Khoi tao bang
	initBoard();
	// Cap nhat label den luot nguoi choi nao
	updatePlayerInfoUI();
	// Label bat dau game 
	startCountdown();
	return true;
}
void GameAIScene::initUI()
{
	auto Size = Director::getInstance()->getVisibleSize();
	auto Origin = Director::getInstance()->getVisibleOrigin();

	// Background
	BackgroundGame = Sprite::create("UI/BackgroundGame.png");
	BackgroundGame->setPosition(Vec2(Size.width / 2 + Origin.x, Size.height / 2 + Origin.y));
	BackgroundGame->setScale(0.67f);
	this->addChild(BackgroundGame);

	// Thong tin nguoi choi da duoc luu vao GameManager
	auto playerName = GameManager::getInstance()->playerWithAI.name;
	auto aiName = GameManager::getInstance()->playerAI.name;

	playerNameLabel = Label::createWithTTF(playerName, "fonts/Marker Felt.ttf", 70);
	playerNameLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	playerNameLabel->setPosition(Vec2(Origin.x + 300, Size.height + 600));
	playerNameLabel->setColor(Color3B::BLACK);
	BackgroundGame->addChild(playerNameLabel);

	playerAINameLabel = Label::createWithTTF(aiName, "fonts/Marker Felt.ttf", 70);
	playerAINameLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	playerAINameLabel->setPosition(Vec2(Origin.x + 1300, Size.height + 600));
	playerAINameLabel->setColor(Color3B::BLACK);
	BackgroundGame->addChild(playerAINameLabel);

	// Label luot choi
	playerTurnLabel = Label::createWithTTF("Player's Turn...", "fonts/Marker Felt.ttf", 80);
	playerTurnLabel->setPosition(Vec2(Size.width / 2 + Origin.x - 450, Size.height + 800));
	playerTurnLabel->setAnchorPoint(Vec2(0, 0.5f));
	playerTurnLabel->setColor(Color3B::YELLOW);
	BackgroundGame->addChild(playerTurnLabel);

	// Menu Button
	auto menuButton = Button::create("UI/PauseGame.png");
	menuButton->setPosition(Vec2(Size.width / 2 + Origin.x + 250, Size.height / 2 + Origin.y - 750));
	menuButton->addClickEventListener([this](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		showPauseMenuPopup();
		});
	BackgroundGame->addChild(menuButton);

	// Sound Button 
	soundButton = Button::create("UI/SoundOn.png");
	soundButton->setPosition(Vec2(Size.width / 2 + Origin.x + 260, Size.height / 2 + Origin.y + 1790));
	soundButton->addClickEventListener([=](Ref* sender) mutable {
		isSoundOn = !isSoundOn;

		if (isSoundOn) {
			SoundManager::getInstance()->resumeAll();
			soundButton->loadTextureNormal("UI/SoundOn.png");
		}
		else {
			SoundManager::getInstance()->pauseAll();
			soundButton->loadTextureNormal("UI/SoundOff.png");
		}
		});
	BackgroundGame->addChild(soundButton);
}

void GameAIScene::showPauseMenuPopup() {
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

	// Resume button 
	auto resumeButton = Button::create("UI/Resume.png");
	resumeButton->setPosition(Vec2(Size.width / 2 + Origin.x, Size.height / 2 + Origin.y));
	resumeButton->setScale(0.6f);
	resumeButton->addClickEventListener([this, dimLayer](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		dimLayer->removeFromParent();
		});
	dimLayer->addChild(resumeButton);

	// Main menu button
	auto mainMenuButton = Button::create("UI/MainMenu.png");
	mainMenuButton->setPosition(Vec2(Size.width / 2 + Origin.x, Size.height / 2 + Origin.y - 250));
	mainMenuButton->setScale(0.6f);
	mainMenuButton->addClickEventListener([this, dimLayer](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		dimLayer->removeFromParent();
		Director::getInstance()->replaceScene(TransitionFade::create(0.3f, MainMenuGameScene::createScene()));
		});
	dimLayer->addChild(mainMenuButton);

	// New game button
	auto newGameButton = Button::create("UI/NewGame.png");
	newGameButton->setPosition(Vec2(Size.width / 2 + Origin.x, Size.height / 2 + Origin.y + 250));
	newGameButton->setScale(0.6f);
	newGameButton->addClickEventListener([this, dimLayer](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		dimLayer->removeFromParent();
		Director::getInstance()->replaceScene(TransitionFade::create(0.3f, GameAIScene::createScene()));
		});
	dimLayer->addChild(newGameButton);
}

void GameAIScene::initBoard() {

	// Co dinh vi tri tung o cua quan co 
	cellPositions[0][0] = Vec2(245, 572);
	cellPositions[0][1] = Vec2(515, 572);
	cellPositions[0][2] = Vec2(808, 572);
	cellPositions[0][3] = Vec2(1095, 572);
	cellPositions[0][4] = Vec2(1370, 572);

	cellPositions[1][0] = Vec2(245, 835);
	cellPositions[1][1] = Vec2(515, 835);
	cellPositions[1][2] = Vec2(808, 835);
	cellPositions[1][3] = Vec2(1095, 835);
	cellPositions[1][4] = Vec2(1370, 835);

	cellPositions[2][0] = Vec2(245, 1102);
	cellPositions[2][1] = Vec2(515, 1102);
	cellPositions[2][2] = Vec2(808, 1102);
	cellPositions[2][3] = Vec2(1095, 1102);
	cellPositions[2][4] = Vec2(1370, 1102);

	cellPositions[3][0] = Vec2(245, 1367);
	cellPositions[3][1] = Vec2(515, 1367);
	cellPositions[3][2] = Vec2(808, 1367);
	cellPositions[3][3] = Vec2(1095, 1367);
	cellPositions[3][4] = Vec2(1370, 1367);

	cellPositions[4][0] = Vec2(245, 1632);
	cellPositions[4][1] = Vec2(515, 1632);
	cellPositions[4][2] = Vec2(805, 1632);
	cellPositions[4][3] = Vec2(1095, 1632);
	cellPositions[4][4] = Vec2(1370, 1632);

	cellPositions[5][0] = Vec2(245, 1895);
	cellPositions[5][1] = Vec2(515, 1895);
	cellPositions[5][2] = Vec2(805, 1895);
	cellPositions[5][3] = Vec2(1095, 1895);
	cellPositions[5][4] = Vec2(1370, 1895);

	// Tao vung cham cho tung cot
	for (int col = 0; col < 5; col++) {
		auto listener = EventListenerTouchOneByOne::create();
		listener->onTouchBegan = [=](Touch* touch, Event* event) {
			SoundManager::getInstance()->playSFX("Sound/Click.mp3");
			Vec2 touchLoc = BackgroundGame->convertToNodeSpace(touch->getLocation());
			Vec2 cell = cellPositions[5][col];
			if (fabs(touchLoc.x - cell.x) < 80) {
				if (currentPlayer == 1 && isGameStarted)
					dropPiece(col);
				return true;
			}
			return false;
			};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}
}

void GameAIScene::dropPiece(int col)
{
	// Kiem tra neu game chua bat dau thi khong cho danh
	if (!isGameStarted)
		return;

	for (int row = 0; row < 6; row++)
	{
		if (board[row][col] == 0)
		{
			// Ghi nhan nguoi choi hien tai
			board[row][col] = currentPlayer;

			std::string fruit = (currentPlayer == 1) ? GameManager::getInstance()->playerWithAI.fruit : GameManager::getInstance()->playerAI.fruit;
			std::string spritePath = "UI/" + fruit + ".png";

			auto piece = Sprite::create(spritePath);
			piece->setScale(1.1f);

			Vec2 targetPos = cellPositions[row][col];
			// Bat dau roi tu tren xuong 
			Vec2 startPos = Vec2(targetPos.x, cellPositions[5][col].y + 150);
			piece->setPosition(startPos);
			BackgroundGame->addChild(piece);
			pieceSprites[row][col] = piece;

			// Hieu ung roi 
			auto fall = MoveTo::create(1.0f, targetPos);
			auto ease = EaseBounceOut::create(fall);
			piece->runAction(ease);

			// Kiem tra thang 
			if (Utils::checkWin(board, row, col)) {
				if (currentPlayer == 1) {
					// Ng??i ch?i th?ng
					Popups::showPopupWinAI(this, currentPlayer);
				}
				else {
					// AI th?ng ? ng??i ch?i thua
					Popups::showPopupLoseAI(this, currentPlayer);
				}
				return;
			}

			// Kiem tra hoa
			if (Utils::isBoardFull(board)) {
				Popups::showPopupDrawAI(this, currentPlayer);
			}

			// Doi luot choi
			switchTurn();
			if (currentPlayer == 2) {
				this->runAction(Sequence::create(
					DelayTime::create(0.5f),
					CallFunc::create([=]() {
						runAIMove();
						}),
					nullptr
				));
			}
			return;
		}
	}
}

void GameAIScene::updatePlayerInfoUI() {
	std::string name = currentPlayer == 1 ?
		GameManager::getInstance()->playerWithAI.name :
		GameManager::getInstance()->playerAI.name;

	playerTurnLabel->setString(name + "'s Turn...");
}

void GameAIScene::startCountdown() {
	auto size = Director::getInstance()->getVisibleSize();

	auto countdownLabel = Label::createWithTTF("Ready", "fonts/Marker Felt.ttf", 150);
	countdownLabel->setColor(Color3B::YELLOW);
	countdownLabel->setPosition(Vec2(-500, size.height / 2));
	this->addChild(countdownLabel, 999);

	std::vector<std::string> countdownValues = { "Ready", "Start!" };
	Vector<FiniteTimeAction*> sequence;

	for (int i = 0; i < countdownValues.size(); ++i) {
		auto appear = Spawn::create(
			MoveTo::create(0.3f, Vec2(size.width / 2, size.height / 2)),
			FadeIn::create(0.3f),
			nullptr
		);
		auto delay = DelayTime::create(0.4f);
		auto slideOut = Spawn::create(
			MoveTo::create(0.3f, Vec2(size.width + 500, size.height / 2)),
			FadeOut::create(0.3f),
			nullptr
		);
		auto updateText = CallFunc::create([=]() {
			countdownLabel->setString(countdownValues[i]);
			countdownLabel->setOpacity(0);
			countdownLabel->setPosition(Vec2(-500, size.height / 2));
			});

		sequence.pushBack(updateText);
		sequence.pushBack(appear);
		sequence.pushBack(delay);
		sequence.pushBack(slideOut);
	}

	sequence.pushBack(CallFunc::create([=]() {
		countdownLabel->removeFromParent();
		isGameStarted = true;
		}));

	auto fullSequence = Sequence::create(sequence);
	countdownLabel->runAction(fullSequence);
}

void GameAIScene::switchTurn()
{
	currentPlayer = (currentPlayer == 1) ? 2 : 1;
	updatePlayerInfoUI();
}

void GameAIScene::runAIMove() {
	AI aiLogic(board);
	int col = aiLogic.calculateMove();

	if (col != -1) {
		dropPiece(col);
	}
}