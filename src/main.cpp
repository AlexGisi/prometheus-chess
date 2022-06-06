#include <iostream>
#include "Board.h"
#include "Defs.h"
#include "PosKey.h"
#include "Move.h"
#include "MoveGen.h"

#define PAWNMOVESW "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define PAWNMOVESB "rnbqkbnr/p1p1p3/3p3p/1p1p4/2P1Pp2/8/PP1P1PpP/RNBQKB1R b KQkq e3 0 1"
#define KNIGHTSKINGS "5k2/1n6/4n3/6N1/8/3N4/8/5K2 w - - 0 1"
#define ROOKS "6k1/8/5r2/8/1nR5/5N2/8/6K1 b - - 0 1"
#define QUEENS "6k1/8/4nq2/8/1nQ5/5N2/1N6/6K1 b - - 0 1 "
#define BISHOPS "6k1/1b6/4n3/8/1n4B1/1B3N2/1N6/2b3K1 b - - 0 1"
#define CASTLE1 "r3k2r/8/8/8/8/8/8/R3K2R w KQkq - 0 1"
#define CASTLE2 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

void init() {
    Board::initSq120To64();
    Board::initFilesRanksBrd();
    BitBoard::initBitMasks();
    PosKey::initHashKeys();
}

int main() {
    init();

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

        std::cout << "Made: " << mv.toStr() << std::endl;
        board.print();
        board.takeMove();
        std::cout << "Taken: " << mv.toStr() << std::endl;
        board.print();

        getchar();
    }

    return 0;
}
