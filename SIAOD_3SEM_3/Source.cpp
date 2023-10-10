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


using namespace std;
int hashFunction(unsigned int key, int size) {
    return (key % size);
}

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
    RecordKey(unsigned int key, int offset) {
        this->key = key;
        this->offset = offset;
    }
    RecordKey() {
        this->key = 0;
        this->offset = -1;
    }
    bool operator==(const RecordKey& other) const {
        return (key == other.key) && (offset == other.offset);
    }
};
struct HashTable {
    int size;
    int countRecords;
    forward_list<RecordKey>* p_array;

    HashTable(int size) {
        this->size = size;
        this->countRecords = 0;
        this->p_array = new forward_list<RecordKey>[size];
    }
    ~HashTable() {
        delete[] p_array;
    }
    void rehash() {
        forward_list<RecordKey>* currentTable = new forward_list<RecordKey>[size * 2];
        for (int i = 0; i < size; i++) {
            for (RecordKey j : p_array[i]) {
                int index = hashFunction(j.key, size * 2);
                currentTable[index].push_front(RecordKey(j.key, j.offset));
            }
        }
        delete[] p_array;
        size = size * 2;
        p_array = currentTable;
    };
    void pushRecordKey(int key, int offset) {
        int index = hashFunction(key, size);
        p_array[index].push_front(RecordKey(key, offset));
        countRecords++;
        if (((double)countRecords / size) > 0.75) {
            this->rehash();
        }
    }
    int deleteRecord(unsigned int key) {
        int index = hashFunction(key, size);
        for (RecordKey record : p_array[index]) {
            if (record.key == key) {
                countRecords--;
                int offset = record.offset;
                p_array[index].remove(record);
                return offset;
            }
        }
        return -1;
    }
    void changeOffsetByKey(unsigned int key) {
        int index = hashFunction(key, size);
        for (RecordKey record : p_array[index]) {
            if (record.key == key) {
                record.offset -= sizeof(Record);
                return;
            }
        }
    }
    int findRecord(unsigned int key) {
        int index = hashFunction(key, size);
        for (RecordKey record : p_array[index]) {
            if (record.key == key) {
                return record.offset;
            }
        }
        return -1;
    }
    void print() {
        cout << "Размер таблицы: " << size << endl;
        for (int i = 0; i < size; i++) {
            for (RecordKey record : p_array[i]) {
                cout << "индекс элемента, в котором хранится запись: " + to_string(i) + " ключ: " + to_string(record.key) + " смещение: " + to_string(record.offset) << endl;
            }
        }
    }


};
int GenerateTxtFile(unsigned int array_size, bool randomOrder) {
    const std::vector<std::string> ARR_FIRST_NAMES = { "Иван", "Василий", "Семен", "Арсений", "Артем", "Лев", "Макар", "Сергей", "Михаил", "Максим", "Евгений", "Николай", "Юрий", "Илья", "Аристарх", "Виталий", "Егор" };
    const std::vector<std::string> ARR_OF_LAST_NAMES = { "Высоцкий", "Пулеметов", "Мельников", "Кузнцеов", "Чупков", "Малютин", "Волков", "Морозов", "Рылов", "Минаков", "Головин", "Чернов", "Смирнов", "Карпов", "Саламатин" };
    const std::vector<std::string> ARR_OF_PATRONOMYCS = { "Артемович", "Иванович", "Львович", "Ильич", "Семнович", "Макарович", "Олегович", "Павлович", "Игоревич", "Юрьевич", "Дмитрович", "Егорович", "Сергеевич" };
    const std::vector<std::string> ARR_OF_CAR_BRANDS = { "BMW", "CHEVROLET", "DODGE", "FERRARI", "LAMBORGINI", "NISSAN", "KIA", "MARK", "MERCEDES", "MAZDA", "FIAT", "KAMAZ", "LIAZ" };

    srand(static_cast<unsigned int>(time(0)));
    set<unsigned int> unique_numbers; // Сет для хранения уникальных чисел
    int min_number = 1000000;
    int max_number = 9999999;
    // Проверяем, что размер массива не больше диапазона возможных уникальных чисел
    if (array_size > max_number - min_number + 1) {
        std::cerr << "Невозможно сгенерировать массив заданного размера с уникальными числами." << std::endl;
        return 1;
    }
    // Генерируем уникальные числа и добавляем их в сет
    while (unique_numbers.size() < array_size) {
        int random_number = min_number + (int)((double)rand() / RAND_MAX * (max_number - min_number + 1));
        unique_numbers.insert(random_number);
    }
    // Конвертируем сет в массив
    vector<int> random_numbers = vector<int>(array_size);
    int i = 0;
    for (int num : unique_numbers) {
        random_numbers[i] = num;
        i++;
    }
    // Перемешиваем элементы массива, если требуется
    if (randomOrder) {
        shuffle(begin(random_numbers), end(random_numbers), default_random_engine{});
    }

    // Запись в файл
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
string getRecordByPos(std::ifstream& fin)
{
    Record record;
    fin.read(reinterpret_cast<char*>(&record), sizeof(Record));
    return to_string(record.number_car) + " " + record.carBrand + " " + record.lastName + " " + record.firstName + " " + record.patronymic + " " + to_string(record.isStolen);
}

int main() {
    setlocale(0, "RUS");
    unsigned int size = 0;
    unsigned int number_car_search = 0;
    unsigned int number_car = 0;
    unsigned short choice = 0;
    char carBrand[24];
    char lastName[24];
    char firstName[24];
    char patronymic[24];
    bool isStolen = false;
    int start;
    int end;
    while (true) {
        /*
        cout << "Введите размер массива: ";
        if (!(cin >> size)) {
            cout << "Ошибка ввода";
        }
        GenerateTxtFile(size, true);
        GenerateBinFile(size);
        */
        cout << "Введите номер команды:" << endl;
        cout << "1 - вывод содержимого бинарного файла" << endl;
        cout << "2 - вывод содержимого хеш таблицы" << endl;
        cout << "3 - добавить элемент в хеш таблицу" << endl;
        cout << "4 - удалить элемент из хеш-таблицы" << endl;
        cout << "5 - поиск элемента по ключу" << endl;
        cout << "0 - завершение программы" << endl;

        ifstream fin("records.bin", ios::binary | ios::in);
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
        while (true) {
            cin >> choice;
            switch (choice) {
            case 1:
                cout << "Содержимое бинарного файла" << endl;
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
                break;
            case 2: {
                table.print();
                break;
            }
            case 3: {
                unsigned int key;
                int offset;
                cout << "Введите ключ и номер записи через пробел: ";
                cin >> key >> offset;
                table.pushRecordKey(key, offset);
                break;
            }
            case 4: {
                unsigned int key;
                cout << "Введите ключ для удаления элемента: ";
                cin >> key;
                table.deleteRecord(key);
                break;
            }
            case 5: {
                unsigned int key;
                cout << "Введите ключ для поиска элемента: ";
                cin >> key;
                cout << "Ключ: " << key << " Смещение: " << table.findRecord(key) << endl;
                break;
            }
            case 0:
                return 0;
            default:
                break;
            }
        }
        fin.close();
    }
    return 0;
}