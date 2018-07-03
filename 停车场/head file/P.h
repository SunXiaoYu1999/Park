#pragma once
#include <time.h>

typedef struct
{
	int num;
	int x;
	int y;
	time_t T_In; // 进入车库时间
}CAR;
void Park_Init();
int Park_Mouse();
void Park();
void Park_InCar();
void Park_OutCar();
void Park_DrawCar(int x, int y);
void Park_ClearCar(int x, int y);
void Park_MoveCar(int x_1, int y_1, int x_2, int y_2);
void Park_InitQuSt();
int Park_GetInput();
void Park_OutConsume(int x, int y, int i);
void Park_CleanConsume(int x, int y);