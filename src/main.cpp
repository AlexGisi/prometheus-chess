#include <iostream>
#include "Board.h"
#include "Defs.h"
#include "PosKey.h"
#include "Move.h"
#include "MoveGen.h"

#define fen1 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"

using namespace std;

void init() {
    Board::initSq120To64();
    Board::initFilesRanksBrd();
    BitBoard::initBitMasks();
    PosKey::initHashKeys();
}

void human() {
    Board b(START_FEN);
    std::string input;
    bool q = false;
    do {
        b.print();
        cout << "Enter move> ";
        cin >> input;

        if(input == "q")
            q = true;
        else if(input == "t")
            b.takeMove();
        else {
            try {
                Move mv = b.getMove(input);
                b.makeMove(mv);
            } catch (std::invalid_argument& e) {
                cout << "Move invalid";
            }

        }
    } while(!q);
}

void perft() {
    Board b(START_FEN);
    b.perft_suite(3);
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

int main() {
    init();

    human();

    return 0;
}
