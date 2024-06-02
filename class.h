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
#include <cstdio>
#include <cstring>
using namespace std;
extern int real_frame_counter;
const int bird_anim_num = 5;
const int button_anim_num = 3;
bool bgm = true;
inline void putimage_alpha(int x, int y, IMAGE* img);
class text {
public:
	POINT pos[3];
	IMAGE context[3];
	text() {
		pos[0] = { 310,214 };
		pos[1] = { 530,220 };
		pos[2] = { 425,165 };
		loadimage(&context[0], _T("img/text/best.png"), 300, 300);
		loadimage(&context[1], _T("img/text/record.png"), 300, 300);
		loadimage(&context[2], _T("img/text/new_record.png"), 400, 400);
	}
};
class button {
public:
	POINT pos;
	IMAGE button_anim[button_anim_num];
	pair<int,int> X;
	pair<int, int>Y;
	bool animing;
	button(int x, int y,int w,int h, int type) :animing(false){
		X.first=pos.x = x;
		Y.first=pos.y = y;
		for (size_t i = 0; i < button_anim_num; i++) {
			wstring path;
			path = L"img/button/" + std::to_wstring(type) +L"/" + std::to_wstring(i) + L".png";
			loadimage(&button_anim[i], path.c_str(), w, h);
		}
	}
	bool check(int x, int y) {
		if (X.first <= x && x <= X.second && Y.first <= y && y <= Y.second) {
			return animing = true;
		}
		else return false;
	}
};
class play_button :public button {
private:
	bool* gaming_location;
public:
	play_button(int x, int y, int w, int h, int type,bool*gaming) :button(x, y, w, h, type),gaming_location(gaming) {
		X.second = X.first + w;
		Y.second = Y.first + h;
	}
	void anime(int idx) {
		if (animing) {
			putimage_alpha(pos.x, pos.y, &button_anim[idx]);
			if (idx == 2) {
				animing = false;
				*gaming_location = true;
			}
		}
		else putimage_alpha(pos.x, pos.y, &button_anim[0]);
	}
};
class quit_button :public button {
private:
	bool* running_location;
public:
	quit_button(int x, int y, int w, int h, int type,bool*running) :button(x, y, w, h, type),running_location(running) {
		X.second = X.first + w;
		Y.second = Y.first + h;
	}
	void anime(int idx) {
		if (animing) { 
			putimage_alpha(pos.x, pos.y, &button_anim[idx]); 
		   if (idx == 2) {
			animing = false;
			*running_location = false;
		   }
		}
		else
			putimage_alpha(pos.x, pos.y, &button_anim[0]);
	}
};

class bgm_button :public button {
public:
	int record;
	int change_num;
	bgm_button(int x, int y, int w, int h, int type) :button(x, y, w, h, type),record(0),change_num(0) {
		X.second = X.first + w;
		Y.second = Y.first + h;
	}
	void anime(int idx) {
		if (animing) { 
			if (change_num % 2==0)
			putimage_alpha(pos.x, pos.y, &button_anim[idx]); 
			else putimage_alpha(pos.x, pos.y, &button_anim[2-idx]);
		   if (idx == 2) {
			animing = false;
			
			if (++change_num % 2) { 
				mciSendString(_T("pause bgm"), NULL, 0, NULL); 
				record = 2;
			}
			else { 
				mciSendString(_T("resume bgm"), NULL, 0, NULL);
				record = 0; }
		   }
		}
		else 
			putimage_alpha(pos.x, pos.y, &button_anim[record]);
	}
};

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
		settextstyle(63, 22, _T("·½ÕýÒ¦Ìå"));
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
	bird(int x=280,int y=-150,int w=110,int h=110) :pos({ x,y }), uptimes(0), downspeed(4) {
		load_bird(w,h);
	}
	void load_bird(int w,int h) {
		for (size_t i = 0; i < bird_anim_num; i++) {
			wstring path1,path2;
			path1 = L"img/bird/" + std::to_wstring(i) + L".png";
			path2=L"img/bird/over3_" + std::to_wstring(i) + L".png";
			loadimage(&bird_anim[i], path1.c_str(), w, h);
			loadimage(&bird_over3[i], path2.c_str(), w, h);
		}
		loadimage(&bird_over1, _T("img/bird/over1.png"), w, h);
		loadimage(&bird_over2, _T("img/bird/over2.png"), w, h);
	}
	void go_up() {
		if (uptimes >= 19)
			pos.y -= 19;
		else if (uptimes >= 15)pos.y -= 15;
		else if (uptimes >= 10)pos.y -= 12;
		else if (uptimes >= 5)pos.y -= 5;
		else pos.y -= 2;
		uptimes--;
	}
	void go_down(bool gravity) {
		if (gravity) {
			if (real_frame_counter % 4 == 0)downspeed++;
			pos.y += downspeed;
			if (pos.y >= 800)pos.y = -180;
		}
	}
	void over() {
		if (real_frame_counter % 2 == 0)downspeed++;
		pos.y += downspeed;
	}
	void clear() {
		pos.x = 280;
		pos.y = -150;
		uptimes = 0;
		downspeed = 4;
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
	barrier(int n,int seed,int hardtype):flag(false){
		srand(seed);
		pos.x = 1440;
		type = n;
		switch (type) {
		case 1:
			loadimage(&barrier_res, _T("img/barrier/0.png"), 100, 400);
			pos.y = rand() % (hardtype * 3 + 260)-290;
			break;
		case 2:
			loadimage(&barrier_res, _T("img/barrier/1.png"), 100, 400);
			pos.y = rand() % (hardtype * 3 + 250) + 480;
			break;
		}
	}
	void go_left(int speedup) {
		pos.x -= 4 + speedup;
		X.first = pos.x - 53, X.second = pos.x + 40;
		Y.first = pos.y - 73, Y.second = pos.y + 360;
	}
};