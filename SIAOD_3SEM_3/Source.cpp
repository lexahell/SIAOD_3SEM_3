#include <iostream>
#include <vector>
#include <ctime>
#include <set>
#include <algorithm>
#include <random>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <chrono>
#include <forward_list>
#include "Record.h"
#include "HashTable.h"
#include "FileManager.h"

using namespace std;

int GenerateTxtFile(unsigned int array_size, bool randomOrder) {
    const std::vector<std::string> ARR_FIRST_NAMES = { "����", "�������", "�����", "�������", "�����", "���", "�����", "������", "������", "������", "�������", "�������", "����", "����", "��������", "�������", "����" };
    const std::vector<std::string> ARR_OF_LAST_NAMES = { "��������", "���������", "���������", "��������", "������", "�������", "������", "�������", "�����", "�������", "�������", "������", "�������", "������", "���������" };
    const std::vector<std::string> ARR_OF_PATRONOMYCS = { "���������", "��������", "�������", "�����", "��������", "���������", "��������", "��������", "��������", "�������", "���������", "��������", "���������" };
    const std::vector<std::string> ARR_OF_CAR_BRANDS = { "BMW", "CHEVROLET", "DODGE", "FERRARI", "LAMBORGINI", "NISSAN", "KIA", "MARK", "MERCEDES", "MAZDA", "FIAT", "KAMAZ", "LIAZ" };

    srand(static_cast<unsigned int>(time(0)));
    set<unsigned int> unique_numbers; // ��� ��� �������� ���������� �����
    int min_number = 1000000;
    int max_number = 9999999;
    // ���������, ��� ������ ������� �� ������ ��������� ��������� ���������� �����
    if (array_size > max_number - min_number + 1) {
        std::cerr << "���������� ������������� ������ ��������� ������� � ����������� �������." << std::endl;
        return 1;
    }
    // ���������� ���������� ����� � ��������� �� � ���
    while (unique_numbers.size() < array_size) {
        int random_number = min_number + (int)((double)rand() / RAND_MAX * (max_number - min_number + 1));
        unique_numbers.insert(random_number);
    }
    // ������������ ��� � ������
    vector<int> random_numbers = vector<int>(array_size);
    int i = 0;
    for (int num : unique_numbers) {
        random_numbers[i] = num;
        i++;
    }
    // ������������ �������� �������, ���� ���������
    if (randomOrder) {
        shuffle(begin(random_numbers), end(random_numbers), default_random_engine{});
    }

    // ������ � ����
    ofstream out("cars.txt");
    if (out.is_open()) {
        for (int i = 0; i < array_size; i++) {
            out << random_numbers[i] << " ";
            out << ARR_OF_CAR_BRANDS[rand() % ARR_OF_CAR_BRANDS.size()] << " ";
            out << ARR_OF_LAST_NAMES[rand() % ARR_OF_LAST_NAMES.size()] << " ";
            out << ARR_FIRST_NAMES[rand() % ARR_FIRST_NAMES.size()] << " ";
            out << ARR_OF_PATRONOMYCS[rand() % ARR_OF_PATRONOMYCS.size()] << " ";
            out << rand() % 2 << endl;
        }
    }
    out.close();
}
int GenerateBinFile(unsigned int size) {
    ifstream fin("cars.txt");
    ofstream binfout("records.bin", ios::trunc | ios::out | ios::binary);
    if (fin.is_open() && binfout.is_open()) {
        Record record;
        for (int i = 0; i < size; i++) {
            fin >> record.number_car;
            fin >> record.carBrand;
            fin >> record.lastName;
            fin >> record.firstName;
            fin >> record.patronymic;
            fin >> record.isStolen;
            binfout.write((char*)&record, sizeof(record));
        }
    }
    fin.close();
    binfout.close();
    return 0;
}
/*
string getRecordByPos(int offset)
{
    Record record;
    fs.seekg(offset, ios::beg);
    fs.read(reinterpret_cast<char*>(&record), sizeof(Record));
    return to_string(record.number_car) + " " + record.carBrand + " " + record.lastName + " " + record.firstName + " " + record.patronymic + " " + to_string(record.isStolen);
}
*/

int main() {
    setlocale(0, "RUS");
    unsigned int size = 0;
    unsigned short choice = 0;
    int start;
    int end;
    while (true) {
        cout << "������� ����� �������:" << endl;
        cout << "1 - ����� ����������� ��������� �����" << endl;
        cout << "2 - ����� ����������� ��� �������" << endl;
        cout << "3 - �������� ������� � ��� �������" << endl;
        cout << "4 - ������� ������� �� ���-�������" << endl;
        cout << "5 - ����� �������� �� �����" << endl;
        cout << "6 - ������������� �������� ���� � ��������� ���-�������" << endl;
        cout << "7 - ������� ������ �� ����� �� �����" << endl;
        cout << "8 - ����� ������ � ����� �� �����" << endl;
        cout << "9 - �������� ������� � ���-������� � �������� ����" << endl;
        cout << "0 - ���������� ���������" << endl;

        bool found = false;
        HashTable table(8);
        RecordKey rec = RecordKey(0, 0 * sizeof(Record));
        RecordKey rec1 = RecordKey(1, 1 * sizeof(Record));
        RecordKey rec2 = RecordKey(8, 2 * sizeof(Record));
        RecordKey rec3 = RecordKey(2, 3 * sizeof(Record));
        RecordKey rec4 = RecordKey(4, 4 * sizeof(Record));
        RecordKey rec5 = RecordKey(3, 5 * sizeof(Record));
        RecordKey rec6 = RecordKey(300, 6 * sizeof(Record));
        table.pushRecordKey(rec.key, rec.offset);
        table.pushRecordKey(rec1.key, rec1.offset);
        table.pushRecordKey(rec2.key, rec2.offset);
        table.pushRecordKey(rec3.key, rec3.offset);
        table.pushRecordKey(rec4.key, rec4.offset);
        table.pushRecordKey(rec5.key, rec6.offset);
        FileManager* p_file = nullptr;
        while (true) {
            cin >> choice;
            switch (choice) {
            case 1: {
                ifstream fin("records.bin", ios::binary | ios::in);
                fin.seekg(0, ios::beg);
                cout << "���������� ��������� �����" << endl;
                for (int i = 0; i < size; i++) {
                    Record record;
                    fin.read(reinterpret_cast<char*>(&record), sizeof(Record));
                    cout << record.number_car
                        << " " << record.carBrand
                        << " " << record.lastName
                        << " " << record.firstName
                        << " " << record.patronymic
                        << " " << record.isStolen << endl;
                }
                fin.close();
                break;
            }
            case 2: {
                table.print();
                break;
            }
            case 3: {
                unsigned int key;
                int offset;
                cout << "������� ���� � ����� ������ ����� ������: ";
                cin >> key >> offset;
                table.pushRecordKey(key, offset);
                break;
            }
            case 4: {
                unsigned int key;
                cout << "������� ���� ��� �������� ��������: ";
                cin >> key;
                table.deleteRecord(key);
                break;
            }
            case 5: {
                unsigned int key;
                cout << "������� ���� ��� ������ ��������: ";
                cin >> key;
                cout << "����: " << key << " ��������: " << table.findRecord(key) << endl;
                break;
            }
            case 6: {
                cout << "������� ���������� �������: ";
                cin >> size;
                GenerateTxtFile(size, true);
                GenerateBinFile(size);
                p_file = new FileManager(size); 
                table = *(p_file->generateTable());
                break;
            }
            case 7:{
                unsigned int key;
                cout << "������� ����: ";
                cin >> key;
                int offset = table.deleteRecord(key);
                p_file->removeRecord(offset);
                size--;
                break;
            }
            case 8:{
                unsigned int key;
                cout << "������� ����: ";
                cin >> key;
                int offset = table.findRecord(key);
                Record record;
                record = p_file->find(offset);
                cout << to_string(record.number_car) + " " + record.carBrand + " " + record.lastName + " " + record.firstName + " " + record.patronymic + " " + to_string(record.isStolen);
                break;
            }
            case 9: {
                // TODO �������� ����� ����� ����������
                unsigned int number_car = 0;
                Record record;
                char carBrand[24];
                char lastName[24];
                char firstName[24];
                char patronymic[24];
                bool isStolen = false;
                cout << "������� ����� ������, �����, �������, ���, ��������, ���������� � �����(0 ��� 1) ����� ������" << endl;
                cin >> record.number_car >> record.carBrand >> record.lastName >> record.firstName >> record.patronymic >> record.isStolen;
                unsigned int key = record.number_car;
                int offset = size * sizeof(Record);
                table.pushRecordKey(key, offset);
                p_file->add(record);
                size++;
                break;
            }
            case 0:
                delete p_file;
                return 0;
            default:
                break;
            }
        }
    }
    return 0;
}