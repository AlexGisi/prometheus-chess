//
// Created by Alex Gisi on 6/19/22.
//

#include <cassert>
#include <climits>
#include "PVTable.h"

/*
 * Creates a hash table with a specified size in memory.
 */
PVTable::PVTable(int size) {
    entries = ( size / (int) sizeof(PVEntry) ) - 2;  // Subtract two to avoid any out-of-bounds.
    table = new PVEntry[size / (int) sizeof(PVEntry)];

    clear();
}

PVTable::~PVTable() {
    clear();
    delete[] table;
}

void PVTable::clear() {
    for(int i=0; i < entries; i++)
        table[i] = PVEntry();
}

PVTable::PVTable(const PVTable &rhs) {
    entries = rhs.entries;
    table = new PVEntry[rhs.entries+2];
    clear();

    for(int i=0; i < entries; i++) {
        table[i] = rhs.table[i];
    }
}

PVTable& PVTable::operator=(const PVTable &rhs) {
    // This self-assignment check is considered bad design.
    if(&rhs == this)
        return *this;

    entries = rhs.entries;
    table = new PVEntry[rhs.entries+2];

    for(int i=0; i < entries; i++) {
        table[i] = rhs.table[i];
    }

    return *this;
}

/*
 * Ref: https://stackoverflow.com/a/11387425/5127535
 */
void PVTable::store(const PosKey& key, const Move& mv) {
    int idx = (int) (key.val & INT_MAX) % entries;
    assert(idx >= 0 && idx <= entries-1);

    table[idx].move = mv;
    table[idx].posKey = key;
}

/*
 * Note collisions are possible and must be handled externally.
 * Ref: https://stackoverflow.com/a/11387425/5127535
 */
std::optional<Move> PVTable::probe(const PosKey &key) {
    int idx = (int) (key.val & INT_MAX) % entries;
    assert(idx >= 0 && idx <= entries-1);

    if (table[idx].posKey == key)
        return {table[idx].move};
    else
        return {};
}
