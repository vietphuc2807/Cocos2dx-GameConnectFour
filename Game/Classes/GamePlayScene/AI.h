#pragma once
#ifndef _AI_H_
#define _AI_H_

#include "cocos2d.h"

class AI {
public:
    AI(int board[6][5]);
    int calculateMove();

private:
    int board[6][5];

    bool willWin(int player, int row, int col);

};
#endif // _AI_H_