//
// Created by Alex Gisi on 3/24/22.
//

#include "Defs.h"
#include "Board.h"

inline bool sqOnBoard(const int sq) { return Board::filesBrd[sq] != OFFBOARD; }

inline bool sqOffBoard(const int sq) { return Board::filesBrd[sq] == OFFBOARD; }

inline bool sideValid(const int side) { return side == WHITE || side == BLACK; }

inline bool fileRankValid(const int fr) { return fr >= 0 && fr <= 7; }

inline bool pieceValidEmpty(const int piece) { return piece >= EMPTY && piece <= bK; }

inline bool pieceValid(const int piece) { return piece >= wP && piece <= bK; }
