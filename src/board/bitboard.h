//
// Created by Alex Gisi on 2/17/22.
//

#ifndef ATHENE_BITBOARD_H
#define ATHENE_BITBOARD_H

#include <cstdint>
#include <cassert>

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
    int pop();
    [[nodiscard]] int count() const;

    inline void clear_bit(int sq) {
        assert(sq >= 0);
        assert(sq <= 63);
        val &= ~(1ULL << sq);
    }

    inline void set_bit(int sq) {
        assert(sq >= 0);
        assert(sq <= 63);
        val |= (1ULL << sq);
    }

private:
    uint64_t val;
};


#endif //ATHENE_BITBOARD_H
