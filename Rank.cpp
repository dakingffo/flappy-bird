#include "Rank.h"
#include "vector"
#include <graphics.h>
#include <conio.h> // for _getch()

Rank::Rank(const std::string& filename) : filename(filename) {}

void Rank::loadData() {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "�޷����ļ�: " << filename << std::endl;
        return;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string name, scoreStr, date;
        std::getline(ss, name, ',');
        std::getline(ss, scoreStr, ',');
        std::getline(ss, date, ',');

        Player player(name, std::stoi(scoreStr), date);
        players.push_back(player);
    }

    file.close();
}

void Rank::sortPlayers() {
    std::sort(players.begin(), players.end(), comparePlayers);
}

void Rank::display(){
    initgraph(500, 600);  // ��ʼ��ͼ�δ���

    IMAGE background;
    loadimage(&background, _T("img/background.jpg"));
    putimage(0, 0, &background);
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(24, 10, "΢���ź�");  // ����������ʽ�ʹ�С
    outtextxy(220, 470, "Quit");
    outtextxy(50, 470, "Reset Rank");
    int y = 150;
    int rank = 1;
    int increase = 60;
    for (const auto& player : players) {
        std::string displayText = std::to_string(rank);
        outtextxy(80, y, displayText.c_str());
        outtextxy(80 + increase, y, player.name.c_str());
        outtextxy(80 + 2.2 * increase, y, std::to_string(player.score).c_str());
        outtextxy(80 + 3 * increase, y, player.date.c_str());
        y += 40;
        rank++;
    }

    // �����˳���ť
    IMAGE quit_button;
    loadimage(&quit_button, _T("img/button/1/2.png"), 100, 50);
    putimage(200, 500, &quit_button);

    // ������հ�ť
    IMAGE reset_button;
    loadimage(&reset_button, _T("img/button/2/1.png"), 100, 50);
    putimage(50, 500, &reset_button);

    // �ȴ��û������ť
    while (true) {
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                // �ж��Ƿ������˳���ť
                if (msg.x >= 200 && msg.x <= 300 && msg.y >= 500 && msg.y <= 550) {
                    closegraph();

                }
                // �ж��Ƿ�������հ�ť
                if (msg.x >= 50 && msg.x <= 150 && msg.y >= 500 && msg.y <= 550) {
                    clearRank();
                    displayEmptyMessage();
                }
            }
        }
    }
}

void Rank::clearRank() {
    players.clear();
    std::ofstream file(filename, std::ios::trunc); // ����ļ�����
    file.close();
}

void Rank::displayEmptyMessage() const {
    initgraph(500, 600);  // ���³�ʼ��ͼ�δ���

    IMAGE background;
    loadimage(&background, _T("img/background.jpg"));
    putimage(0, 0, &background);
    setbkmode(TRANSPARENT);
    settextcolor(BLACK);
    settextstyle(24, 10, "΢���ź�");  // ����������ʽ�ʹ�С

    outtextxy(150, 300, _T("Rankborad is empty"));

    // �����˳���ť
    IMAGE quit_button;
    loadimage(&quit_button, _T("img/button/1/2.png"), 100, 50);
    putimage(200, 500, &quit_button);

    // ������հ�ť
    IMAGE reset_button;
    loadimage(&reset_button, _T("img/button/2/1.png"), 100, 50);
    putimage(50, 500, &reset_button);

    // �ȴ��û������ť
    while (true) {
        if (MouseHit()) {
            MOUSEMSG msg = GetMouseMsg();
            if (msg.uMsg == WM_LBUTTONDOWN) {
                // �ж��Ƿ������˳���ť
                if (msg.x >= 200 && msg.x <= 300 && msg.y >= 500 && msg.y <= 550) {
                    closegraph();

                }
                // �ж��Ƿ�������հ�ť
                if (msg.x >= 50 && msg.x <= 150 && msg.y >= 500 && msg.y <= 550) {
                    //clearRank();
                    displayEmptyMessage();
                }
            }
        }
    }
}


bool comparePlayers(const Player& a, const Player& b) {
    return a.score > b.score;  // �Ӵ�С����
}
