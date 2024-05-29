#pragma once
#pragma comment(lib,"MSIMG32.LIB")
#include <graphics.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>
extern int real_frame_counter;
const int bird_anim_num = 5;
IMAGE bird_anim[bird_anim_num];
IMAGE bird_over1, bird_over2, bird_over3[bird_anim_num];
class bird {
public:
	int uptimes;
	int downspeed;
	POINT pos;
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
			pos.y -= 18;
		else if (uptimes >= 9)pos.y -= 12;
		else pos.y -= 5;
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
	std::pair<int, int>X;
    std::pair<int, int>Y;
	int type;
	barrier(int n,int seed){
		srand(seed);
		pos.x = 1440;
		type = n;
		switch (type) {
		case 1:
			loadimage(&barrier_res, _T("img/barrier/test.jpg"), 100, 400);
			pos.y = rand() % 300-300;
			break;
		case 2:
			loadimage(&barrier_res, _T("img/barrier/test.jpg"), 100, 400);
			pos.y = rand() % 300 + 440;
			break;
		}
	}
	void go_left(int speedup) {
		pos.x -= 4 + speedup;
		switch (type) {
		case 1:
			X.first = pos.x - 55, X.second = pos.x + 40;
			Y.first = pos.y-80, Y.second = pos.y + 360;
			break;
		case 2:
			X.first = pos.x-55 , X.second = pos.x + 40;
			Y.first = pos.y-80, Y.second = pos.y + 360;
			break;
		}
	}
};