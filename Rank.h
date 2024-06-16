#pragma once

#include <vector>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <iostream>
#include "graphics.h"  // EasyX��ͷ�ļ�
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

    void clearRank();          // ����������а����ݵ�˽�к���
    void displayEmptyMessage() const; // ������ʾ�����а�Ϊ�ա���˽�к���
};

bool comparePlayers(const Player& a, const Player& b);
