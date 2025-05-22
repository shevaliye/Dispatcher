#include <iostream>
#include "level.h"

int main() {
    cout << "Start new game (1) or load game (2)? ";
    int choice;
    cin >> choice;

    if (choice == 1) {
        cout << "Enter parameters: resource_capacity | number_of_resources | number_of_processes\n";
        int rc, nr, np;
        cin >> rc >> nr >> np;
        level(rc, nr, np);
    }
    else {
        level(0, 0, 0); // Параметры перезапишутся при загрузке
    }
    return 0;
}