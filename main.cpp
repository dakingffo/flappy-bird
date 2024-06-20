#include "class.h"
#include "anime_and_sound.h"
#include "ctime"

// 检查游戏结束条件的函数
bool CheckEnd(bool& Gravity, Bird& Player, vector<Barrier>::iterator i, int& OverType) {
    if (i->type == 1 && Player.pos.y <= i->Y.second || i->type == 2 && Player.pos.y >= i->Y.first) {
        if (i->X.first - 10 <= Player.pos.x && Player.pos.x <= i->X.first + 10) {
            OverType = 1;
            return true;
        }
    }
    if (i->X.first <= Player.pos.x && Player.pos.x <= i->X.second) {
        if (i->type == 1 && Player.pos.y <= i->Y.second) {
            OverType = 2;
            return true;
        } else if (i->type == 2 && Player.pos.y >= i->Y.first) {
            OverType = 3;
            Gravity = false;
            return true;
        }
    }
    return false;
}

int main() {
    int Best;
    FILE* fp;
    fopen_s(&fp, "rank.txt", "r");
    if (fscanf_s(fp, "%d", &Best) == EOF) Best = 0;
    fclose(fp);

    srand(time(0));
    srand(rand());

    ExMessage Msg;
    initgraph(1280, 720);

    bool Running = true;
    bool Gaming = false;
    Bird TitleBird(540, 35, 170, 170);
    PlayButton ThePlay(440, 470, 370, 150, 0, &Gaming);
    QuitButton TheQuit(890, 471, 160, 150, 1, &Running);
    BgmButton TheBgm(200, 466, 150, 150, 2);
    Board TheBoard;
    Bird Player;
    vector<Barrier> Barriers;
    Text TheText;

    BeginBatchDraw();
    load_background();
    sound();

    while (Running) {
        DWORD BeginTime = GetTickCount();
        menu_animation(TitleBird, TheQuit, ThePlay, TheBgm, TheText, Best, Gaming);

        while (peekmessage(&Msg)) {
            if (Msg.message == WM_LBUTTONDOWN) {
                if (ThePlay.check(Msg.x, Msg.y)) {
                    mciSendString(_T("play button from 0"), NULL, 0, NULL);
                    ButtonAniming = true;
                } else if (TheQuit.check(Msg.x, Msg.y)) {
                    mciSendString(_T("play button from 0"), NULL, 0, NULL);
                    Sleep(150);
                    ButtonAniming = true;
                } else if (TheBgm.check(Msg.x, Msg.y)) {
                    mciSendString(_T("play bgm_button from 0"), NULL, 0, NULL);
                    ButtonAniming = true;
                }
            }

            if (Gaming) {
                int GamingFrameCounter = 0;
                int BoardComeOut = 65;
                int BoardDownTime = 0;
                bool Gravity = true;
                int End = 0;
                int OverType = 0;
                int HardType = 0;
                int PointCount = 0;
                bool NewRecord = false;

                while (Gaming) {
                    BoardComeOut--;
                    GamingFrameCounter++;
                    DWORD BeginTime = GetTickCount();

                    if (End == 1) Gaming = false;
                    else if (End > 1) End--;
                    else {
                        for (auto i = Barriers.begin(); i != Barriers.end();) {
                            i->go_left(HardType);
                            if (CheckEnd(Gravity, Player, i, OverType)) {
                                End = 150;
                                mciSendString(_T("play dead from 0"), NULL, 0, NULL);
                            } else if (!i->flag && Player.pos.x > i->pos.x) {
                                i->flag = true;
                                mciSendString(_T("play point from 0"), NULL, 0, NULL);
                                Player.getting_point = 40;
                                PointCount++;
                            }
                            if (i->pos.x < -200) i = Barriers.erase(i);
                            else i++;
                        }
                    }

                    if (End == 0) Player.go_down(Gravity);
                    else if (End >= 65) Player.downspeed = 0;
                    else if (End < 65) Player.over();

                    if (End == 0) {
                        if (!Player.uptimes) {
                            while (peekmessage(&Msg)) {
                                if (Msg.message == WM_KEYDOWN && Msg.vkcode == VK_UP || Msg.message == WM_LBUTTONDOWN) {
                                    BoardDownTime = 13;
                                    mciSendString(_T("play jump from 0"), NULL, 0, NULL);
                                    Player.uptimes = 20;
                                    Player.downspeed = 3;
                                }
                            }
                        } else {
                            BoardDownTime--;
                            Player.go_up();
                        }
                    }

                    if (GamingFrameCounter % (140 - 20 * HardType) == 0) {
                        Barriers.push_back(Barrier(1, rand() % (100 + HardType), HardType));
                        Barriers.push_back(Barrier(2, rand() % (100 + HardType), HardType));
                    }

                    HardType += GamingFrameCounter / 1080;
                    GamingFrameCounter %= 1080;

                    if (PointCount / 2 > Best) NewRecord = true;

                    gaming_animation(End, OverType, Player, Barriers, TheBoard, BoardDownTime, TheText, NewRecord, BoardComeOut);
                    TheBoard.show_point(PointCount, BoardComeOut);

                    FlushBatchDraw();

                    DWORD EndTime = GetTickCount();
                    DWORD DTime = EndTime - BeginTime;
                    if (DTime < 1000 / 75)
                        Sleep(1000 / 75 - DTime);
                }

                if (PointCount / 2 > Best) {
                    Best = PointCount / 2;
                    fopen_s(&fp, "rank.txt", "w");
                    fprintf_s(fp, "%d", Best);
                    fclose(fp);
                }

                Barriers.clear();
                Player.clear();
            }
        }

        FlushBatchDraw();
        DWORD EndTime = GetTickCount();
        DWORD DTime = EndTime - BeginTime;
        if (DTime < 1000 / 75)
            Sleep(1000 / 75 - DTime);
    }

    EndBatchDraw();
    return 0;
}
