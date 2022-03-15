#include <iostream>
#include "Board.h"
#include "Defs.h"
#include "PosKey.h"

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

    b1.showSqAttBySide(WHITE);

    return 0;
}
