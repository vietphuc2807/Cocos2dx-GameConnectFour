#pragma once
#include "cocos2d.h"

class SaveDataManager {
public:
    static void saveGame(int board[6][5], int currentPlayer);
    static bool loadGame(int board[6][5], int& currentPlayer);
    static void clearGame(); // Xóa file khi b?t ??u game m?i
};
