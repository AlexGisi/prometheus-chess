//
// Created by Alex Gisi on 6/19/22.
//

#ifndef ATHENE_PVTABLE_H
#define ATHENE_PVTABLE_H

#include <optional>
#include <memory>
#include "../defs.h"
#include "../move/move.h"
#include "poskey.h"


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
                uint8_t p_depth,
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
        uint8_t depth = 0;
        uint8_t age;
        EntryFlag flag = EntryFlag::HFNONE;
        bool full = false;
    };

    // Default size of 64 MB.
    explicit PVTable(size_t size = 0x100000 * 64);

    PVTable(const PVTable& rhs);
    PVTable& operator=(const PVTable& rhs);

    void clear();
    void store(const PosKey& key, PVEntry value);
    std::optional<PVEntry> probe(const PosKey& key);
    std::optional<SearchMove> probe_move(const PosKey& key, int alpha, int beta, int depth);

    PVTable resize(size_t new_size);
    void prep_search();

private:
    [[nodiscard]] uint64_t hash(const PosKey& key) const;

    std::unique_ptr<PVEntry[]> table;
    uint64_t entries;
    uint64_t entries_full = 0;

    uint64_t new_write = 0;
    uint64_t overwrite = 0;
    uint64_t hit = 0;
    uint64_t cut = 0;
    uint8_t current_age = 0;
};


#endif //ATHENE_PVTABLE_H
