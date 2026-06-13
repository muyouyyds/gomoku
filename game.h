#pragma once
#ifndef GAME_H
#define GAME_H
#define BOARD_SIZE 14 // 0: empty, 1: black, 2: white
void initBoard(void);
int PlacePiece(int i, int j, int color);
int GetNextPlayer();
int TryPlace(int i, int j);
int ToGridX(int mouseX);
int ToGridY(int mouseY);
extern int currentPlayer; // 1黑 2白
extern int board[BOARD_SIZE][BOARD_SIZE]; //创建棋盘数组
extern int LOCX[14];
extern int LOCY[14];
#endif

