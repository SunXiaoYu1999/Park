#pragma once
#include "stdafx.h"
#include <stdio.h>
#include <graphics.h>
#include <conio.h>
#include <iostream>
#include "P.h"

using namespace std;

// 定义停车场单价 每秒1元
const int PRICE = 1;
// 定义全局栈1 , 当主停车场
CAR St_1[7]; int top_1 = -1;
// 定义全局栈2 ，当暂停区
CAR St_2[7]; int top_2 = -1;
// 定义全局队列 ， 便道
CAR Qu[8]; int rear = -1; int front = -1;

void Park_Init()
{
	initgraph(1600, 900);
	setbkcolor(RGB(233,215,215));  //  背景颜色
	cleardevice();
	IMAGE a;
	loadimage(&a, L"User.jpg", 500, 900, NULL);
	putimage(1100, 0, &a);
	// button 1
	setfillcolor(DARKGRAY);
	fillroundrect(1200,50,1500,150,100,100);

	// button 1 TEXT
	settextcolor(RGB(255,255,255));
	settextstyle(50, 0, L"微软雅黑");
	setbkmode(TRANSPARENT);                   // 字体背景透明
	outtextxy(1275, 75, L"进一辆车");

	// button 2
	setfillcolor(DARKGRAY);
	fillroundrect(1200,200,1500,300,100,100);

	// button 2 TEXT
	settextcolor(RGB(255, 255, 255));
	settextstyle(50, 0, L"微软雅黑");
	setbkmode(TRANSPARENT);                   // 字体背景透明
	outtextxy(1275, 225, L"出一辆车");

	// button 3
	setfillcolor(DARKGRAY);
	fillroundrect(1450, 800, 1550, 850, 50, 50);

	// button 3 TEXT
	settextcolor(RGB(255, 255, 255));
	settextstyle(30, 0, L"微软雅黑");
	setbkmode(TRANSPARENT);                   // 字体背景透明
	outtextxy(1480, 810, L"退出");

	// 绘制停车场
	int n = 16;  // 墙宽
	setlinecolor(RGB(41, 138, 18));
	setfillcolor(RGB(95, 66, 66));  // 围墙颜色
	POINT pts1[] = { { 0,0 },{ 0, 900 },{ 1000, 900 },{1000,900 - n},{n,900 - n},{n,n},{1100,n},{1100,0} };
	fillpolygon(pts1, 8);
	POINT pts2[] = { { 100 + n,100 + n },{ 100 + n,800 - n },{ 1000, 800 - n },{ 1000,100 + n },{ 300 + 3 * n,100 + n },{300 + 3 * n,100 + 2 * n },{ 1000 - n,100 + 2 * n },{ 1000 - n,800 - 2 * n },{100 + 2 * n,800 - 2 * n},{100 + 2 * n,100 + n} };
	fillpolygon(pts2, 10);
	fillrectangle(200 + 2 * n, 100 + n, 200 + 3 * n, 800 - 2 * n);
	fillpolygon(pts2, 10);
	fillrectangle(300 + 3 * n, 100 + n, 300 + 4 * n, 800 - 2 * n);

	//  绘制车辆序号
	int m = 100; // 序号间隔
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	settextstyle(40, 0, L"微软雅黑");
	int i = 1;
	int x_1 = 350 + n, y_1 = 715 - 2 * n;
	int x_2 = 395 + n, y_2 = 100 + 2 * n;
	TCHAR s[5];
	do {				
		swprintf_s(s, _T("%d"), i);
		outtextxy(x_1, y_1, s);
		i++;
		y_1 -= m;
	} while (i < 7);
//	do {
//		swprintf_s(s, _T("%d"), i);
//		outtextxy(x_2, y_2, s);
//		i++;
//		x_2 += m;
//	} while (i < 13);

	// 输出单价
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	settextstyle(50, 0, L"微软雅黑");
	swprintf_s(s, _T("%d"), PRICE);
	outtextxy(1335, 450, s);
	outtextxy(1250, 450, L"单价    元/秒");
	outtextxy(1235, 575, L"本次消费    元");

}
//  鼠标函数，返回1代表进一辆车，返回2代表出一辆车，返回0代表退出
int Park_Mouse()
{
	int x, y;
	MOUSEMSG mou;          // 一个鼠标消息变量
	while (true) {
		mou = GetMouseMsg();
		if (mou.uMsg == WM_LBUTTONDOWN) {
			x = mou.x;
			y = mou.y;
			if ((x > 1200 && x < 1500) && (y > 50 && y < 150))
				return 1;
			else if ((x > 1200 && x < 1500) && (y > 200 && y < 300 ))
				return 2;
			else if ((x > 1450 && x < 1550) && (y > 800 && y < 850 ))
				return 0;
		}
	}
}

void Park()
{
	int mou;
	Park_Init();
	Park_InitQuSt();
	do {
		mou = Park_Mouse();
		switch (mou) {
		case 1:
			Park_InCar();
			break;
		case 2:
			Park_OutCar();
			break;
		}
	} while (mou != 0);
	closegraph();
}
// 进车模块
void Park_InCar()
{
	int num;
	if (rear >= front)
		num = top_1 + rear - front;
	else
		num = top_1 + rear + 8 - front;
	if (num < 12)
	{
		Park_MoveCar(1050, 850 - 16, 1050, 50 + 16);
		if (rear == front && top_1 < 6) {
			++top_1;
			St_1[top_1].x = St_1[top_1 - 1].x;
			St_1[top_1].y = St_1[top_1 - 1].y - 100;
			St_1[top_1].num = St_1[top_1 - 1].num + 1;
			St_1[top_1].T_In = time(NULL);
			Park_MoveCar(1050, 50 + 16, 250 + 48, 50 + 16);
			Park_MoveCar(250 + 48, 50 + 16, St_1[top_1].x, St_1[top_1].y);
		}
		else{
			int x = Qu[rear].x;
			int y = Qu[rear].y;
			int nu = Qu[rear].num;
			++rear;
			rear = rear % 8;
			Qu[rear].x = x + 100;
			Qu[rear].y = y;
			Qu[rear].num = nu + 1;
			Park_MoveCar(1050, 50 + 16, Qu[rear].x, Qu[rear].y);
		}
	}
}
// 出车模块
void Park_OutCar()
{
	int num;
	if (rear >= front)
		num = top_1 + rear - front;
	else
		num = top_1 + rear + 8 - front;
	int reply = Park_GetInput();
	while(reply <= 0 || reply > num)
		reply = Park_GetInput();
	if (reply <= 6) {
		while (St_1[top_1].num != reply) {
			++top_2;
			St_2[top_2].x = St_2[top_2 - 1].x;
			St_2[top_2].y = St_2[top_2 - 1].y - 100;
			St_2[top_2].T_In = St_1[top_1].T_In;
			Park_MoveCar(St_1[top_1].x, St_1[top_1].y, 250 + 48, 50 + 16);
			Park_MoveCar(250 + 48, 50 + 16, 150 + 32, 50 + 16);
			Park_MoveCar(150 + 32, 50 + 16, St_2[top_2].x, St_2[top_2].y);
			--top_1;
		}
		time_t T2 = time(NULL);
		int price  = ((int)T2 - (int)St_1[top_1].T_In) * PRICE;
		Park_OutConsume(1390, 585, price);
		Park_MoveCar(St_1[top_1].x, St_1[top_1].y, 250 + 48, 50 + 16);
		Park_MoveCar(250 + 48, 50 + 16, 50 + 16, 50 + 16);
		Park_MoveCar(50 + 16, 50 + 16, 50 + 16, 900 - 16 - 50);
		Park_MoveCar(50 + 16, 900 - 16 - 50, 1050, 900 - 16 - 50);
		Park_MoveCar(1050, 900 - 16 - 50, 1050, 900 + 50);
		top_1--;
		while (top_2 > 0) {
			++top_1;
			St_1[top_1].x = St_1[top_1 - 1].x;
			St_1[top_1].y = St_1[top_1 - 1].y - 100;
			St_1[top_1].T_In = St_2[top_2].T_In;
			Park_MoveCar(St_2[top_2].x, St_2[top_2].y, 150 + 32, 50 + 16);
			Park_MoveCar(150 + 32, 50 + 16, 250 + 48, 50 + 16);
			Park_MoveCar(250 + 48, 50 + 16, St_1[top_1].x, St_1[top_1].y);
			--top_2;
		}
		if (num > 6) {
			int x = Qu[front].x;
			int y = Qu[front].y;
			int nu = Qu[front].num;
			front++;
			front = front % 8;
			Park_MoveCar(Qu[front].x, Qu[front].y, 250 + 48, 50 + 16);
			++top_1;
			St_1[top_1].x = St_1[top_1 - 1].x;
			St_1[top_1].y = St_1[top_1 - 1].y - 100;
			St_1[top_1].T_In = time(NULL);
			Park_MoveCar(250 + 48, 50 + 16, St_1[top_1].x, St_1[top_1].y);
			Qu[front].x = x;
			Qu[front].y = y;
			Qu[front].num = nu;
			for (int i = front; i != rear;) {
				x = Qu[i].x;
				y = Qu[i].y;
				nu = Qu[i].num;
				i++;
				i = i % 8;
				Qu[i].x = x + 100;
				Qu[i].y = y;
				Qu[i].num = nu;
				Park_MoveCar(Qu[i].x + 100, Qu[i].y, Qu[i].x, Qu[i].y);
			}
		}

	}
	else {

	}
	Park_CleanConsume(1390, 580);
}

void Park_DrawCar(int x, int y)
{
	setfillcolor(BLACK);
	solidrectangle(x - 15, y - 15, x + 15, y + 15);

	setlinecolor(BLACK);
	line(x - 30, y - 30, x - 15, y - 15);
	line(x + 30, y - 30, x + 15, y - 15);
	line(x - 30, y + 30, x - 15, y + 15);
	line(x + 30, y + 30, x + 15, y + 15);

	setfillcolor(BLACK);
	solidcircle(x - 30, y - 30, 10);
	solidcircle(x + 30, y - 30, 10);
	solidcircle(x - 30, y + 30, 10);
	solidcircle(x + 30, y + 30, 10);
}

void Park_ClearCar(int x, int y)
{
	setfillcolor(RGB(233, 215, 215));
	solidrectangle(x - 15, y - 15, x + 15, y + 15);

	setlinecolor(RGB(233, 215, 215));
	line(x - 30, y - 30, x - 15, y - 15);
	line(x + 30, y - 30, x + 15, y - 15);
	line(x - 30, y + 30, x - 15, y + 15);
	line(x + 30, y + 30, x + 15, y + 15);


	setfillcolor(RGB(233, 215, 215));
	solidcircle(x - 30, y - 30, 10);
	solidcircle(x + 30, y - 30, 10);
	solidcircle(x - 30, y + 30, 10);
	solidcircle(x + 30, y + 30, 10);
}

void Park_MoveCar(int x_1, int y_1, int x_2, int y_2)
{
	int T = 1;
	if (x_1 == x_2) {
		int i;
		for (i = y_1; i != y_2;) {		
			Park_DrawCar(x_1, i);
//			Sleep(T);
			Park_ClearCar(x_1, i);
			if (i < y_2)
				i++;
			else
				i--;
		}
		Park_DrawCar(x_1, i);
	}
	else {
		int i;
		for (i = x_1; i != x_2;) {
			Park_DrawCar(i, y_1);
//			Sleep(T);
			Park_ClearCar(i, y_1);
			if (i < x_2)
				i++;
			else
				i--;
		}
		Park_DrawCar(i, y_1);
	}
}

void Park_InitQuSt()
{
	St_1[++top_1].x = 250 + 48;
	St_1[top_1].y = 900 - 50 - 32;
	St_1[top_1].num = 0;

	St_2[++top_2].x = 150 + 32;
	St_2[top_2].y = 900 - 50 - 32;

	Qu[++rear].x = 250 + 64;
	Qu[++front].y = 50 + 16;
	Qu[rear].num = 6;
}

int Park_GetInput()
{
	WCHAR s[3];
	InputBox(s, 3, L"请输入车辆序号",L"InPut");
	char s1[3];
	for (int i = 0; i < 3; i++) {
		s1[i] = s[i];
	}
	int r;
	sscanf_s(s1, "%d", &r);
	return r;
}

void Park_OutConsume(int x, int y, int consume)
{
	TCHAR s[5];
	settextcolor(BLACK);
	setbkmode(TRANSPARENT);
	settextstyle(25, 0, L"微软雅黑");
	swprintf_s(s, _T("%d"), consume);
	outtextxy(x, y, s);
}

void Park_CleanConsume(int x, int y)
{
	setfillcolor(RGB(247,245,249));
	solidrectangle(x, y, x + 40, y + 40);
}

