#ifndef __GAME_RIOT_SCENE_H__
#define __GAME_RIOT_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GameRiotScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(GameRiotScene);

private:
    void initUI();

    void initBoard();

    void updatePlayerInfoUI();

    void switchTurn();

    void dropPiece(int col);

    void showPauseMenuPopup();

    void startCountdown();

    void showOptionsPopup();

    void Oject();

    void RandomPos(float dt);

    cocos2d::ui::Button* soundButton;
    cocos2d::Sprite* BackgroundGame;
    cocos2d::Sprite* pieceSprites[6][5];
    cocos2d::Vec2 cellPositions[6][5];

    int board[6][5];
    int currentPlayer;
    bool isSoundOn = true;
    bool isGameStarted = false;

    cocos2d::Label* playerTurnLabel;
    cocos2d::Label* player1NameLabel;
    cocos2d::Label* player2NameLabel;

    cocos2d::Sprite* Gun = nullptr;
    std::vector<cocos2d::Vec2> Pos;
};

#endif // __GAME_RIOT_SCENE_H__