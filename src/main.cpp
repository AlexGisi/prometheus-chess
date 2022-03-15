#include <iostream>
#include "Board.h"
#include "Defs.h"
#include "PosKey.h"
#include "Move.h"

#define FEN1 "8/8/8/3B4/8/8/8/3R4 w - - 0 2"

void init() {
    Board::initSq120To64();
    Board::initFilesRanksBrd();
    BitBoard::initBitMasks();
    PosKey::initHashKeys();
}

int main() {
    init();

    Board b1(FEN1);
    b1.print();

    int m = 0;
    int from = A2; int to = G5;
    int cap = wR; int prom = bB;
    m = (from) | (to << 7) | (cap << 14) | (prom << 20);
    printf("\ndec:%d hex:%X\n", m, m);

    Move move(m);

    std::cout << move.from() << ' ' << move.to() << ' ' << move.captured() << ' ' << move.promoted() << std::endl;
    std::cout << move.toStr() << std::endl;

    return 0;
}
