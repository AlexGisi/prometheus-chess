#include <iostream>
#include "Board.h"
#include "Defs.h"
#include "PosKey.h"
#include "Move.h"
#include "MoveGen.h"

using namespace std;

void init() {
    Board::initSq120To64();
    Board::initFilesRanksBrd();
    BitBoard::initBitMasks();
    PosKey::initHashKeys();
}

int main() {
    init();

    Board board(START_FEN);
    u64 n = board.perft(4);
    cout << "perft: " << n << endl;

    return 0;
}

void basicTest() {
    Board board(START_FEN);
    MoveList ml;
    MoveGen mg(board);
    mg.generateAllMoves(&ml);

    int mvNum = 0;
    Move mv;

    for(mvNum = 0; mvNum < ml.count; ++mvNum) {
        mv = ml.moves[mvNum].move;

        if (!board.makeMove(mv))
            continue;

        std::cout << "Made: " << mv.to_str() << std::endl;
        board.print();
        board.takeMove();
        std::cout << "Taken: " << mv.to_str() << std::endl;
        board.print();

        getchar();
    }
}
