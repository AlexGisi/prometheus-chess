//
// Created by Alex Gisi on 3/11/22.
// Encapsulates position hashing. For detailed explanation see
// https://www.youtube.com/watch?v=WqVwQBXLwE0&list=PLZ1QII7yudbc-Ky058TEaOstZHVbT-2hg&index=12
//

#ifndef BLUE_POSKEY_H
#define BLUE_POSKEY_H


#include <cstdint>

class Board;

class PosKey {
public:
    PosKey();
    explicit PosKey(const Board* pos);
    PosKey(const PosKey& rhs);

    PosKey& operator=(const PosKey& rhs);
    bool operator==(const PosKey& rhs) const;

    static void initHashKeys();

    // In-place updates to the key.
    void hashPce(int pce, int sq);
    void hashCa(int castlePerm);
    void hash_side();
    void hash_ep(int enPas);

    // Keys for possible states, random number for each state.
    static uint64_t pieceKeys[13][120];
    static uint64_t sideKey;
    static uint64_t castleKeys[16];

    uint64_t val;

private:
    static uint64_t rand_64();
};


#endif //BLUE_POSKEY_H
