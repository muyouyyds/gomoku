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
        board[i][j] = currentPlayer; // 更新棋盘状态
        currentPlayer = GetNextPlayer(); // 切换玩家
        return 1;
    }
    return 0;
}


int ToGridX(int mouseX)
{
    for (int i = 0; i < BOARD_SIZE-1; i++)
    {
        if (mouseX >= LOCX[i]&& mouseX <= LOCX[i+1])
        {
            return i;
        }
    }
   
    if (mouseX >= LOCX[BOARD_SIZE-1]) // 处理最后一个网格的情况
    {
        return 13; // 返回最后一个网格的索引
    }
    return -1; // 返回 -1 表示未找到对应的网格位置
}

int ToGridY(int mouseY)
{
    for (int j = 0; j < BOARD_SIZE-1; j++)
    {
        if (mouseY >= LOCY[j] && mouseY <= LOCY[j+1])
        {
            return j;
        }
        if (mouseY >= LOCY[BOARD_SIZE-1]) // 处理最后一个网格的情况
        {
            return 13; // 返回最后一个网格的索引
        }
    }
    return -1; // 返回 -1 表示未找到对应的网格位置  
}