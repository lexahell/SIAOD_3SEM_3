#pragma once
#ifndef HASHTABLE
#define HASHTABLE

#include "Record.h"
#include <forward_list>
#include <iostream>
struct HashTable{
    int size;
    int countRecords;
    std::forward_list<RecordKey>* p_array;
    int hashFunction(unsigned int key, int size);
    HashTable(int size);
    ~HashTable();
    void rehash();
    void pushRecordKey(int key, int offset);
    int deleteRecord(unsigned int key);
    void changeOffsetByKey(unsigned int key);
    int findRecord(unsigned int key);
    void print();
};


#endif