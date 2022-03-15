//
// Created by Alex Gisi on 2/17/22.
//

#ifndef BLUE_BITBOARD_H
#define BLUE_BITBOARD_H

#include <cstdint>


class BitBoard {
public:
    BitBoard();
    BitBoard(const BitBoard& rhs);

    BitBoard& operator=(const BitBoard& rhs);
    BitBoard& operator=(uint64_t rhs);

    bool operator==(const BitBoard& rhs) const;
    bool operator!=(const BitBoard& rhs) const;
    explicit operator bool() const;

    void print() const;
    int popBit();
    int countBits() const;

    void clearBit(int sq);
    void setBit(int sq);

    static uint64_t setMask[64];
    static uint64_t clearMask[64];
    static void initBitMasks();

private:
    uint64_t val;
};


#endif //BLUE_BITBOARD_H
