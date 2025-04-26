#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>


using namespace std;

struct Animals {
    string klichka;
    string vid;
    string poroda;
    int age;
    string fio;
};

vector<Animals> LoadAnimals(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Файл не существует или не может быть открыт для чтения. Будет создан новый файл при сохранении." << endl;
        return {};
    }

    vector<Animals> animals;
    int count;
    file >> count;
    file.ignore();

    for (int i = 0; i < count; i++) {
        Animals animal;
        getline(file, animal.klichka);
        getline(file, animal.vid);
        getline(file, animal.poroda);
        file >> animal.age;
        file.ignore();
        getline(file, animal.fio);
        animals.push_back(animal);
    }

    file.close();
    cout << "Данные успешно загружены из файла! Загружено животных: " << animals.size() << endl;
    return animals;
}

void SaveAnimals(const vector<Animals>& animals, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка открытия файла для записи!" << endl;
        return;
    }
 
    file << animals.size() << endl;
    for (const auto& animal : animals) {
        file << animal.klichka << endl;
        file << animal.vid << endl;
        file << animal.poroda << endl;
        file << animal.age << endl;
        file << animal.fio << endl;
    }

    file.close();
    cout << "Данные успешно сохранены в файл! Сохранено животных: " << animals.size() << endl;
}

string toUpperCase(const string& str) {
    string result = str;
    for (char& c : result) {
        c = toupper(static_cast<unsigned char>(c));
    }
    return result;
}

void addAnimal(vector<Animals>& animals, const string& filename) {
    Animals newAnimal;

    cout << "\nВведите кличку животного: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, newAnimal.klichka);
    newAnimal.klichka = toUpperCase(newAnimal.klichka);

    cout << "Введите вид животного: ";
    getline(cin, newAnimal.vid);
    newAnimal.vid = toUpperCase(newAnimal.vid);

    cout << "Введите породу животного: ";
    getline(cin, newAnimal.poroda);
    newAnimal.poroda = toUpperCase(newAnimal.poroda);

    cout << "Введите возраст животного в годах: ";
    while (!(cin >> newAnimal.age) || newAnimal.age <= 0 || newAnimal.age > 100) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Неверный ввод! Пожалуйста, введите число от 1 до 100: ";
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    bool Fio = false;
    while (!Fio) {
        cout << "Введите ФИО владельца (Фамилия Имя Отчество): ";
        getline(cin, newAnimal.fio);
        newAnimal.fio = toUpperCase(newAnimal.fio);

        stringstream ss(newAnimal.fio);
        string word;
        int wordCount = 0;
        while (ss >> word) {
            wordCount++;
        }

        if (wordCount == 3) {
            Fio = true;
        } else {
            cout << "Ошибка! Введите ФИО в формате 'Фамилия Имя Отчество' (ровно 3 слова)." << endl;
        }
    }

    animals.push_back(newAnimal);
}

void removeAnimal(vector<Animals>& animals, const string& filename) {
    size_t index;
    cout << "Введите индекс животного для удаления (1-" << animals.size() << "): ";
    cin >> index;

    if (index < 1 || index > animals.size()) {
        cout << "Неверный индекс!" << endl;
        return;
    }

    animals.erase(animals.begin() + (index - 1));
    SaveAnimals(animals, filename);
    cout << "Животное успешно удалено!" << endl;
}

void OldSort(vector<Animals>& animals) {
    size_t n = animals.size();
    for (size_t i = 0; i < n - 1; i++) {
        size_t maxIndex = i;
        for (size_t j = i + 1; j < n; j++) {
            if (animals[j].age > animals[maxIndex].age) {
                maxIndex = j;
            }
        }
        if (maxIndex != i) {
            swap(animals[i], animals[maxIndex]);
        }
    }
    cout << "Животные отсортированы по старшинству методом прямого выбора" << endl;
}

size_t partitionAsc(vector<Animals>& animals, size_t low, size_t high) {
    int pivot = animals[high].age;
    size_t i = low;

    for (size_t j = low; j < high; j++) {
        if (animals[j].age <= pivot) {
            swap(animals[i], animals[j]);
            i++;
        }
    }
    swap(animals[i], animals[high]);
    return i;
}

void quickSortAsc(vector<Animals>& animals, size_t low, size_t high) {
    if (low < high) {
        size_t pivotIndex = partitionAsc(animals, low, high);
        if (pivotIndex > 0) {
            quickSortAsc(animals, low, pivotIndex - 1);
        }
        quickSortAsc(animals, pivotIndex + 1, high);
    }
}

void LittleSort(vector<Animals>& animals) {
    if (!animals.empty()) {
        quickSortAsc(animals, 0, animals.size() - 1);
    }
    cout << "Животные отсортированы по меньшинству методом Quick Sort" << endl;
}

void AlfSort(vector<Animals>& animals) {
    size_t n = animals.size();
    for (size_t i = 0; i < n - 1; i++) {
        for (size_t j = 0; j < n - i - 1; j++) {
            if (animals[j].klichka > animals[j + 1].klichka) {
                swap(animals[j], animals[j + 1]);
            }
        }
    }
    cout << "Животные отсортированы по алфавиту!" << endl;
}

void chooseSort(vector<Animals>& animals) {
    cout << "Выберите, как будем сортировать?" << endl;
    cout << "1 - по старшинству " << endl;
    cout << "2 - по меньшинству" << endl;
    cout << "3 - по алфавиту" << endl;
    
    int k;
    cin >> k;
    
    switch(k) {
        case 1: OldSort(animals); break;
        case 2: LittleSort(animals); break;
        case 3: AlfSort(animals); break;
        default: cout << "Неверный выбор!" << endl; break;
    }
}

void editAnimal(vector<Animals>& animals, const string& filename) {
    if (animals.empty()) {
        cout << "Список животных пуст! Нечего редактировать." << endl;
        return;
    }

    cout << "\nСписок животных для редактирования:\n";
    for (size_t i = 0; i < animals.size(); i++) {
        cout << i + 1 << ". Кличка: " << animals[i].klichka
             << ", Порода: " << animals[i].poroda << endl;
    }

    size_t index;
    cout << "Введите индекс животного для редактирования (1-" << animals.size() << "): ";
    cin >> index;

    if (index < 1 || index > animals.size()) {
        cout << "Неверный индекс!" << endl;
        return;
    }

    Animals& animal = animals[index - 1];
    cout << "\nРедактируем животное: " << animal.klichka << endl;
    cout << "Текущие данные:\n";
    cout << "1. Кличка: " << animal.klichka << endl;
    cout << "2. Вид: " << animal.vid << endl;
    cout << "3. Порода: " << animal.poroda << endl;
    cout << "4. Возраст: " << animal.age << endl;
    cout << "5. ФИО владельца: " << animal.fio << endl;

    int fieldChoice;
    cout << "\nВыберите, что хотите отредактировать:\n";
    cout << "1. Кличку\n";
    cout << "2. Вид\n";
    cout << "3. Породу\n";
    cout << "4. Возраст\n";
    cout << "5. ФИО владельца\n";
    cout << "0. Отмена\n";
    cout << "Ваш выбор: ";
    cin >> fieldChoice;

    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    bool edited = false;
    switch (fieldChoice) {
        case 1: {
            cout << "Введите новую кличку животного: ";
            getline(cin, animal.klichka);
            animal.klichka = toUpperCase(animal.klichka);
            cout << "Кличка успешно обновлена!" << endl;
            edited = true;
            break;
        }
        case 2: {
            cout << "Введите новый вид животного: ";
            getline(cin, animal.vid);
            cout << "Вид успешно обновлен!" << endl;
            edited = true;
            break;
        }
        case 3: {
            cout << "Введите новую породу животного: ";
            getline(cin, animal.poroda);
            cout << "Порода успешно обновлена!" << endl;
            edited = true;
            break;
        }
        case 4: {
            cout << "Введите новый возраст животного в годах: ";
            while (!(cin >> animal.age) || animal.age <= 0 || animal.age > 100) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "Неверный ввод! Пожалуйста, введите число от 1 до 100: ";
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Возраст успешно обновлен!" << endl;
            edited = true;
            break;
        }
        case 5: {
            bool Fio = false;
            while (!Fio) {
                cout << "Введите новое ФИО владельца (Фамилия Имя Отчество): ";
                getline(cin, animal.fio);
                stringstream ss(animal.fio);
                string word;
                int wordCount = 0;
                while (ss >> word) wordCount++;
                if (wordCount == 3) Fio = true;
                else cout << "Ошибка! Введите ФИО в формате 'Фамилия Имя Отчество' (ровно 3 слова)." << endl;
            }
            cout << "ФИО владельца успешно обновлено!" << endl;
            edited = true;
            break;
        }
        case 0: {
            cout << "Редактирование отменено." << endl;
            return;
        }
        default: {
            cout << "Неверный выбор! Редактирование отменено." << endl;
            return;
        }
    }

    if (edited) {
        SaveAnimals(animals, filename);
        cout << "Изменения успешно сохранены в файл!" << endl;
    }
}

void linearSearch(const vector<Animals>& animals) {
    if (animals.empty()) {
        cout << "Список животных пуст! Нечего искать." << endl;
        return;
    }

    string searchKey;
    cout << "Введите кличку животного для поиска: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, searchKey);
    searchKey = toUpperCase(searchKey);

    bool found = false;
    for (size_t i = 0; i < animals.size(); i++) {
        if (animals[i].klichka == searchKey) {
            cout << "\nНайдено животное №" << i + 1 << ":\n";
            cout << "  Кличка: " << animals[i].klichka << "\n";
            cout << "  Вид: " << animals[i].vid << "\n";
            cout << "  Порода: " << animals[i].poroda << "\n";
            cout << "  Возраст: " << animals[i].age << " лет\n";
            cout << "  ФИО владельца: " << animals[i].fio << "\n";
            found = true;
        }
    }

    if (!found) {
        cout << "Животное с кличкой '" << searchKey << "' не найдено." << endl;
    }
}

void binarySearch(const vector<Animals>& animals) {
    if (animals.empty()) {
        cout << "Список животных пуст! Нечего искать." << endl;
        return;
    }

    bool isSorted = true;
    for (size_t i = 0; i < animals.size() - 1; i++) {
        if (animals[i].klichka > animals[i + 1].klichka) {
            isSorted = false;
            break;
        }
    }

    if (!isSorted) {
        cout << "Для двоичного поиска массив должен быть отсортирован по кличке!" << endl;
        cout << "Выполните сортировку по алфавиту (пункт 5 -> 3) и повторите попытку." << endl;
        return;
    }

    string searchKey;
    cout << "Введите кличку животного для поиска: ";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    getline(cin, searchKey);
    searchKey = toUpperCase(searchKey);

    size_t left = 0;
    size_t right = animals.size() - 1;
    bool found = false;

    while (left <= right) {
        size_t mid = left + (right - left) / 2;
        
        if (animals[mid].klichka == searchKey) {
            size_t first = mid;
            size_t last = mid;
            
            while (first > 0 && animals[first - 1].klichka == searchKey) {
                first--;
            }
            
            while (last < animals.size() - 1 && animals[last + 1].klichka == searchKey) {
                last++;
            }
            
            for (size_t i = first; i <= last; i++) {
                cout << "\nНайдено животное №" << i + 1 << ":\n";
                cout << "  Кличка: " << animals[i].klichka << "\n";
                cout << "  Вид: " << animals[i].vid << "\n";
                cout << "  Порода: " << animals[i].poroda << "\n";
                cout << "  Возраст: " << animals[i].age << " лет\n";
                cout << "  ФИО владельца: " << animals[i].fio << "\n";
            }
            
            found = true;
            break;
        }
        else if (animals[mid].klichka < searchKey) {
            left = mid + 1;
        }
        else {
            if (mid == 0) break;
            right = mid - 1;
        }
    }

    if (!found) {
        cout << "Животное с кличкой '" << searchKey << "' не найдено." << endl;
    }
}
void filterAnimalsByAge(const vector<Animals>& animals) {
    if (animals.empty()) {
        cout << "Список животных пуст! Нечего искать." << endl;
        return;
    }

    int minAge;
    cout << "Введите минимальный возраст для поиска: ";
    while (!(cin >> minAge) || minAge < 0) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Неверный ввод! Пожалуйста, введите положительное число: ";
    }

    // Собираем всех животных старше minAge
    vector<Animals> filteredAnimals;
    for (const auto& animal : animals) {
        if (animal.age >= minAge) {
            filteredAnimals.push_back(animal);
        }
    }

    if (filteredAnimals.empty()) {
        cout << "Животных старше " << minAge << " лет не найдено." << endl;
        return;
    }


    vector<Animals> sortedAnimals = filteredAnimals;
    sort(sortedAnimals.begin(), sortedAnimals.end(),
        [](const Animals& a, const Animals& b) {
            return a.vid < b.vid;
        });


    ofstream outFile("zad.txt");
    if (!outFile.is_open()) {
        cerr << "Ошибка открытия файла zad.txt для записи!" << endl;
        return;
    }

    outFile << "\n" << string(60, '=') << "\n";
    outFile << "  СПИСОК ЖИВОТНЫХ СТАРШЕ " << minAge << " ЛЕТ\n";
    outFile << "  Всего найдено: " << filteredAnimals.size() << "\n";
    outFile << string(60, '=') << "\n";

    string currentType = "";
    for (size_t i = 0; i < sortedAnimals.size(); i++) {
        if (sortedAnimals[i].vid != currentType) {
            if (!currentType.empty()) {
                outFile << string(60, '-') << "\n";
            }
            currentType = sortedAnimals[i].vid;
            outFile << "\nВИД: " << currentType << "\n";
            outFile << string(30, '-') << "\n";
        }

        outFile << "Животное №" << i + 1 << ":\n";
        outFile << "  Кличка: " << sortedAnimals[i].klichka << "\n";
        outFile << "  Порода: " << sortedAnimals[i].poroda << "\n";
        outFile << "  Возраст: " << sortedAnimals[i].age << " лет\n";
        outFile << "  ФИО владельца: " << sortedAnimals[i].fio << "\n";
        
        if (i < sortedAnimals.size() - 1 && sortedAnimals[i+1].vid == currentType) {
            outFile << string(30, '-') << "\n";
        }
    }
    outFile << string(60, '=') << "\n";
    outFile.close();

    cout << "\n" << string(60, '=') << "\n";
    cout << "  СПИСОК ЖИВОТНЫХ СТАРШЕ " << minAge << " ЛЕТ\n";
    cout << "  Всего найдено: " << filteredAnimals.size() << "\n";
    cout << "  Результат сохранен в файл zad.txt\n";
    cout << string(60, '=') << "\n";

    currentType = "";
    for (size_t i = 0; i < sortedAnimals.size(); i++) {
        if (sortedAnimals[i].vid != currentType) {
            if (!currentType.empty()) {
                cout << string(60, '-') << "\n";
            }
            currentType = sortedAnimals[i].vid;
            cout << "\nВИД: " << currentType << "\n";
            cout << string(30, '-') << "\n";
        }

        cout << "Животное №" << i + 1 << ":\n";
        cout << "  Кличка: " << sortedAnimals[i].klichka << "\n";
        cout << "  Порода: " << sortedAnimals[i].poroda << "\n";
        cout << "  Возраст: " << sortedAnimals[i].age << " лет\n";
        cout << "  ФИО владельца: " << sortedAnimals[i].fio << "\n";
        
        if (i < sortedAnimals.size() - 1 && sortedAnimals[i+1].vid == currentType) {
            cout << string(30, '-') << "\n";
        }
    }
    cout << string(60, '=') << "\n";
}


int main() {
    
    
    char cwd[1024];
       if (getcwd(cwd, sizeof(cwd))) {
           cout << "Текущая рабочая директория: " << cwd << endl;
       }
    
    setlocale(LC_ALL, "ru_RU.UTF-8");
    vector<Animals> animals;
    string filename = "animal.txt";

    animals = LoadAnimals(filename);

    if (animals.empty()) {
        animals.push_back({"Мурка", "Кот", "Сиамская", 3, "Иванов Иван Иванович"});
        animals.push_back({"Шарик", "Собака", "Лабрадор", 5, "Петров Петр Петрович"});
        animals.push_back({"Буся", "Кот", "Персидская", 2, "Сидорова Анна Сергеевна"});
        animals.push_back({"Рекс", "Собака", "Немецкая овчарка", 4, "Кузнецов Алексей Викторович"});
        animals.push_back({"Пушок", "Хомяк", "Сирийский", 1, "Морозова Ольга Николаевна"});
        animals.push_back({"Кеша", "Попугай", "Волнистый", 2, "Смирнов Дмитрий Андреевич"});
        animals.push_back({"Барон", "Собака", "Доберман", 6, "Лебедева Екатерина Павловна"});
        animals.push_back({"Тишка", "Кот", "Мейн-кун", 3, "Васильев Сергей Михайлович"});
        animals.push_back({"Чижик", "Птица", "Канарейка", 1, "Козлова Мария Александровна"});
        animals.push_back({"Гоша", "Черепаха", "Среднеазиатская", 8, "Николаев Игорь Олегович"});
        SaveAnimals(animals, filename);
    }

    int choice;
    do {
        cout << "\n" << string(40, '=') << "\n";
        cout << "|          МЕНЮ УПРАВЛЕНИЯ            |\n";
        cout << string(40, '-') << "\n";
        cout << "| 1. Добавить животного              |\n";
        cout << "| 2. Удалить животного               |\n";
        cout << "| 3. Сохранить животных в файл       |\n";
        cout << "| 4. Показать животных               |\n";
        cout << "| 5. Сортировать животных            |\n";
        cout << "| 6. Редактировать животного         |\n";
        cout << "| 7. Линейный поиск по кличке        |\n";
        cout << "| 8. Двоичный поиск по кличке        |\n";
        cout << "| 9. Фильтр по возрасту              |\n";
        cout << "| 0. Выход                           |\n";
        cout << string(40, '=') << "\n";
        cout << "Выберите действие: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Неверный ввод! Пожалуйста, введите число." << endl;
            continue;
        }

        switch (choice) {
            case 1: addAnimal(animals, filename); break;
            case 2: removeAnimal(animals, filename); break;
            case 3: SaveAnimals(animals, filename); break;
            case 4:
                if (!animals.empty()) {
                    cout << "\n" << string(50, '=') << "\n";
                    cout << "          СПИСОК ЖИВОТНЫХ\n";
                    cout << string(50, '=') << "\n";
                    for (size_t i = 0; i < animals.size(); i++) {
                        cout << "Животное №" << i + 1 << ":\n";
                        cout << "  Кличка: " << animals[i].klichka << "\n";
                        cout << "  Вид: " << animals[i].vid << "\n";
                        cout << "  Порода: " << animals[i].poroda << "\n";
                        cout << "  Возраст: " << animals[i].age << " лет\n";
                        cout << "  ФИО владельца: " << animals[i].fio << "\n";
                        if (i < animals.size() - 1) cout << string(50, '-') << "\n";
                    }
                    cout << string(50, '=') << "\n";
                } else {
                    cout << "В файле нет животных! Сначала добавьте нового животного." << endl;
                }
                break;
            case 5: chooseSort(animals); break;
            case 6: editAnimal(animals, filename); break;
            case 7: linearSearch(animals); break;
            case 8: binarySearch(animals); break;
            case 9: filterAnimalsByAge(animals); break;
            case 0: cout << "Выход из программы." << endl; break;
            default: cout << "Неверный выбор!" << endl;
                
        }
    } while (choice != 0);

    
    
    return 0;
}

