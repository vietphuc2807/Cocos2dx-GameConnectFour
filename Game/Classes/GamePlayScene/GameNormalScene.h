#ifndef __GAME_NORMAL_SCENE_H__
#define __GAME_NORMAL_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class GameNormalScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(GameNormalScene);

private:
    void initUI();

	void initBoard();

    void updatePlayerInfoUI();

    void switchTurn();

    void dropPiece(int col);

    void startCountdown();

	void showPauseMenuPopup();

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
};

#endif // __GAME_NORMAL_SCENE_H__
