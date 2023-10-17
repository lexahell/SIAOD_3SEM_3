#include "Record.h"
bool RecordKey::operator==(const RecordKey& other) const {
    return (key == other.key) && (offset == other.offset);
}

RecordKey::RecordKey(unsigned int key, int offset) {
    this->key = key;
    this->offset = offset;
}
RecordKey::RecordKey() {
    this->key = 0;
    this->offset = -1;
}