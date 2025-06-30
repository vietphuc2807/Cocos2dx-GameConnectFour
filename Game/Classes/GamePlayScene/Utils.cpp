#include "Utils.h"

bool Utils::checkWin(int board[6][5], int row, int col)
{
    int player = board[row][col];
    const int dx[4] = { 1, 0, 1, 1 };   
    const int dy[4] = { 0, 1, 1, -1 };

    for (int d = 0; d < 4; ++d)
    {
        int count = 1;
        for (int dir = -1; dir <= 1; dir += 2)
        {
            int x = col + dx[d] * dir;
            int y = row + dy[d] * dir;

            while (x >= 0 && x < 5 && y >= 0 && y < 6 && board[y][x] == player)
            {
                count++;
                x += dx[d] * dir;
                y += dy[d] * dir;
            }
        }

        if (count >= 4)
            return true;
    }

    return false;
}

bool Utils::isBoardFull(int board[6][5])
{
    for (int row = 0; row < 6; ++row)
    {
        for (int col = 0; col < 5; ++col)
        {
            if (board[row][col] == 0)
                return false;
        }
    }
    return true;
}