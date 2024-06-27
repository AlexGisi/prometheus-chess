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
    Board::initialize_lookup_tables();
    BitBoard::initBitMasks();
    PosKey::initHashKeys();
    MoveGen::initMvvLva();
}

void perft(int depth, const string& resultsfile) {
    if (depth > 6) {
        cout << "perft depth must be <= 6" << endl;
        return;
    }
    Board b(START_FEN);
    b.perft_suite(depth, resultsfile);
}

int main(int argc, char** argv) {
    init();
    if (argc > 1) {
        string subcommand = argv[1];
        if (subcommand == "perft") {
            if (argc == 4) {
                string depth = argv[2];
                string resultsfile = argv[3];
                perft(std::stoi(depth), resultsfile);
            } else {
                cout << "usage: perft [depth] [resultsfile]" << endl;
                return -1;
            }
        } else {
            cout << "subcommands: perft" << endl;
        }
    } else {
        uci_loop();
    }
    return 0;
}
