#include "GamePlayScene/GameRiotScene.h"
#include "Scene/MainMenuGame_Scene.h"
#include "Manager/GameManager.h";
#include "Manager/SoundManager.h";
#include "Utils/Utils.h"
#include "ui/CocosGUI.h"
#include "GamePlayScene/Popups.h"

using namespace cocos2d::ui;
USING_NS_CC;


Scene* GameRiotScene::createScene()
{
	return GameRiotScene::create();
}

bool GameRiotScene::init()
{
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
void GameRiotScene::initUI()
{
	auto Size = Director::getInstance()->getVisibleSize();
	auto Origin = Director::getInstance()->getVisibleOrigin();
	// Background
	BackgroundGame = Sprite::create("UI/BackgroundGame.png");
	BackgroundGame->setPosition(Vec2(Size.width / 2 + Origin.x, Size.height / 2 + Origin.y));
	BackgroundGame->setScale(0.67f);
	this->addChild(BackgroundGame);

	// Thong tin nguoi choi da duoc luu vao GameManager
	auto player1Name = GameManager::getInstance()->player1.name;
	auto player2Name = GameManager::getInstance()->player2.name;

	player1NameLabel = Label::createWithTTF(player1Name, "fonts/Marker Felt.ttf", 70);
	player1NameLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	player1NameLabel->setPosition(Vec2(Origin.x + 300, Size.height + 600));
	player1NameLabel->setColor(Color3B::BLACK);
	BackgroundGame->addChild(player1NameLabel);

	player2NameLabel = Label::createWithTTF(player2Name, "fonts/Marker Felt.ttf", 70);
	player2NameLabel->setAnchorPoint(Vec2(0.5f, 0.5f));
	player2NameLabel->setPosition(Vec2(Origin.x + 1300, Size.height + 600));
	player2NameLabel->setColor(Color3B::BLACK);
	BackgroundGame->addChild(player2NameLabel);

	playerTurnLabel = Label::createWithTTF("Player 1's Turn...", "fonts/Marker Felt.ttf", 80);
	playerTurnLabel->setPosition(Vec2(Size.width / 2 + Origin.x - 450, Size.height + 800));
	playerTurnLabel->setAnchorPoint(Vec2(0, 0.5f));
	playerTurnLabel->setColor(Color3B::YELLOW);
	BackgroundGame->addChild(playerTurnLabel);

	auto menuButton = Button::create("UI/PauseGame.png");
	menuButton->setPosition(Vec2(Size.width / 2 + Origin.x + 250, Size.height / 2 + Origin.y - 750));
	menuButton->addClickEventListener([this](Ref* sender) {
		SoundManager::getInstance()->playSFX("Sound/Click.mp3");
		showPauseMenuPopup();
		});
	BackgroundGame->addChild(menuButton);
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

	Oject();

}
void GameRiotScene::startCountdown()
{
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
void GameRiotScene::updatePlayerInfoUI()
{
	std::string name = currentPlayer == 1 ?
		GameManager::getInstance()->player1.name :
		GameManager::getInstance()->player2.name;

	playerTurnLabel->setString(name + "'s Turn...");
}

void GameRiotScene::showPauseMenuPopup()
{
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
		Director::getInstance()->replaceScene(TransitionFade::create(0.3f, GameRiotScene::createScene()));
		});
	dimLayer->addChild(newGameButton);

}

void GameRiotScene::initBoard()
{
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
				dropPiece(col);
				return true;
			}
			return false;
			};
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
	}

	////DEBUG
	//for (int row = 0; row < 6; row++) {
	//	for (int col = 0; col < 5; col++) {
	//		auto marker = Sprite::create("UI/tron.png"); 
	//		marker->setPosition(cellPositions[row][col]);
	//		BackgroundGame->addChild(marker, 10); 
	//	}
	//} // dùng ?? debug ?ánh d?u v? trí các ô trên bàn ch?i xem ? ?âu 

}
void GameRiotScene::dropPiece(int col)
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

			std::string fruit = (currentPlayer == 1) ? GameManager::getInstance()->player1.fruit : GameManager::getInstance()->player2.fruit;
			std::string spritePath = "UI/" + fruit + ".png";

			auto piece = Sprite::create(spritePath);
			piece->setScale(1.1f);

			// Tao hieu ung bom
			auto Bom = Sprite::create("UI/bom.png");
			Bom->setScale(0.04f);

			Vec2 targetPos1 = cellPositions[row][col];
			// Bat dau roi tu tren xuong 
			Vec2 startPos1 = Vec2(targetPos1.x, cellPositions[5][col].y + 150);
			Bom->setPosition(startPos1);
			pieceSprites[row][col] = Bom;

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

			auto fall1 = MoveTo::create(1.0f, targetPos1);
			Bom->runAction(fall1);


			// Kiem tra thang 
			if (Utils::checkWin(board, row, col)) {
				Popups::showPopupWin(this, currentPlayer);
			}

			// Kiem tra hoa
			if (Utils::isBoardFull(board)) {
				Popups::showPopupDraw(this, currentPlayer);
			}

			// Doi luot choi
			switchTurn();

			return;

		}
	}
}
void GameRiotScene::Oject()
{
	// Create the gun sprite and store it as a member variable
	Gun = Sprite::create("UI/Gun.png");
	Gun->setRotation(180); // Rotate horizontally
	BackgroundGame->addChild(Gun, 1000);
	Gun->setPosition(Vec2(235, 2300)); // Set initial position

	// Store possible positions as a member variable
	Pos = {
		Vec2(235, 2300),
		Vec2(515, 2300),
		Vec2(805, 2300),
		Vec2(1090, 2300),
		Vec2(1365, 2300)
	};

	// Schedule the RandomPos function every 10 seconds
	this->schedule(CC_SCHEDULE_SELECTOR(GameRiotScene::RandomPos), 10.0f);
	//BackgroundGame->addChild(Bom);


}

void GameRiotScene::RandomPos(float dt)
{
	if (Pos.empty() || !Gun || !isGameStarted) return;

	int col = RandomHelper::random_int(0, static_cast<int>(Pos.size()) - 1);

	// Di chuy?n Gun ??n c?t random
	auto move = MoveTo::create(0.5f, Pos[col]);
	auto easeMove = EaseSineInOut::create(move);

	// Sau khi Gun di chuy?n xong thì th? bom
	auto dropBom = CallFunc::create([=]() {
		for (int row = 0; row < 6; row++)
		{
			if (board[row][col] == 0)
			{
				board[row][col] = 9;

				auto Bom = Sprite::create("UI/bom.png");
				Bom->setScale(0.04f);
				Vec2 startPos = Vec2(cellPositions[5][col].x, cellPositions[5][col].y + 150);
				Vec2 targetPos = cellPositions[row][col];
				Bom->setPosition(startPos);

				BackgroundGame->addChild(Bom, 2000);
				pieceSprites[row][col] = Bom;

				auto fall = MoveTo::create(1.0f, targetPos);

				// Sau khi bom r?i xong ? Xóa các piece li?n k?
				auto removeNearby = CallFunc::create([=]() {
					// Phát âm thanh n?

					SoundManager::getInstance()->playSFX("Sound/no.mp3");

					// T?o sprite explosion ? v? trí bom
					auto explosion = Sprite::create("UI/no-1.png");
					explosion->setScale(2.0f); // to h?n ?? d? nhìn
					explosion->setPosition(Bom->getPosition());
					BackgroundGame->addChild(explosion, 3000);

					// T?o animation t? 7 frame
					Vector<SpriteFrame*> frames;
					for (int i = 1; i <= 7; i++)
					{
						std::string frameName = "UI/no-" + std::to_string(i) + ".png";
						auto spriteTmp = Sprite::create(frameName);
						if (spriteTmp)
						{
							auto frame = spriteTmp->getSpriteFrame();
							if (frame) frames.pushBack(frame);
						}
					}

					auto animation = Animation::createWithSpriteFrames(frames, 0.1f);
					auto animate = Animate::create(animation);
					auto removeExplosion = CallFunc::create([explosion]() {
						explosion->removeFromParent();
						});
					explosion->runAction(Sequence::create(animate, removeExplosion, nullptr));

					// Xóa các piece xung quanh 4 h??ng
					const int dx[] = { -1, 1, 0, 0 };
					const int dy[] = { 0, 0, -1, 1 };

					for (int i = 0; i < 4; ++i)
					{
						int newRow = row + dx[i];
						int newCol = col + dy[i];

						if (newRow >= 0 && newRow < 6 && newCol >= 0 && newCol < 5)
						{
							if (board[newRow][newCol] == 1 || board[newRow][newCol] == 2)
							{
								if (pieceSprites[newRow][newCol])
								{
									pieceSprites[newRow][newCol]->removeFromParent();
									pieceSprites[newRow][newCol] = nullptr;
								}
								board[newRow][newCol] = 0;
							}
						}
					}

					// Xóa bom
					if (pieceSprites[row][col])
					{
						pieceSprites[row][col]->removeFromParent();
						pieceSprites[row][col] = nullptr;
					}
					board[row][col] = 0;
					});

				auto seq = Sequence::create(fall, DelayTime::create(0.2f), removeNearby, nullptr);
				Bom->runAction(seq);

				return;
			}
		}
		});

	auto sequence = Sequence::create(easeMove, dropBom, nullptr);
	Gun->runAction(sequence);
}



void GameRiotScene::switchTurn()
{
	currentPlayer = (currentPlayer == 1) ? 2 : 1;
	updatePlayerInfoUI();
}