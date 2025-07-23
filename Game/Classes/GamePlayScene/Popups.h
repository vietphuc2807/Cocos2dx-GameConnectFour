#ifndef __POPUPS_H__
#define __POPUPS_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

class Popups : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    static void showPopupWin(Scene* parent, int currentPlayer);
    static void showPopupDraw(Scene* parent, int currentPlayer);
    static void showPopupLoseAI(Scene* parent, int currentPlayer);
    static void showPopupWinAI(Scene* parent, int currentPlayer);

    CREATE_FUNC(Popups);

private:

    

    

    
    int board[6][5];
};

#endif // __POPUPS_H__
