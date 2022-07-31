//
// Created by Alex Gisi on 2/17/22.
//

#ifndef ATHENE_BOARD_H
#define ATHENE_BOARD_H

#include <string>
#include "BitBoard.h"
#include "Defs.h"
#include "PosKey.h"
#include "Move.h"
#include "PVTable.h"

class Board {
public:
    Board();  // Create an empty board.
    explicit Board(const std::string& fen);

    Board(const Board& rhs);
    Board& operator=(const Board& rhs);

    void setUpEmpty();
    void setUp(const std::string& fen);

    static int sq120ToSq64[BRD_SQ_NUM];
    static int sq64ToSq120[64];
    static int filesBrd[BRD_SQ_NUM];
    static int ranksBrd[BRD_SQ_NUM];
    #define sq64(sq) Board::sq120ToSq64[sq]
    #define sq120(sq) Board::sq64ToSq120[sq]

    static void initSq120To64();
    static void initFilesRanksBrd();

    static std::string sqToStr(int sq);

    void reset();
    void updateListsMaterial();
    [[nodiscard]] bool checkBoard() const;

    [[nodiscard]] bool sqAttacked(int sq, int att_side) const;
    void showSqAttBySide(int side) const;

    [[nodiscard]] std::string to_str() const;
    void print() const;

    int pieces[BRD_SQ_NUM];
    BitBoard pawns[3];  // WHITE, BLACK, BOTH.

    int kingSq[2];

    int side;
    int enPas;
    int fiftyMove;

    int ply;
    int hisPly;  // History.

    PosKey posKey;

    int pceNum[13];  // Number of pieces of each type.
    int bigPce[2];  // Number of 'big pieces', i.e. not pawns.
    int majPce[2];  // Number of 'major pieces', i.e. rooks and queens.
    int minPce[2];  // Number of 'minor pieces', bishops and knights.
    int material[2];  // Sum of material values for white and black's pieces.

    int castlePerm;  // All castle permissions represented by four bits.

    typedef struct {
        // Move about to be played.
        Move move;

        // States prior to move.
        int castlePerm;
        int enPas;
        int fiftyMove;
        PosKey posKey;
    } S_UNDO;

    S_UNDO history[MAX_GAME_MOVES];

    // Track positions for each piece, so we don't have to loop through pieces[] to
    // find piece positions.
    // Ex:
    // pceList[wN][0] = E1;
    // pceList[wN][1] = D4;
    // ...
    //
    int pceList[13][10];

    PVTable pvTable;
    Move pvArray[MAX_DEPTH];
    friend class PosKey;

    // Arrays for move ordering.
    int searchHistory[13][BRD_SQ_NUM];
    Move searchKillers[2][MAX_DEPTH];

    // Making moves.
    void clearPiece(int sq);
    void addPiece(int sq, int pce);
    void movePiece(int from, int to);
    bool makeMove(Move& move);
    void takeMove();
    Move getMove(std::string str);

    // Perft.
    u64 perft(int depth);
    void perft_suite(int depth);
    bool perft_eval_pos(int depth, const std::string& fen, const u64* correct);

    // Principal variation.
    int getPVLine(int depth);
};


#endif //ATHENE_BOARD_H
