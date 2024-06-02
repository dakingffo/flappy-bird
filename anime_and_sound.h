#pragma once
#include "class.h"
int real_frame_counter = 0;
int back_ground_frame_counter = 0;
int bird_frame_counter = 0;
int button_frame_counter = 0;
bool button_animing = false;
const int background_anim_num = 480;
IMAGE background[background_anim_num];
inline void putimage_alpha(int x, int y, IMAGE* img) {
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}
inline void menu_animation(bird& title_bird, quit_button& thequit, play_button& theplay, bgm_button& thebgm, text& thetext,int best) {
	if (++real_frame_counter % 2 == 0)
		back_ground_frame_counter++;
	if (real_frame_counter % 3 == 0)bird_frame_counter++;
	back_ground_frame_counter = back_ground_frame_counter % background_anim_num;
	bird_frame_counter = bird_frame_counter % bird_anim_num;
	cleardevice();
	putimage(0, 0, &background[back_ground_frame_counter]);
	putimage_alpha(title_bird.pos.x, title_bird.pos.y, &title_bird.bird_anim[bird_frame_counter]);
	for (size_t i = 0; i < 2; i++) {
		putimage_alpha(thetext.pos[i].x, thetext.pos[i].y, &thetext.context[i]);
	}
	if (button_animing&&real_frame_counter % 6 == 0)
		button_frame_counter++;
	thequit.anime(button_frame_counter);
	theplay.anime(button_frame_counter);
	thebgm.anime(button_frame_counter);
	if (button_frame_counter == 2)button_animing = false, button_frame_counter = 0;
	settextstyle(85, 45, _T("����Ҧ��"));
	settextcolor(YELLOW);
	setbkmode(TRANSPARENT);
	outtextxy(850, 296, to_wstring(best).c_str());
}
inline void gaming_animation(int end, int overtype , bird& player, vector<barrier>& barriers,board& theboard,int board_down_time,text&thetext) {
	if (++real_frame_counter % 2 == 0)
		back_ground_frame_counter++;
	if (real_frame_counter % 3 == 0)bird_frame_counter++;
	back_ground_frame_counter = back_ground_frame_counter % background_anim_num;
	bird_frame_counter = bird_frame_counter % bird_anim_num;
	cleardevice();
	putimage(0, 0, &background[back_ground_frame_counter]);
	if (!end)putimage_alpha(player.pos.x, player.pos.y, &player.bird_anim[bird_frame_counter]);
	else if (overtype == 1)putimage_alpha(player.pos.x, player.pos.y, &player.bird_over1);
	else if (overtype == 2)putimage_alpha(player.pos.x, player.pos.y, &player.bird_over2);
	else if (overtype == 3)putimage_alpha(player.pos.x, player.pos.y, &player.bird_over3[bird_frame_counter]);
	if(end>0)putimage_alpha(thetext.pos[2].x, thetext.pos[2].y, &thetext.context[2]);
	for (int i = 0; i < barriers.size(); i++)
		putimage_alpha(barriers[i].pos.x, barriers[i].pos.y, &barriers[i].barrier_res);
	theboard.stage(board_down_time);
}
inline void sound() {
	mciSendString(_T("open sound/get_point_sound.mp3 alias point"), NULL, 0, NULL);
	mciSendString(_T("open sound/dead_sound.mp3 alias dead"), NULL, 0, NULL);
	mciSendString(_T("open sound/jump_sound.mp3 alias jump"), NULL, 0, NULL);
	mciSendString(_T("open sound/back_ground_music.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("open sound/button1.mp3 alias bgm_button"), NULL, 0, NULL);
	mciSendString(_T("open sound/button0.mp3 alias button"), NULL, 0, NULL);
	mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);
}
void load_background() {
	for (size_t i = 0; i < background_anim_num; i++) {
		std::wstring path;
		if (i >= 100)
			path = L"img/background/�ϳ� 1/�ϳ� 1_00" + std::to_wstring(i) + L".png";
		else if (i >= 10)
			path = L"img/background/�ϳ� 1/�ϳ� 1_000" + std::to_wstring(i) + L".png";
		else
			path = L"img/background/�ϳ� 1/�ϳ� 1_0000" + std::to_wstring(i) + L".png";
		loadimage(&background[i], path.c_str());
	}
}