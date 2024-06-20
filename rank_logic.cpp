#include "Rank.h"

int main() {
    Rank rank("rank.txt");
    rank.loadData();
    rank.sortPlayers();
    rank.display();
    while (true);
    return 0;
}
