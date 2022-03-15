#include <iostream>
#include "Board.h"
#include "Defs.h"
#include "PosKey.h"

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

    assert(b1.checkBoard());

    return 0;
}
