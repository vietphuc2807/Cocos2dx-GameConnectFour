#ifndef __UTILS_H__
#define __UTILS_H__

#include "cocos2d.h"

class Utils
{
public:
    static bool checkWin(int board[6][5], int row, int col);

    static bool isBoardFull(int board[6][5]);
};

#endif // __UTILS_H__
