//
// Created by Alex Gisi on 3/11/22.
//

#include <random>
#include <cmath>
#include <cassert>
#include "poskey.h"
#include "../defs.h"
#include "../board/board.h"

PosKey::PosKey() {
    val = 0;
}

PosKey::PosKey(const Board* pos) {
    int sq = 0;
    uint64_t finalKey = 0;
    int piece = EMPTY;

    // Pieces.
    for(sq = 0; sq < BRD_SQ_NUM; ++sq) {
        piece = pos->pieces[sq];
        if(piece != NO_SQ && piece != EMPTY && piece != OFFBOARD) {
            assert(piece >= wP && piece <= bK);
            finalKey ^= pieceKeys[piece][sq];
        }
    }

    if(pos->side == WHITE)
        finalKey ^= sideKey;

    if(pos->enPas != NO_SQ) {
        assert(pos->enPas >= 0 && pos->enPas < BRD_SQ_NUM);
        finalKey ^= pieceKeys[EMPTY][pos->enPas];
    }

    assert(pos->castlePerm >= 0 && pos->castlePerm <= 15);

    finalKey ^= castleKeys[pos->castlePerm];

    val = finalKey;
}

bool PosKey::operator==(const PosKey& rhs) const {
    return val == rhs.val;
}

bool PosKey::operator!=(const PosKey& rhs) const {
    return val != rhs.val;
}

// Via https://stackoverflow.com/a/21096340/5127535.
uint64_t PosKey::rand_64() {
    std::random_device rd;
    std::mt19937_64 e2(rd());

    std::uniform_int_distribution<long long int> dist(std::llround(std::pow(2, 61)), std::llround(std::pow(2, 62)));
    return dist(e2);
}

void PosKey::init_hash_keys() {
    int idx = 0;
    int idx2 = 0;
    for(idx = 0; idx < 13; ++idx) {
        for(idx2 = 0; idx2 < 120; ++idx2)
            pieceKeys[idx][idx2] = rand_64();
    }
    sideKey = rand_64();
    for(idx = 0; idx < 16; ++idx)
        castleKeys[idx] = rand_64();
}

PosKey::PosKey(const PosKey &rhs) {
    val = rhs.val;
}

void PosKey::hash_piece(const int pce, const int sq) {
    val ^= pieceKeys[pce][sq];
}

void PosKey::hash_castle(int castlePerm) {
    val ^= castleKeys[castlePerm];
}

void PosKey::hash_side() {
    val ^= sideKey;
}

void PosKey::hash_ep(int enPas) {
    val ^= pieceKeys[EMPTY][enPas];
}

PosKey &PosKey::operator=(const PosKey &rhs) = default;

uint64_t PosKey::pieceKeys[13][120];
uint64_t PosKey::sideKey;
uint64_t PosKey::castleKeys[16];
