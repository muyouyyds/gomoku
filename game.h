#pragma once
#ifndef GAME_H
#define GAME_H
#define BOARD_SIZE 15 // 0: empty, 1: black, 2: white
void initBoard(void);
int PlacePiece(int i, int j);
int GetNextPlayer();
int TryPlace(int i, int j);
int ToGridX(int mouseX);
int ToGridY(int mouseY);
int CheckWin(int i, int j);
extern int currentPlayer; // 1黑 2白
extern int board[BOARD_SIZE][BOARD_SIZE]; //创建棋盘数组
extern int LOCX[16];
extern int LOCY[16];
extern int gamestatus; // -1:stop, 0: ongoing, 1: black wins, 2: white wins
#endif

