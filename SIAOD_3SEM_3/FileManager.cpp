#include "FileManager.h"
FileManager::FileManager(int countRecord) {
	this->countRecord = countRecord;
};

int FileManager::getCountRecord() {
	return countRecord;
}

void FileManager::add(Record& record) {
	fs.open("records.bin", ios::in | ios::out | ios::binary);
	if (fs.is_open() ) {
		fs.seekp(0, std::ios::end);
		fs.write(reinterpret_cast<char*>(&record), sizeof(Record));
		++countRecord;
	}
	fs.close();
}

HashTable* FileManager::generateTable() {
	fs.open("records.bin", ios::in | ios::out | ios::binary);
	Record rec;
	HashTable* table = new HashTable(8);
	if (fs.is_open()) {
		fs.seekg(0, ios::beg);
		for (int i = 0; i < countRecord; i++) {
			fs.read(reinterpret_cast<char*>(&rec), sizeof(Record));
			table->pushRecordKey(rec.number_car,i * sizeof(Record));
		}
	}
	fs.seekg(0, ios::beg);
	fs.close();
	return table;
}

Record FileManager::find(unsigned int offset){
	fs.open("records.bin", ios::in | ios::out | ios::binary);
	Record rec;
	fs.seekg(offset, ios::beg);
	fs.read(reinterpret_cast<char*>(&rec), sizeof(Record));
	fs.close();
	return rec;
}

void FileManager::removeRecord(unsigned int offset) {
	fs.open("records.bin", ios::in | ios::out | ios::binary);
	if (fs.is_open()) {
		fs.seekg(0, std::ios::beg);
		std::fstream ofs("temp.bin", std::ios::out | std::ios::binary);
		Record rec;
		for (int i = 0; i < countRecord; i++) {
			if ((i * sizeof(Record)) != offset) {
				fs.read(reinterpret_cast<char*>(&rec), sizeof(Record));
				ofs.write(reinterpret_cast<char*>(&rec), sizeof(Record));
			}
			else {
				fs.seekg(sizeof(Record), std::ios::cur);
			}
		}
		ofs.close();
		fs.close();
		remove("records.bin");
		rename("temp.bin", "records.bin");
		--countRecord;
	}
	fs.close();
}
FileManager::~FileManager() {
	fs.close();
}
