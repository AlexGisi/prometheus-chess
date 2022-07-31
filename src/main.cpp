#include <iostream>
#include "Board.h"
#include "Defs.h"
#include "PosKey.h"
#include "Move.h"
#include "MoveGen.h"
#include "search.cpp"
#include "uci.h"

#define fen1 "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1"
#define mate_in_3 "2rr3k/pp3pp1/1nnqbN1p/3pN3/2pP4/2P3Q1/PPB4P/R4RK1 w - -"
#define w "r1b1k2r/ppppnppp/2n2q2/2b5/3NP3/2P1B3/PP3PPP/RN1QKB1R w KQkq - 0 1"

using namespace std;

void init() {
    Board::initSq120To64();
    Board::initFilesRanksBrd();
    BitBoard::initBitMasks();
    PosKey::initHashKeys();
    MoveGen::initMvvLva();
}

void human() {
    int pv_num = 0;
    int max = 0;

    Board b(w);
    MoveList ml;
    SearchInfo info;

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
        else if(input == "p") {
            max = b.getPVLine(4);
            printf("PvLine of %d moves: ", max);
            for(pv_num = 0; pv_num < max; pv_num++) {
                Move mv = b.pvArray[pv_num];
                printf(" %s", mv.to_str().c_str());
            }
            cout << endl;
        }
        else if(input == "s") {
            info.depth = 6;
            search(b, info);
        }
        else {
            try {
                Move mv = b.getMove(input);
                b.pvTable.store(b.posKey, mv);
                b.makeMove(mv);
            } catch (std::invalid_argument& e) {
                cout << "Move invalid" << endl;
            }

        }
    } while(!q);
}

void perft() {
    Board b(START_FEN);
    b.perft_suite(4);
}

void basicTest() {
    Board board(START_FEN);
    MoveList ml;
    MoveGen mg(&board);
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
    uci_loop();

    return 0;
}
