#pragma once
#include "class.h"

// 全局变量声明
int RealFrameCounter = 0;
int BackGroundFrameCounter = 0;
int BirdFrameCounter = 0;
int ButtonFrameCounter = 0;
int BirdAnimNum = 50;
bool ButtonAniming = false;
const int BackgroundAnimNum = 480;
IMAGE Background[BackgroundAnimNum];

// 带透明度的图片显示函数
inline void putimage_alpha(int x, int y, IMAGE* img) {
    int w = img->getwidth();
    int h = img->getheight();
    AlphaBlend(GetImageHDC(NULL), x, y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER,0,255,AC_SRC_ALPHA });
}

// 菜单动画函数
inline void menu_animation(Bird& TitleBird, QuitButton& TheQuit, PlayButton& ThePlay, BgmButton& TheBgm, Text& TheText, int Best, bool Gaming) {
    if (++RealFrameCounter % 2 == 0)
        BackGroundFrameCounter++;
    if (RealFrameCounter % 3 == 0)
        BirdFrameCounter++;

    BackGroundFrameCounter = BackGroundFrameCounter % BackgroundAnimNum;

    BirdFrameCounter = BirdFrameCounter % BirdAnimNum;

    cleardevice();
    putimage(0, 0, &Background[BackGroundFrameCounter]);

    if (!ButtonAniming && !Gaming || TheBgm.animing)
        putimage_alpha(TitleBird.pos.x, TitleBird.pos.y = 35, &TitleBird.bird_anim[BirdFrameCounter]);
    else
        putimage_alpha(TitleBird.pos.x, TitleBird.pos.y -= 17, &TitleBird.bird_anim[BirdFrameCounter]);

    for (size_t i = 0; i < 2; i++)
        putimage_alpha(TheText.pos[i].x, TheText.pos[i].y, &TheText.context[i]);

    if (ButtonAniming && RealFrameCounter % 6 == 0)
        ButtonFrameCounter++;

    TheQuit.animate(ButtonFrameCounter);
    ThePlay.animate(ButtonFrameCounter);
    TheBgm.animate(ButtonFrameCounter);

    if (ButtonFrameCounter == 2)
        ButtonAniming = false, ButtonFrameCounter = 0;

    settextstyle(85, 45, _T("方正姚体"));
    settextcolor(YELLOW);
    setbkmode(TRANSPARENT);
    outtextxy(850, 296, reinterpret_cast<LPCTSTR>(to_wstring(Best).c_str()));
}

// 游戏动画函数
inline void gaming_animation(int End, int OverType, Bird& Player, vector<Barrier>& Barriers, Board& TheBoard, int BoardDownTime, Text& TheText, bool NewRecord, int BoardComeOut) {
    if (++RealFrameCounter % 2 == 0)
        BackGroundFrameCounter++;
    if (RealFrameCounter % 3 == 0)
        BirdFrameCounter++;

    BackGroundFrameCounter = BackGroundFrameCounter % BackgroundAnimNum;
    BirdFrameCounter = BirdFrameCounter % BirdAnimNum;

    cleardevice();
    putimage(0, 0, &Background[BackGroundFrameCounter]);

    if (!End)
        putimage_alpha(Player.pos.x, Player.pos.y, &Player.bird_anim[BirdFrameCounter]);
    else
        switch (OverType) {
            case 1: putimage_alpha(Player.pos.x, Player.pos.y, &Player.bird_over1); break;
            case 2: putimage_alpha(Player.pos.x, Player.pos.y, &Player.bird_over2); break;
            case 3: putimage_alpha(Player.pos.x, Player.pos.y, &Player.bird_over3[BirdFrameCounter]); break;
        }

    if (Player.getting_point) {
        static int Y;
        if (Player.getting_point == 40)
            Y = Player.pos.y;
        putimage_alpha(Player.pos.x + 65, Y - 19 * (4 - Player.getting_point / 10), &Player.point[Player.getting_point / 10 - 1]);
        if (RealFrameCounter % 3 == 0)
            Player.getting_point -= 10;
    }

    if (End > 0 && NewRecord)
        putimage_alpha(TheText.pos[2].x, TheText.pos[2].y, &TheText.context[2]);

    for (int i = 0; i < Barriers.size(); i++)
        putimage_alpha(Barriers[i].pos.x, Barriers[i].pos.y, &Barriers[i].barrier_res);

    TheBoard.stage(BoardDownTime, BoardComeOut);
}

// 声音初始化函数
inline void sound() {
    mciSendString(_T("open sound/get_point_sound.mp3 alias point"), NULL, 0, NULL);
    mciSendString(_T("open sound/dead_sound.mp3 alias dead"), NULL, 0, NULL);
    mciSendString(_T("open sound/jump_sound.mp3 alias jump"), NULL, 0, NULL);
    mciSendString(_T("open sound/back_ground_music.mp3 alias bgm"), NULL, 0, NULL);
    mciSendString(_T("open sound/button1.mp3 alias bgm_button"), NULL, 0, NULL);
    mciSendString(_T("open sound/button0.mp3 alias button"), NULL, 0, NULL);
    mciSendString(_T("play bgm repeat from 0"), NULL, 0, NULL);
}

// 背景加载函数
void load_background() {
    for (size_t i = 0; i < BackgroundAnimNum; i++) {
        std::wstring path;
        if (i >= 100)
            path = L"img/background/pics/pics_00" + std::to_wstring(i) + L".png";
        else if (i >= 10)
            path = L"img/background/pics/pics_000" + std::to_wstring(i) + L".png";
        else
            path = L"img/background/pics/pics_0000" + std::to_wstring(i) + L".png";
        loadimage(&Background[i], reinterpret_cast<LPCTSTR>(path.c_str()));
    }
}
