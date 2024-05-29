#pragma once
#include <Windows.h>
#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"MSIMG32.LIB")
#include <graphics.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>
using namespace std;
extern int real_frame_counter;
const int bird_anim_num = 5;
inline void putimage_alpha(int x, int y, IMAGE* img);
class board {
public:
	IMAGE up_stage;
	IMAGE down_stage;
	IMAGE numbers;
	board() {
		loadimage(&up_stage, _T("img/board/0.png"),242,240);
		loadimage(&down_stage, _T("img/board/1.png"),242,240);
	}
	void stage(int board_down_time) {
		if(board_down_time>0)putimage_alpha(490, 520, &down_stage);
		else putimage_alpha(490, 520, &up_stage);
	}
	void show_point(int point) {
		settextstyle(63, 22, _T("����Ҧ��"));
		settextcolor(YELLOW);
		setbkmode(TRANSPARENT);
		if(point<20)outtextxy(675, 620, to_wstring(point/2).c_str());
		else outtextxy(664, 620, to_wstring(point / 2).c_str());
	}
};
class bird {
public:
	POINT pos;
	IMAGE bird_anim[bird_anim_num];
	IMAGE bird_over1, bird_over2, bird_over3[bird_anim_num];
	int uptimes;
	int downspeed;
	bird() :pos({ 280,250 }), uptimes(0), downspeed(3) {
		load_bird();
	}
	void load_bird() {
		for (size_t i = 0; i < bird_anim_num; i++) {
			std::wstring path1,path2;
			path1 = L"img/bird/" + std::to_wstring(i) + L".png";
			path2=L"img/bird/over3_" + std::to_wstring(i) + L".png";
			loadimage(&bird_anim[i], path1.c_str(), 110, 110);
			loadimage(&bird_over3[i], path2.c_str(), 110, 110);
		}
		loadimage(&bird_over1, _T("img/bird/over1.png"), 110, 110);
		loadimage(&bird_over2, _T("img/bird/over2.png"), 110, 110);
	}
	void go_up() {
		if (uptimes >= 15)
			pos.y -= 17;
		else if (uptimes >= 9)pos.y -= 12;
		else pos.y -= 4;
		uptimes--;
	}
	void go_down(bool gravity) {
		if (gravity) {
			if (real_frame_counter % 4 == 0)downspeed++;
			pos.y += downspeed;
			if (pos.y >= 780)pos.y = -60;
		}
	}
	void over() {
		if (real_frame_counter % 2 == 0)downspeed++;
		pos.y += downspeed;
	}
};
class barrier {
public:
	POINT pos;
	IMAGE barrier_res;
	bool flag;
	std::pair<int, int>X;
    std::pair<int, int>Y;
	int type;
	barrier(int n,int seed):flag(false){
		srand(seed);
		pos.x = 1440;
		type = n;
		switch (type) {
		case 1:
			loadimage(&barrier_res, _T("img/barrier/test.jpg"), 100, 400);
			pos.y = rand() % 300-290;
			break;
		case 2:
			loadimage(&barrier_res, _T("img/barrier/test.jpg"), 100, 400);
			pos.y = rand() % 250 + 480;
			break;
		}
	}
	void go_left(int speedup) {
		pos.x -= 4 + speedup;
		X.first = pos.x - 55, X.second = pos.x + 40;
		Y.first = pos.y - 75, Y.second = pos.y + 360;
	}
};