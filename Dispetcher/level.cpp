#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include "level.h"
using namespace std;

bool checker(vector<vector<int>>& table, vector<int>& resources) {
    for (int i = 0; i < table.size(); i++) {
        for (int x = 0; x < resources.size(); x++) {
            if (table[i][x] > 0 && resources[x] >= table[i][x]) {
                return true;
            }
        }
    }
    return false;
}

void save_game(const string& filename, int MAX_CAPACITY, int NUMBER_OF_RESOURCES, int NUMBER_OF_PROCESSES, int counter, const vector<int>& resources, const vector<vector<int>>& base_table) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to save game." << endl;
        return;
    }
    file << MAX_CAPACITY << " " << NUMBER_OF_RESOURCES << " " << NUMBER_OF_PROCESSES << " " << counter << endl;
    for (int res : resources) {
        file << res << " ";
    }
    file << endl;
    for (const auto& row : base_table) {
        for (int val : row) {
            file << val << " ";
        }
        file << endl;
    }
    file.close();
    cout << "Game saved successfully!" << endl;
}

bool load_game(const string& filename, int& MAX_CAPACITY, int& NUMBER_OF_RESOURCES, int& NUMBER_OF_PROCESSES, int& counter, vector<int>& resources, vector<vector<int>>& base_table) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Failed to open file.\n";
        return false;
    }

    if (!(file >> MAX_CAPACITY >> NUMBER_OF_RESOURCES >> NUMBER_OF_PROCESSES >> counter)) {
        cerr << "Error reading parameters.\n";
        return false;
    }

    resources.resize(NUMBER_OF_RESOURCES);
    for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
        if (!(file >> resources[i])) {
            cerr << "Error reading resources.\n";
            return false;
        }
    }

    base_table.resize(NUMBER_OF_PROCESSES, vector<int>(NUMBER_OF_RESOURCES));
    for (int i = 0; i < NUMBER_OF_PROCESSES; ++i) {
        for (int j = 0; j < NUMBER_OF_RESOURCES; ++j) {
            if (!(file >> base_table[i][j])) {
                cerr << "Error reading base table at process " << i << ", resource " << j << ".\n";
                return false;
            }
        }
    }

    cout << "Game loaded successfully!\n";
    return true;
}

void level(int MAX_CAPACITY, int NUMBER_OF_RESOURCES, int NUMBER_OF_PROCESSES) {
    int MAX_CAP = MAX_CAPACITY;
    int NUM_RES = NUMBER_OF_RESOURCES;
    int NUM_PROC = NUMBER_OF_PROCESSES;
    vector<int> resources;
    vector<vector<int>> base_table;
    int counter = 0;
    bool game_loaded = false;

    // Если параметры нулевые, загружаем игру
    if (MAX_CAP == 0 && NUM_RES == 0 && NUM_PROC == 0) {
        string filename;
        cout << "Enter filename to load: ";
        cin >> filename;
        if (!load_game(filename, MAX_CAP, NUM_RES, NUM_PROC, counter, resources, base_table)) {
            cerr << "Exiting due to load failure.\n";
            return;
        }
        game_loaded = true;
    }
    else {
        // Инициализация новой игры
        srand(time(0));
        resources.resize(NUM_RES);
        for (int i = 0; i < NUM_RES; ++i) {
            resources[i] = rand() % MAX_CAP + 1;
        }
        base_table.resize(NUM_PROC, vector<int>(NUM_RES, 0));
        for (int i = 0; i < NUM_PROC; ++i) {
            for (int j = 0; j < NUM_RES; ++j) {
                base_table[i][j] = rand() % resources[j] + 1;
            }
        }
    }

    // Основной цикл игры
    while (true) {
        if (!game_loaded) {
            for (int i = 0; i < NUM_PROC; ++i) {
                cout << "process_" << i + 1;
                for (int j = 0; j < NUM_RES; ++j) {
                    cout << " " << (base_table[i][j] < 0 ? 0 : base_table[i][j]);
                }
                cout << endl;
            }
        }
        game_loaded = false;

        // Генерация запроса
        int require_process = rand() % NUM_PROC + 1;
        int require_res = rand() % NUM_RES + 1;
        if (base_table[require_process - 1][require_res - 1] < 0) continue;

        cout << "\nCurrent resources: ";
        for (int res : resources) cout << res << " ";
        cout << "\nProcess " << require_process << " requires "
            << base_table[require_process - 1][require_res - 1]
            << " of resource " << require_res << endl;

        // Обработка ввода
        string input;
        cout << "Enter your answer (1/0), 'save' or 'load': ";
        cin >> input;

        if (input == "save") {
            string filename;
            cout << "Enter filename: ";
            cin >> filename;
            save_game(filename, MAX_CAP, NUM_RES, NUM_PROC, counter, resources, base_table);
            continue;
        }
        else if (input == "load") {
            string filename;
            cout << "Enter filename: ";
            cin >> filename;
            if (load_game(filename, MAX_CAP, NUM_RES, NUM_PROC, counter, resources, base_table)) {
                game_loaded = true;
                system("cls");
            }
            continue;
        }

        // Проверка ответа
        bool answer = (input == "1");
        if (answer) {
            if (resources[require_res - 1] >= base_table[require_process - 1][require_res - 1]) {
                resources[require_res - 1] -= base_table[require_process - 1][require_res - 1];
                base_table[require_process - 1][require_res - 1] *= -1;

                // Проверка на завершение процесса
                bool all_allocated = true;
                for (int val : base_table[require_process - 1]) {
                    if (val > 0) {
                        all_allocated = false;
                        break;
                    }
                }

                if (all_allocated) {
                    for (int i = 0; i < NUM_RES; ++i) {
                        resources[i] += abs(base_table[require_process - 1][i]); // Исправлено на abs()
                    }
                    counter++;
                }
            }
            else {
                cout << "Insufficient resources!\n";
            }
        }

        system("cls");

        // Проверка условий победы/поражения
        if (counter == NUM_PROC) {
            cout << "You win!\n";
            level(++MAX_CAPACITY, ++NUMBER_OF_RESOURCES, ++NUMBER_OF_PROCESSES);
            break;
        }
        if (!checker(base_table, resources)) {
            cout << "Deadlock occurred! You lose!\n";
            break;
        }
    }
}