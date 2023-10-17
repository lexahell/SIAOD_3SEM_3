#pragma once
#ifndef FILEMANAGER
#define FILEMANAGER
#include <fstream>
#include <string>
#include "Record.h"
#include "HashTable.h"

using namespace std;

class FileManager {
	fstream fs;
	int countRecord;
public:
    FileManager(int countRecord);
    int getCountRecord();
    void add(Record&);
    HashTable* generateTable();
    Record find(unsigned int);
    void removeRecord(unsigned int);
    ~FileManager();
};


#endif