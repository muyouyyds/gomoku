#include "game.h"

int board[BOARD_SIZE][BOARD_SIZE] = {0}; //创建棋盘数组

void initBoard(void)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            board[i][j] = 0;
        }
    }
}


int PlacePiece(int i, int j, int color)
{
    // 越界检查
    if (i < 0 || i >= BOARD_SIZE ||
        j < 0 || j >= BOARD_SIZE)
        return 0;

    // 已有棋子
    if (board[i][j] != 0)
        return 0;

    board[i][j] = color;
    return 1;
}


int currentPlayer = 1; // 1黑 2白

int GetNextPlayer()
{
    return (currentPlayer == 1) ? 2 : 1;
}


int TryPlace(int i, int j)
{
    if (PlacePiece(i, j, currentPlayer))
    {
        currentPlayer = GetNextPlayer();
        return 1;
    }
    return 0;
}


int ToGridX(int mouseX)
{
    return (mouseX - 292 + 18) / 36;
}

int ToGridY(int mouseY)
{
    return (mouseY - 87 + 18) / 36;
}