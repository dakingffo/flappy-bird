#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include "graphics.h"  // EasyX库头文件
#include <conio.h>
#pragma comment(lib, "MSIMG32.LIB")

struct Player {
    std::string name;
    int score;
    std::string date;

    Player(const std::string& n, int s, const std::string& d) : name(n), score(s), date(d) {}
};

class Rank {
public:
    Rank(const std::string& filename);
    void loadData();
    void sortPlayers();
    void display();

private:
    std::string filename;
    std::vector<Player> players;

    void clearRank();          // 新增清空排行榜数据的私有函数
    void displayEmptyMessage() const; // 新增显示“排行榜为空”的私有函数
};

bool comparePlayers(const Player& a, const Player& b);
