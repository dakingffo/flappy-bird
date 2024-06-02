#include "class.h"
#include "anime_and_sound.h"
bool checkend(bool &gravity, bird& player,vector<barrier>::iterator i,int &overtype) {
	if (i->type == 1 && player.pos.y <= i->Y.second || i->type == 2 && player.pos.y >= i->Y.first)
		if (i->X.first - 10 <= player.pos.x && player.pos.x <= i->X.first + 10) {
			overtype = 1;
			return true;
		}
	if (i->X.first <= player.pos.x && player.pos.x <= i->X.second)
		if (i->type == 1 && player.pos.y <= i->Y.second) {
			overtype = 2;
			return true;
		}
		else if (i->type == 2 && player.pos.y >= i->Y.first) {
			overtype = 3;
			gravity = false;
			return true;
		}
	return false;
}
int main() {
	int best;
	FILE* fp;
	fopen_s(&fp,"record.txt", "r");
	if (fscanf_s(fp, "%d", &best)==EOF)best = 0;
	fclose(fp);
	srand(time(0));
	srand(rand());
	ExMessage msg;
	initgraph(1280, 720);
	bool running = true;
	bool gaming = false;
	bird title_bird(540,35,170,170);
	play_button theplay(435, 470, 370, 150, 0,&gaming);
	quit_button thequit(890, 470, 160, 150, 1,&running);
	bgm_button thebgm(200, 470, 150, 150, 2);
	board theboard;
	bird player;
	vector<barrier>barriers;
	text thetext;
	BeginBatchDraw();
	load_background();
	sound();
	while (running) {
		DWORD begin_time = GetTickCount();
		menu_animation(title_bird,thequit,theplay,thebgm,thetext,best);
		while (peekmessage(&msg)) {
			if (msg.message == WM_LBUTTONDOWN) {
				if (theplay.check(msg.x, msg.y)) {
					mciSendString(_T("play button from 0"), NULL, 0, NULL);
					button_animing = true;
				}
				else if (thequit.check(msg.x, msg.y)) { 
					mciSendString(_T("play button from 0"), NULL, 0, NULL);
					Sleep(150);
					button_animing = true; 
				}
				else if (thebgm.check(msg.x, msg.y)) { 
					mciSendString(_T("play bgm_button from 0"), NULL, 0, NULL); 
				    button_animing = true; 
				}
			}
			 if (gaming) {
				int gaming_frame_counter = 0;
				int board_down_time = 0;
				bool gravity = true;
				int end = 0;
				int overtype = 0;
				int hardtype = 0;
				int point_count = 0;
				while (gaming) {
					gaming_frame_counter++;
					DWORD begin_time = GetTickCount();
					if (end == 1)gaming = false;
					else if (end > 1)end--;
					else for (auto i = barriers.begin(); i != barriers.end();) {
						i->go_left(hardtype);
						if (checkend(gravity,player, i, overtype)) {
							end = 150;
							mciSendString(_T("play dead from 0"), NULL, 0, NULL);
						}
						else if (!i->flag && player.pos.x > i->pos.x) {
							i->flag = true;
							mciSendString(_T("play point from 0"), NULL, 0, NULL);
							point_count++;
						}
						if (i->pos.x < -200) i = barriers.erase(i);
						else i++;
					};
					if (end == 0)player.go_down(gravity);
					else if (end >= 65)player.downspeed = 0;
					else if (end < 65)player.over();
					if (end == 0)
						if (!player.uptimes)
							while (peekmessage(&msg)) {
								if (msg.message == WM_KEYDOWN && msg.vkcode == VK_UP || msg.message == WM_LBUTTONDOWN) {
									board_down_time = 13;
									mciSendString(_T("play jump from 0"), NULL, 0, NULL);
									player.uptimes = 20;
									player.downspeed = 3;
								}
							}
						else {
							board_down_time--;
							player.go_up();
						}
					if (gaming_frame_counter % (140 - 20 * hardtype) == 0) {
						barriers.push_back(barrier(1, rand() % (100 + hardtype), hardtype));
						barriers.push_back(barrier(2, rand() % (100 + hardtype), hardtype));
					}
					hardtype += gaming_frame_counter / 1080, gaming_frame_counter %= 1080;
					gaming_animation(end, overtype, player, barriers, theboard, board_down_time,thetext);
					theboard.show_point(point_count);
					FlushBatchDraw();
					DWORD end_time = GetTickCount();
					DWORD dtime = end_time - begin_time;
					if (dtime < 1000 / 75)
						Sleep(1000 / 75 - dtime);
				}
				if (point_count/2 > best) {
					best = point_count/2;
					fopen_s(&fp, "record.txt", "w");
					fprintf_s(fp, "%d", best);
					fclose(fp);
				}
				barriers.clear();
				player.clear();
			 }
		}
		FlushBatchDraw();
		DWORD end_time = GetTickCount();
		DWORD dtime = end_time - begin_time;
		if (dtime < 1000 / 75)
			Sleep(1000 / 75 - dtime);
	}
	EndBatchDraw();
	return 0;
}