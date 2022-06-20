//
// Created by Alex Gisi on 6/19/22.
//

#ifndef BLUE_PVTABLE_H
#define BLUE_PVTABLE_H

#include <optional>
#include "Defs.h"
#include "Move.h"
#include "PosKey.h"


class PVTable {
public:
    class PVEntry {
    public:
        PVEntry() = default;
        PVEntry(const PosKey& key, const Move& m) {
            posKey = key;
            move = m;
        }

        PosKey posKey;
        Move move;
    };

    explicit PVTable(int size = 0x100000 * 2);
    ~PVTable();

    PVTable(const PVTable& rhs);
    PVTable& operator=(const PVTable& rhs);

    void clear();
    void store(const PosKey& key, const Move& mv);
    std::optional<Move> probe(const PosKey& key);

private:
    PVEntry* table;
    int entries;
};


#endif //BLUE_PVTABLE_H
