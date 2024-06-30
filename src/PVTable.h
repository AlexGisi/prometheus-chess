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


enum class EntryFlag { HFNONE, HFALPHA, HFBETA, HFEXACT };

class PVTable {
public:
    class PVEntry {
    public:
        // If initialized with default constructor must set full to true
        // before storage.
        PVEntry() : age(0), flag(EntryFlag::HFNONE) {};
        PVEntry(const PosKey& key,
                const SearchMove &m,
                int p_depth,
                EntryFlag p_flag) {
            posKey = key;
            move = m;
            depth = p_depth;
            age = 0; // Must be set before storing.
            flag = p_flag;
            full = true;
        }

        PosKey posKey;
        SearchMove move;
        int depth = 0;
        int age;
        EntryFlag flag = EntryFlag::HFNONE;
        bool full = false;
    };

    // Default size of 64 MB.
    explicit PVTable(int size = 0x100000 * 64);

    PVTable(const PVTable& rhs);
    PVTable& operator=(const PVTable& rhs);

    void clear();
    void store(const PosKey& key, PVEntry value);
    std::optional<PVEntry> probe(const PosKey& key);
    std::optional<SearchMove> probe_move(const PosKey& key, int alpha, int beta, int depth);

    PVTable resize(int new_size);
    void prep_search();

private:
    [[nodiscard]] int hash(const PosKey& key) const;

    std::unique_ptr<PVEntry[]> table;
    int entries;
    int entries_full = 0;

    int new_write = 0;
    int overwrite = 0;
    int hit = 0;
    int cut = 0;
    int current_age = 0;
};


#endif //ATHENE_PVTABLE_H
