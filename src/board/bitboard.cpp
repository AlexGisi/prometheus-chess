//
// Created by Alex Gisi on 2/17/22.
//
// Important: bitboards refer to squares with a 64-square representation, not 120
// like most of the program.
//

#include <iostream>
#include "bitboard.h"
#include "board.h"

BitBoard::BitBoard() {
    val = 0ULL;
}

BitBoard::BitBoard(const BitBoard &rhs) {
    val = rhs.val;
}

BitBoard& BitBoard::operator=(const BitBoard &rhs) = default;

BitBoard& BitBoard::operator=(uint64_t rhs) {
    val = rhs;
    return *this;
}

void BitBoard::print() const {
    uint64_t shifted = 1ULL;

    int rank = 0;
    int file = 0;
    int sq = 0;
    int sq64 = 0;

    std::cout << std::endl;
    for(rank = RANK_8; rank >= RANK_1; --rank) {
        for(file = FILE_A; file <= FILE_H; ++file) {
            sq = FR2SQ(file, rank);
            sq64 = Board::sq120ToSq64[sq];

            if((shifted << sq64) & val)
                std::cout << 'X';
            else
                std::cout << '-';
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

/*
 * Remove the least significant '1' in the bitboard. Return the index.
 */
int BitBoard::pop() {
    uint64_t b = val ^ (val-1);
    auto fold = (unsigned) ((b & 0xffffffff) ^ (b >> 32));
    val &= (val-1);
    return BitTable[(fold * 0x783a9b23) >> 26];
}

int BitBoard::count() const {
    uint64_t b = val;
    int r = 0;
    for( ; b; r++, b &= b-1);
    return r;
}

bool BitBoard::operator==(const BitBoard &rhs) const {
    return val == rhs.val;
}

bool BitBoard::operator!=(const BitBoard &rhs) const {
    return val != rhs.val;
}

BitBoard::operator bool() const {
    return val != 0;
}
