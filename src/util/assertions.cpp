//
// Created by Alex Gisi on 3/24/22.
//

#include "../defs.h"

#ifndef ATHENE_ASSERTIONS_CPP
#define ATHENE_ASSERTIONS_CPP

inline bool sideValid(const int side) { return side == WHITE || side == BLACK; }

inline bool fileRankValid(const int fr) { return fr >= 0 && fr <= 7; }

inline bool pieceValidEmpty(const int piece) { return piece >= EMPTY && piece <= bK; }

inline bool pieceValid(const int piece) { return piece >= wP && piece <= bK; }

#endif
