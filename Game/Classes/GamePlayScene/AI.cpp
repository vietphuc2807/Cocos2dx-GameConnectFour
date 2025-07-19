#include "AI.h"
#include "GameplayScene/Utils.h"

AI::AI(int inputBoard[6][5]) {
    for (int row = 0; row < 6; ++row)
        for (int col = 0; col < 5; ++col)
            board[row][col] = inputBoard[row][col];
}

int AI::calculateMove() {
    std::vector<int> validCols;

    for (int col = 0; col < 5; col++) {
        if (board[5][col] == 0)
            validCols.push_back(col);
    }

    // Uu tien nuoc thang
    for (int col : validCols) {
        for (int row = 0; row < 6; ++row) {
            if (board[row][col] == 0) {
                board[row][col] = 2;
                if (Utils::checkWin(board, row, col)) {
                    board[row][col] = 0;
                    return col;
                }
                board[row][col] = 0;
                break;
            }
        }
    }

    // Chan nguoi choi
    for (int col : validCols) {
        for (int row = 0; row < 6; ++row) {
            if (board[row][col] == 0) {
                board[row][col] = 1;
                if (Utils::checkWin(board, row, col)) {
                    board[row][col] = 0;
                    return col;
                }
                board[row][col] = 0;
                break;
            }
        }
    }

    // Chon cot ngau nhien
    if (!validCols.empty()) {
        int randIndex = rand() % validCols.size();
        return validCols[randIndex];
    }

    return -1;
}