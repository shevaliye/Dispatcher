#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "level.h"

class Button {
public:
    sf::RectangleShape shape;
    sf::Text text;
    bool isPressed = false;

    Button(sf::Font& font, const std::string& btnText, float x, float y, float width, float height,
        sf::Color bgColor = sf::Color(70, 70, 70),
        sf::Color textColor = sf::Color::White) {
        shape.setSize(sf::Vector2f(width, height));
        shape.setPosition(x, y);
        shape.setFillColor(bgColor);
        shape.setOutlineThickness(2);
        shape.setOutlineColor(sf::Color::White);

        text.setFont(font);
        text.setString(btnText);
        text.setCharacterSize(20);
        text.setFillColor(textColor);
        sf::FloatRect textRect = text.getLocalBounds();
        text.setOrigin(textRect.left + textRect.width / 2.0f,
            textRect.top + textRect.height / 2.0f);
        text.setPosition(x + width / 2.0f, y + height / 2.0f);
    }

    bool contains(float x, float y) const {
        return shape.getGlobalBounds().contains(x, y);
    }

    void update(const sf::Vector2f& mousePos) {
        if (contains(mousePos.x, mousePos.y)) {
            shape.setOutlineColor(sf::Color::Yellow);
        }
        else {
            shape.setOutlineColor(sf::Color::White);
        }
    }

    void draw(sf::RenderWindow& window) const {
        window.draw(shape);
        window.draw(text);
    }
};

class Table {
public:
    std::vector<std::string> processNames;
    std::vector<std::string> resourceNames;
    std::vector<std::vector<std::string>> allocationData; // Для отображения (всегда >= 0)
    std::vector<std::string> totalResources;             // Для отображения
    std::vector<int> resources;                          // Фактические ресурсы
    std::vector<std::vector<int>> allocations;           // Остаток запроса (может быть <0)
    std::vector<std::vector<int>> allocatedResources;    // Сколько уже выделено

    sf::Font font;
    sf::Color headerColor = sf::Color::Yellow;
    sf::Color cellColor = sf::Color::White;
    unsigned int cellPadding = 10;

    Table() {
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            std::cerr << "Failed to load font! Using default.\n";
            static sf::Font defaultFont;
            font = defaultFont;
        }
    }

    void initialize(int processes, int res, const std::vector<int>& totals,
        const std::vector<std::vector<int>>& allocs) {
        // Проверка входных данных
        if (processes <= 0 || res <= 0 ||
            totals.size() != static_cast<size_t>(res) ||
            allocs.size() != static_cast<size_t>(processes)) {
            std::cerr << "Invalid initialization parameters!\n";
            return;
        }
        processNames.clear();
        resourceNames.clear();
        allocationData.clear();
        totalResources.clear();

        resources = totals;
        allocations = allocs;
        allocatedResources.resize(processes, std::vector<int>(res, 0));

        // Инициализация имен процессов
        for (int i = 0; i < processes; i++) {
            processNames.push_back("Process " + std::to_string(i + 1));
        }

        // Инициализация имен ресурсов
        for (int i = 0; i < res; i++) {
            resourceNames.push_back("Res " + std::to_string(i + 1));
        }

        // Инициализация данных для отображения
        allocationData.resize(processes, std::vector<std::string>(res));
        for (int i = 0; i < processes; i++) {
            for (int j = 0; j < res; j++) {
                allocationData[i][j] = std::to_string(allocs[i][j]);
            }
        }

        // Инициализация отображения общих ресурсов
        totalResources.resize(res);
        for (int i = 0; i < res; i++) {
            totalResources[i] = std::to_string(totals[i]);
        }

        // Проверка выделенной памяти
    if (allocations.size() != processNames.size() || 
        (!allocations.empty() && allocations[0].size() != resourceNames.size())) {
        std::cerr << "Initialization failed: size mismatch!\n";
    }
    }

    bool allocateResource(int processIdx, int resIdx) {
        // Проверка валидности индексов
        if (processIdx < 0 || processIdx >= processNames.size() ||
            resIdx < 0 || resIdx >= resourceNames.size()) {
            return false;
        }

        // Проверка, требуется ли еще ресурс
        if (allocations[processIdx][resIdx] <= 0) {
            return false;
        }

        // Вычисляем сколько можем взять
        int amount_to_take = std::min(allocations[processIdx][resIdx], resources[resIdx]);

        if (amount_to_take > 0) {
            // Обновляем фактические ресурсы
            resources[resIdx] -= amount_to_take;

            // Обновляем остаток запроса
            allocations[processIdx][resIdx] -= amount_to_take;

            // Запоминаем сколько выделили
            allocatedResources[processIdx][resIdx] += amount_to_take;

            // Обновляем отображаемые данные
            allocationData[processIdx][resIdx] = allocations[processIdx][resIdx] > 0 ?
                std::to_string(allocations[processIdx][resIdx]) : "0";

            totalResources[resIdx] = std::to_string(resources[resIdx]);
            return true;
        }
        return false;
    }

    void completeProcess(int processIdx) {
        // Проверка валидности индекса
        if (processIdx < 0 || processIdx >= allocations.size()) {
            std::cerr << "Invalid process index in completeProcess: " << processIdx << std::endl;
            return;
        }

        for (size_t i = 0; i < resourceNames.size(); ++i) {
            // Проверка границ для allocatedResources
            if (i < allocatedResources[processIdx].size() && allocatedResources[processIdx][i] > 0) {
                // Проверка границ для resources
                if (i < resources.size()) {
                    resources[i] += allocatedResources[processIdx][i];
                    // Обновление отображаемых данных с проверкой
                    if (i < totalResources.size()) {
                        totalResources[i] = std::to_string(resources[i]);
                    }
                }
                allocatedResources[processIdx][i] = 0;
            }

            // Сброс запросов процесса с проверкой
            if (i < allocations[processIdx].size()) {
                allocations[processIdx][i] = 0;
            }
            if (processIdx < allocationData.size() && i < allocationData[processIdx].size()) {
                allocationData[processIdx][i] = "0";
            }
        }
    }

    void draw(sf::RenderWindow& window, float startX, float startY) const {
        if (processNames.empty() || resourceNames.empty()) return;

        const float rowHeight = 40.f;
        const float colWidth = 150.f;

        // Рисуем фон таблицы
        sf::RectangleShape tableBg(sf::Vector2f(
            colWidth * (resourceNames.size() + 1),
            rowHeight * (processNames.size() + 2)));
        tableBg.setPosition(startX, startY);
        tableBg.setFillColor(sf::Color(30, 30, 30));
        window.draw(tableBg);

        // Заголовки ресурсов
        for (size_t col = 0; col < resourceNames.size(); ++col) {
            sf::Text text;
            text.setFont(font);
            text.setString(resourceNames[col]);
            text.setCharacterSize(20);
            text.setFillColor(headerColor);
            text.setPosition(startX + (col + 1) * colWidth + cellPadding,
                startY + cellPadding);
            window.draw(text);
        }

        // Имена процессов
        for (size_t row = 0; row < processNames.size(); ++row) {
            sf::Text text;
            text.setFont(font);
            text.setString(processNames[row]);
            text.setCharacterSize(20);
            text.setFillColor(cellColor);
            text.setPosition(startX + cellPadding,
                startY + (row + 1) * rowHeight + cellPadding);
            window.draw(text);
        }

        // Данные аллокаций
        for (size_t row = 0; row < processNames.size(); ++row) {
            for (size_t col = 0; col < resourceNames.size(); ++col) {
                sf::Text text;
                text.setFont(font);
                text.setString(allocationData[row][col]);
                text.setCharacterSize(20);
                text.setFillColor(cellColor);
                text.setPosition(startX + (col + 1) * colWidth + cellPadding,
                    startY + (row + 1) * rowHeight + cellPadding);
                window.draw(text);
            }
        }

        // Итоговые ресурсы
        sf::Text totalText("Total", font, 20);
        totalText.setFillColor(headerColor);
        totalText.setPosition(startX + cellPadding,
            startY + (processNames.size() + 1) * rowHeight + cellPadding);
        window.draw(totalText);

        for (size_t col = 0; col < resourceNames.size(); ++col) {
            sf::Text text;
            text.setFont(font);
            text.setString(totalResources[col]);
            text.setCharacterSize(20);
            text.setFillColor(cellColor);
            text.setPosition(startX + (col + 1) * colWidth + cellPadding,
                startY + (processNames.size() + 1) * rowHeight + cellPadding);
            window.draw(text);
        }
    }
};

void generateNewRequest(Table& table, int& process, int& resource, sf::Text& requestText) {
    if (table.processNames.empty() || table.resourceNames.empty()) {
        requestText.setString("No processes/resources available!");
        return;
    }

    std::vector<std::pair<int, int>> available_requests;
    for (size_t p = 0; p < table.processNames.size(); p++) {
        for (size_t r = 0; r < table.resourceNames.size(); r++) {
            if (p < table.allocations.size() &&
                r < table.allocations[p].size() &&
                table.allocations[p][r] > 0 &&
                r < table.resources.size()) {
                available_requests.emplace_back(p, r);
            }
        }
    }
    if (available_requests.empty()) {
        // Проверяем победу или deadlock
        bool all_completed = true;
        for (const auto& proc : table.allocations) {
            for (int val : proc) {
                if (val > 0) {
                    all_completed = false;
                    break;
                }
            }
            if (!all_completed) break;
        }

        if (all_completed) {
            requestText.setString("YOU WIN! All processes completed!");

        }
        else {
            requestText.setString("DEADLOCK! No available requests");
        }
        return;
    }

    // Выбираем случайный валидный запрос (исправленная строка)
    const auto& selected = available_requests[rand() % available_requests.size()];
    process = selected.first;
    resource = selected.second;

    requestText.setString("Process " + std::to_string(process + 1) +
        " requests " + table.allocationData[process][resource] +
        " of Resource " + std::to_string(resource + 1));
}

void handleApproval(Table& table, int& process, int& resource,
    sf::Text& requestText, int& counter,
    int& MAX_CAPACITY, int& NUMBER_OF_RESOURCES,
    int& NUMBER_OF_PROCESSES, bool& showTable) {

    // Проверка валидности индексов
    if (process < 0 || process >= table.processNames.size() ||
        resource < 0 || resource >= table.resourceNames.size()) {
        requestText.setString("Invalid process or resource index!");
        return;
    }

    if (table.allocateResource(process, resource)) {
        // Проверка завершения процесса с защитой от выхода за границы
        bool completed = true;
        if (process >= 0 && process < table.allocations.size()) {
            for (size_t i = 0; i < table.allocations[process].size(); ++i) {
                if (table.allocations[process][i] > 0) {
                    completed = false;
                    break;
                }
            }
        }
        else {
            completed = false;
        }

        if (completed) {
            counter++;
            table.completeProcess(process);

            // Проверка победы
            if (counter >= NUMBER_OF_PROCESSES) {
                requestText.setString("You win!\n");

                // Увеличиваем сложность
                MAX_CAPACITY++;
                NUMBER_OF_RESOURCES++;
                NUMBER_OF_PROCESSES++;

                // Перезапускаем игру с проверкой размеров
                std::vector<int> resources(NUMBER_OF_RESOURCES);
                std::vector<std::vector<int>> allocations(NUMBER_OF_PROCESSES,
                    std::vector<int>(NUMBER_OF_RESOURCES, 0));  // Явная инициализация нулями

                for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                    resources[i] = (rand() % MAX_CAPACITY) + 1;
                }

                for (int i = 0; i < NUMBER_OF_PROCESSES; i++) {
                    for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                        if (j < resources.size()) {  // Защита от выхода за границы
                            allocations[i][j] = (rand() % resources[j]) + 1;
                        }
                    }
                }

                table.initialize(NUMBER_OF_PROCESSES, NUMBER_OF_RESOURCES,
                    resources, allocations);
                counter = 0;

                // Генерируем новый запрос с проверкой
                if (table.processNames.size() > 0 && table.resourceNames.size() > 0) {
                    generateNewRequest(table, process, resource, requestText);
                }
                return;
            }
        }

        // Проверка deadlock с защитой
        if (!checker(table.allocations, table.resources)) {
            requestText.setString("Deadlock occurred! You lose!\n");
            return;
        }

        // Генерация нового запроса с проверкой
        if (table.processNames.size() > 0 && table.resourceNames.size() > 0) {
            generateNewRequest(table, process, resource, requestText);
        }
    }
    else {
        requestText.setString("Cannot allocate resource!");
        sf::sleep(sf::milliseconds(500));
        if (table.processNames.size() > 0 && table.resourceNames.size() > 0) {
            generateNewRequest(table, process, resource, requestText);
        }
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Resource Manager");
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        return 1;
    }

    sf::Clock messageTimer;  // Таймер для сообщений
    float messageDisplayTime = 2.0f; // Время показа сообщения (2 секунды)
    bool showingSystemMessage = false; // Флаг отображения сообщения

    // ===== Инициализация всех переменных =====
    // Игровые параметры
    int MAX_CAPACITY = 5;
    int NUMBER_OF_RESOURCES = 2;
    int NUMBER_OF_PROCESSES = 2;
    int counter = 0;
    bool showTable = false;
    int currentProcess = -1;
    int currentResource = -1;

    // Кнопки главного меню
    Button newGameBtn(font, "New game", 300, 250, 200, 40, sf::Color::Green);
    Button loadGameBtn(font, "Load game", 300, 300, 200, 40, sf::Color::Blue);

    // Кнопки игрового интерфейса
    Button approveBtn(font, "APPROVE (1)", 200, 450, 200, 50, sf::Color::Green);
    Button rejectBtn(font, "REJECT (0)", 400, 450, 200, 50, sf::Color::Red);

    Button saveBtn(font, "SAVE", 200, 520, 200, 50, sf::Color::Blue);
    Button loadBtn(font, "LOAD", 400, 520, 200, 50, sf::Color::Cyan);

    // Текстовые элементы
    sf::Text title("Resource Manager", font, 40);
    title.setFillColor(sf::Color::Cyan);
    title.setPosition(250, 150);

    sf::Text requestText("", font, 24);
    requestText.setPosition(50, 400);
    requestText.setFillColor(sf::Color::Cyan);

    sf::Text systemMessageText("", font, 24);
    systemMessageText.setPosition(50, 370); 
    systemMessageText.setFillColor(sf::Color::Cyan); 

    sf::Text messageText("", font, 24);
    messageText.setPosition(250, 350);
    messageText.setFillColor(sf::Color::Yellow);

    // Таблица
    Table table;
    // ===== Конец инициализации =====

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            // ===== Обработка событий =====
            if (!showTable) {
                // Главное меню
                if (event.type == sf::Event::MouseButtonReleased) {
                    // Новая игра
                    if (newGameBtn.contains(event.mouseButton.x, event.mouseButton.y)) {
                        // Сброс параметров к начальным значениям
                        MAX_CAPACITY = 5;
                        NUMBER_OF_RESOURCES = 2;
                        NUMBER_OF_PROCESSES = 2;

                        std::vector<int> resources(NUMBER_OF_RESOURCES);
                        std::vector<std::vector<int>> allocations(NUMBER_OF_PROCESSES,
                            std::vector<int>(NUMBER_OF_RESOURCES));

                        for (int i = 0; i < NUMBER_OF_RESOURCES; i++) {
                            resources[i] = rand() % MAX_CAPACITY + 1;
                        }

                        for (int i = 0; i < NUMBER_OF_PROCESSES; i++) {
                            for (int j = 0; j < NUMBER_OF_RESOURCES; j++) {
                                allocations[i][j] = rand() % resources[j] + 1;
                            }
                        }

                        table.initialize(NUMBER_OF_PROCESSES, NUMBER_OF_RESOURCES, resources, allocations);
                        showTable = true;
                        counter = 0;
                        generateNewRequest(table, currentProcess, currentResource, requestText);
                    }
                    // Загрузка игры
                    else if (loadGameBtn.contains(event.mouseButton.x, event.mouseButton.y)) {
                        if (load_game("save.txt", MAX_CAPACITY, NUMBER_OF_RESOURCES,
                            NUMBER_OF_PROCESSES, counter, table.resources, table.allocations)) {
                            table.initialize(NUMBER_OF_PROCESSES, NUMBER_OF_RESOURCES,
                                table.resources, table.allocations);
                            showTable = true;
                            generateNewRequest(table, currentProcess, currentResource, requestText);
                            messageText.setString("");
                        }
                        else {
                            messageText.setString("No saved game found!");
                        }
                    }
                }
            }
            else {
                // Игровой процесс
                if (event.type == sf::Event::MouseButtonReleased) {
                    if (approveBtn.contains(event.mouseButton.x, event.mouseButton.y)) {
                        handleApproval(table, currentProcess, currentResource,
                            requestText, counter,
                            MAX_CAPACITY, NUMBER_OF_RESOURCES,
                            NUMBER_OF_PROCESSES, showTable);
                    }
                    else if (rejectBtn.contains(event.mouseButton.x, event.mouseButton.y)) {
                        requestText.setString("Request rejected!");
                        sf::sleep(sf::milliseconds(500));
                        generateNewRequest(table, currentProcess, currentResource, requestText);
                    }
                }

                else if (saveBtn.contains(event.mouseButton.x, event.mouseButton.y)) {
                    save_game("save.txt", MAX_CAPACITY, NUMBER_OF_RESOURCES,
                        NUMBER_OF_PROCESSES, counter, table.resources, table.allocations);
                    systemMessageText.setString("Game saved!");
                    messageTimer.restart(); // Запускаем таймер
                    showingSystemMessage = true;
                }
                else if (loadBtn.contains(event.mouseButton.x, event.mouseButton.y)) {
                    if (load_game("save.txt", MAX_CAPACITY, NUMBER_OF_RESOURCES,
                        NUMBER_OF_PROCESSES, counter, table.resources, table.allocations)) {
                        table.initialize(NUMBER_OF_PROCESSES, NUMBER_OF_RESOURCES,
                            table.resources, table.allocations);
                        systemMessageText.setString("Game loaded!");
                    }
                    else {
                        systemMessageText.setString("Load failed!");
                    }
                    messageTimer.restart();
                    showingSystemMessage = true;
                }

                // Обработка клавиатуры
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Num1 || event.key.code == sf::Keyboard::Numpad1) {
                        handleApproval(table, currentProcess, currentResource,
                            requestText, counter,
                            MAX_CAPACITY, NUMBER_OF_RESOURCES,
                            NUMBER_OF_PROCESSES, showTable);
                    }
                    else if (event.key.code == sf::Keyboard::Num0 || event.key.code == sf::Keyboard::Numpad0) {
                        requestText.setString("Request rejected!");
                        sf::sleep(sf::milliseconds(500));
                        generateNewRequest(table, currentProcess, currentResource, requestText);
                    }
                }
            }
        }

        if (showingSystemMessage && messageTimer.getElapsedTime().asSeconds() > messageDisplayTime) {
            systemMessageText.setString("");
            showingSystemMessage = false;
        }

        // ===== Отрисовка =====
        window.clear(sf::Color::Black);

        if (showTable) {
            table.draw(window, 50, 50);
            window.draw(requestText);
            window.draw(systemMessageText);
            approveBtn.draw(window);
            rejectBtn.draw(window);
            saveBtn.draw(window);
            loadBtn.draw(window);
        }
        else {
            // Главное меню
            window.draw(title);
            newGameBtn.draw(window);
            loadGameBtn.draw(window);
            window.draw(messageText);
        }

        window.display();
    }

    return 0;
}