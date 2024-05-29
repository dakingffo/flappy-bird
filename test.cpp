#include "class.h"
using namespace std;
bird player;
vector<barrier>barriers;
int overtype=0;
int real_frame_counter = 0;
int back_ground_frame_counter = 0;
int bird_frame_counter = 0;
int barrier_frame_counter = 0;
const int background_anim_num =480;
IMAGE background[background_anim_num];
inline void putimage_alpha(int x, int y, IMAGE* img) {
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}
inline void anime(int end,int overtype=0) {
		if (++real_frame_counter % 2 == 0)
			back_ground_frame_counter++, barrier_frame_counter++;
		if (real_frame_counter % 3 == 0)bird_frame_counter++;
		back_ground_frame_counter = back_ground_frame_counter % background_anim_num;
		bird_frame_counter = bird_frame_counter % bird_anim_num;
		barrier_frame_counter = barrier_frame_counter % 100;
		cleardevice();
		putimage(0, 0, &background[back_ground_frame_counter]);
		if (!end)putimage_alpha(player.pos.x, player.pos.y, &bird_anim[bird_frame_counter]);
		else if(overtype==1)putimage_alpha(player.pos.x, player.pos.y, &bird_over1);
		else if(overtype==2)putimage_alpha(player.pos.x, player.pos.y, &bird_over2);
		else if(overtype==3)putimage_alpha(player.pos.x, player.pos.y, &bird_over3[bird_frame_counter]);
		for (int i = 0; i < barriers.size(); i++)
			putimage_alpha(barriers[i].pos.x, barriers[i].pos.y, &barriers[i].barrier_res);
}
inline void sound() {
	mciSendString(_T("open sound/get_point_sound.mp3 alias point"), NULL, 0, NULL);
	mciSendString(_T("open sound/dead_sound.mp3 alias dead"), NULL, 0, NULL);
	mciSendString(_T("open sound/jump_sound.mp3 alias jump"), NULL, 0, NULL);
	mciSendString(_T("open sound/back_ground_music.mp3 alias bgm"), NULL, 0, NULL);
	mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);
}
void load_background() {
	for (size_t i = 0; i <background_anim_num;i++) {
		std::wstring path;
		if(i>=100)
		path = L"img/background/合成 1/合成 1_00" + std::to_wstring(i) + L".png";
		else if(i>=10)
		path = L"img/background/合成 1/合成 1_000" + std::to_wstring(i) + L".png";
		else 
		path = L"img/background/合成 1/合成 1_0000" + std::to_wstring(i) + L".png";
		loadimage(&background[i], path.c_str());
	}
}
bool checkend(int*end,bool *gravity, vector<barrier>::iterator i) {
	if (i->type == 1 && player.pos.y <= i->Y.second || i->type == 2 && player.pos.y >= i->Y.first)
		if (i->X.first - 10 <= player.pos.x && player.pos.x <= i->X.first + 10) {
			*end = 100;
			overtype = 1;
			return true;
		}
	if (i->X.first <= player.pos.x && player.pos.x <= i->X.second)
		if (i->type == 1 && player.pos.y <= i->Y.second) {
			*end = 100;
			overtype = 2;
			return true;
		}
		else if (i->type == 2 && player.pos.y >= i->Y.first) {
			*end = 100;
			overtype = 3;
			*gravity = false;
			return true;
		}
	return false;
}
int main() {
	srand(time(0));
	srand(rand());
	ExMessage msg;
	initgraph(1280, 720);
	bool running = true;
	int end = 0;
	int speedup = 0;
	bool gravity = true;
	BeginBatchDraw();
	load_background();
	sound();
	while (running) {
		DWORD begin_time = GetTickCount();
		if (end == 1)running = false;
		else if (end > 1)end--;
		else for (auto i = barriers.begin(); i != barriers.end();) {
			i->go_left(speedup);
			if(checkend(&end,&gravity,i))mciSendString(_T("play dead from 0"), NULL, 0, NULL);
			else if (!i->flag&&player.pos.x > i->pos.x) {
				i->flag = true;
				mciSendString(_T("play point from 0"), NULL, 0, NULL);
			}
			if (i->pos.x < -200) i = barriers.erase(i);
			else i++;
		};
		if (end == 0)player.go_down(gravity);
		else if(end>=65)player.downspeed = 0;
	    else if (end < 65)player.over();
		if(end == 0)
			if(!player.uptimes)
			while (peekmessage(&msg)) {
			if (msg.message == WM_KEYDOWN) {
				if (msg.vkcode == VK_UP) {
					mciSendString(_T("play jump from 0"), NULL, 0, NULL);
					player.uptimes = 20;
					player.downspeed = 3;
				}
			}
		}else
			player.go_up();
		if (real_frame_counter % (140 - 15 * speedup) == 0) {
			barriers.push_back(barrier(1, rand() % 100));
			barriers.push_back(barrier(2, rand() % 100));
		}
		anime(end,overtype);
		if (real_frame_counter / 1080 != 0)speedup += real_frame_counter / 520, real_frame_counter %= 520;
		FlushBatchDraw();
		DWORD end_time = GetTickCount();
		DWORD dtime = end_time-begin_time;
		if (dtime < 1000 / 75)
			Sleep(1000 / 75 - dtime);
	}
	EndBatchDraw();
	return 0;
}