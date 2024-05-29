#include "class.h"
#include "anime_and_sound.h"
using namespace std;
int overtype=0;
int point_count = 0;
board theboard;
bird player;
vector<barrier>barriers;
bool checkend(int* end,bool *gravity, vector<barrier>::iterator i) {
	if (i->type == 1 && player.pos.y <= i->Y.second || i->type == 2 && player.pos.y >= i->Y.first)
		if (i->X.first - 10 <= player.pos.x && player.pos.x <= i->X.first + 10) {
			*end = 120;
			overtype = 1;
			return true;
		}
	if (i->X.first <= player.pos.x && player.pos.x <= i->X.second)
		if (i->type == 1 && player.pos.y <= i->Y.second) {
			*end = 120;
			overtype = 2;
			return true;
		}
		else if (i->type == 2 && player.pos.y >= i->Y.first) {
			*end = 120;
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
	int board_down_time = 0;
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
				point_count++;
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
					board_down_time = 10;
					mciSendString(_T("play jump from 0"), NULL, 0, NULL);
					player.uptimes = 20;
					player.downspeed = 3;
				}
			}
			}
			else {
				board_down_time--;
				player.go_up();
			}
		if (real_frame_counter % (140 - 20 * speedup) == 0) {
			barriers.push_back(barrier(1, rand() % 100));
			barriers.push_back(barrier(2, rand() % 100));
		}
		speedup += real_frame_counter / 1080, real_frame_counter %= 1080;
		anime(end,overtype,player,barriers,theboard,board_down_time);
		theboard.show_point(point_count);
		FlushBatchDraw();
		DWORD end_time = GetTickCount();
		DWORD dtime = end_time-begin_time;
		if (dtime < 1000 / 75)
			Sleep(1000 / 75 - dtime);
	}
	EndBatchDraw();
	return 0;
}