//
// Created by Alex Gisi on 6/19/22.
//

#include <cassert>
#include <climits>
#include "pvtable.h"

/*
 * Creates a hash table with a specified size (in bytes) in memory.
 */
PVTable::PVTable(int size) {
    entries = ( size / (int) sizeof(PVEntry) ) - 2;  // Subtract two to avoid any out-of-bounds.
    table = std::make_unique<PVEntry[]>(size / (int) sizeof(PVEntry));
}

void PVTable::clear() {
    for(int i=0; i < entries; i++) {
        table[i] = PVEntry();
    }
    new_write = 0;
    entries_full = 0;
    overwrite = 0;
    hit = 0;
    cut = 0;
    current_age = 0;
}

void PVTable::prep_search() {
    current_age++;
    new_write = 0;
    overwrite = 0;
    hit = 0;
    cut = 0;
}

PVTable::PVTable(const PVTable &rhs) {
    entries = rhs.entries;
    table = std::make_unique<PVEntry[]>(rhs.entries);
    entries_full = rhs.entries_full;
    new_write = rhs.new_write;
    overwrite = rhs.overwrite;
    hit = rhs.hit;
    cut = rhs.cut;
}

PVTable& PVTable::operator=(const PVTable &rhs) {
    entries = rhs.entries;
    table = std::make_unique<PVEntry[]>(rhs.entries);
    entries_full = rhs.entries_full;
    new_write = rhs.new_write;
    overwrite = rhs.overwrite;
    hit = rhs.hit;
    cut = rhs.cut;

    return *this;
}

/*
 * Ref: https://stackoverflow.com/a/11387425/5127535
 */
void PVTable::store(const PosKey& key, PVEntry value) {
    int idx = hash(key);
    assert(idx >= 0 && idx <= entries-1);
    assert(value.full == true);
    assert(value.move.move.move != 0);

    bool do_store = false;
    if (table[idx].full) {
        if (table[idx].age < current_age || table[idx].depth < value.depth) {
            overwrite++;
            do_store = true;
        }
    } else {
        new_write++;
        entries_full++;
        do_store = true;
    }

    if (do_store) {
        value.age = current_age;
        table[idx] = value;
    }
}

/*
 * Note collisions are possible and must be handled externally.
 * Ref: https://stackoverflow.com/a/11387425/5127535
 */
std::optional<PVTable::PVEntry> PVTable::probe(const PosKey &key) {
    int idx = hash(key);
    assert(idx >= 0 && idx <= entries-1);

    if (table[idx].full && table[idx].posKey == key)
        return table[idx];
    else
        return {};
}

PVTable PVTable::resize(int new_size) {
    PVTable new_table(new_size);
    for (int i=0; i < entries; i++) {
        if (table[i].full) {
            new_table.store(table[i].posKey, table[i]);
        }
    }
    return new_table;
}

// Ref: https://github.com/bluefeversoft/vice/blob/main/Vice11/src/pvtable.c#L69
std::optional<SearchMove> PVTable::probe_move(const PosKey& key, int alpha, int beta, int depth) {
    Move move;
    int score;
    int idx = hash(key);

    assert(idx >= 0 && idx <= entries-1);
    assert(depth >= 1 && depth <= MAX_DEPTH);
    assert(alpha < beta);
    assert(alpha >= -INFINITE && alpha <= INFINITE);
    assert(beta >= -INFINITE && beta <= INFINITE);

    if (table[idx].posKey != key || table[idx].depth < depth) {
        return {};
    }

    PVEntry res = table[idx];
    score = res.move.score;
    move = table[idx].move.move;

    hit++;

    assert(res.depth >= 1 && res.depth <= MAX_DEPTH);
    assert(res.flag != EntryFlag::HFNONE);

    switch (res.flag) {
        case EntryFlag::HFALPHA:
            if (res.move.score <= alpha) {
                score = alpha;
            }
            break;
        case EntryFlag::HFBETA:
            if (res.move.score >= beta) {
                score = beta;
            }
            break;
        case EntryFlag::HFEXACT:
            break;
        default:
            assert(false);
    }

    cut++;
    return {{move, score}};
}

int PVTable::hash(const PosKey &key) const {
    return (int) (key.val & INT_MAX) % entries;
}
