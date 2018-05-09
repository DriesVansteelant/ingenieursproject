#ifndef HULPFUNCTIES_H
#define HULPFUNCTIES_H
#include <stdlib.h>
#include "HeaderFiles/dwenguinoBoard.h"
#include "HeaderFiles/dwenguinoBoard.h"
#include "HeaderFiles/dwenguinoLCD.h"

#define LEFT  1
#define UP 2
#define RIGHT  3
#define DOWN 0
#define NONE -1

#define B0 0b00000

int GiveIndex(BYTE bal[8],int dir);
int MoveRight(BYTE balExtra[8],BYTE bal[8]);
int MoveLeft(BYTE balExtra[8],BYTE bal[8]);
void MoveDown(BYTE balExtra[8],BYTE bal[8]);
void MoveUp(BYTE balExtra[8],BYTE bal[8]);
void PrintBal(BYTE bal[8],int CursorH,int CursorB, int index);
void Swap(BYTE A[8],BYTE B[8]);

#endif
