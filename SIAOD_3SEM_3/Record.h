#pragma once
#ifndef RECORD
#define RECORD
#include <iostream>
#include <fstream>
struct Record {
    unsigned int number_car;
    char carBrand[24] = { 0 };
    char lastName[24] = { 0 };
    char firstName[24] = { 0 };
    char patronymic[24] = { 0 };
    bool isStolen = false;
};
struct RecordKey {
    unsigned int key;
    int offset;
    RecordKey(unsigned int key, int offset);
    RecordKey();
    bool operator==(const RecordKey& other) const;
};
#endif