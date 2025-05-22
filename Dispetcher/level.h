#pragma once
#include <iostream>
#include <vector>
using namespace std;
void level(int MAX_CAPACITY, int NUMBER_OF_RESOURCES, int NUMBER_OF_PROCESSES);
bool checker(vector<vector<int>>& table, vector<int>& resources);
void save_game(const string& filename, int MAX_CAPACITY, int NUMBER_OF_RESOURCES, int NUMBER_OF_PROCESSES, int counter, const vector<int>& resources, const vector<vector<int>>& base_table);
bool load_game(const string& filename, int& MAX_CAPACITY, int& NUMBER_OF_RESOURCES, int& NUMBER_OF_PROCESSES, int& counter, vector<int>& resources, vector<vector<int>>& base_table);