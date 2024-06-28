//
// Created by Alex Gisi on 6/19/22.
//

#ifndef ATHENE_PVTABLE_H
#define ATHENE_PVTABLE_H

#include <optional>
#include <memory>
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

    // Default size of 2 MB.
    explicit PVTable(int size = 0x100000 * 64);

    PVTable(const PVTable& rhs);
    PVTable& operator=(const PVTable& rhs);

    void clear();
    void store(const PosKey& key, const Move& mv);
    std::optional<Move> probe(const PosKey& key);

    PVTable resize(int new_size);

private:
    std::unique_ptr<PVEntry[]> table;
    int entries;
};


#endif //ATHENE_PVTABLE_H
