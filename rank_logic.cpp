#include "Leaderboard.h"

int main() {
    Leaderboard leaderboard("record.txt");
    leaderboard.loadData();
    leaderboard.sortPlayers();
    leaderboard.display();
    while (true);
    return 0;
}
