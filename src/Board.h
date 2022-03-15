//
// Created by Alex Gisi on 2/17/22.
//

#ifndef BLUE_BOARD_H
#define BLUE_BOARD_H

#include <string>
#include "BitBoard.h"
#include "Defs.h"
#include "PosKey.h"

class Board {
public:
    Board();
    explicit Board(const std::string& fen);

    Board(const Board& rhs);
    Board& operator=(const Board& rhs);

    void setUpEmpty();

    static int sq120ToSq64[BRD_SQ_NUM];
    static int sq64ToSq120[64];
    static int filesBrd[BRD_SQ_NUM];
    static int ranksBrd[BRD_SQ_NUM];

    static void initSq120To64();
    static void initFilesRanksBrd();

    void reset();
    void updateListsMaterial();
    bool checkBoard() const;

    bool sqAttacked(int sq, int att_side) const;
    void showSqAttBySide(int side) const;

    void print();

private:
    int pieces[BRD_SQ_NUM];
    BitBoard pawns[3];  // WHITE, BLACK, BOTH.

    int kingSq[2];

    int side;
    int enPas;
    int fiftyMove;

    int ply;
    int hisPlay;  // History.

    PosKey posKey;

    int pceNum[13];  // Number of pieces of each type.
    int bigPce[2];  // Number of 'big pieces', i.e. not pawns.
    int majPce[2];  // Number of 'major pieces', i.e. rooks and queens.
    int minPce[2];  // Number of 'minor pieces', bishops and knights.
    int material[2];  // Sum of material values for white and black's pieces.

    int castlePerm;  // All castle permissions represented by four bits.

    typedef struct {
        // Move about to be played.
        int move;

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

    friend class PosKey;
};


#endif //BLUE_BOARD_H
