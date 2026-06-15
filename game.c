#include "game.h"
#include <windows.h>

int board[BOARD_SIZE][BOARD_SIZE] = {0}; //创建棋盘数组
int gamestatus = -1; // -1:stop, 0: ongoing, 1: black wins, 2: white wins
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


int PlacePiece(int i, int j)
{
    // 越界检查
    if (i < 0 || i >= BOARD_SIZE ||
        j < 0 || j >= BOARD_SIZE)
        return 0;

    // 已有棋子
    if (board[i][j] != 0)
        return 0;

    if (gamestatus != 0)
        return 0;
    return 1;
}


int currentPlayer = 1; // 1黑 2白

int GetNextPlayer()
{
    return (currentPlayer == 1) ? 2 : 1;
}


int TryPlace(int i, int j)
{
    if (PlacePiece(i, j))
    {
        board[i][j] = currentPlayer; // 更新棋盘状态
        
        if (CheckWin(i, j))
    {    
       gamestatus = currentPlayer;
       MessageBox(NULL, (currentPlayer == 1) ? TEXT("黑棋获胜！") : TEXT("白棋获胜！"), TEXT("游戏结束"), MB_OK);
    }
    currentPlayer = GetNextPlayer(); // 切换玩家
        return 1;
    }
    
    return 0;
}


int ToGridX(int mouseX)
{
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        if (mouseX >= LOCX[i]&& mouseX <= LOCX[i+1])
        {
            return i;
        }
    }
       return -1; // 返回 -1 表示未找到对应的网格位置
}

int ToGridY(int mouseY)
{
    for (int j = 0; j < BOARD_SIZE; j++)
    {
        if (mouseY >= LOCY[j] && mouseY <= LOCY[j+1])
        {
            return j;
        }
    }
    return -1; // 返回 -1 表示未找到对应的网格位置  
}


int CheckWin(int i, int j)
{
    int color = board[i][j];
    if (color == 0)
        return 0;

    // 检查四个方向
    int directions[4][2] = {{1, 0}, {0, 1}, {1, 1}, {1, -1}};
    for (int d = 0; d < 4; d++)
    {
        int count = 1;
        for (int step = 1; step < 5; step++)
        {
            int x = i + step * directions[d][0];
            int y = j + step * directions[d][1];
            if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && board[x][y] == color)
                count++;
            else
                break;
        }
        for (int step = 1; step < 5; step++)
        {
            int x = i - step * directions[d][0];
            int y = j - step * directions[d][1];
            if (x >= 0 && x < BOARD_SIZE && y >= 0 && y < BOARD_SIZE && board[x][y] == color)
                count++;
            else
                break;
        }
        if (count >= 5)
            return color; // 返回获胜者的颜色
    }
    return 0; // 没有赢家
}