#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "level.h"
using namespace std;

bool checker(vector<vector<int>>& table, vector<int>& resources)
{
	for (int i = 0; i < table.size(); i++)
	{
		for (int x = 0; x < resources.size(); x++)
		{
			if (table[i][x] > 0 && resources[x] >= table[i][x])
			{
				return true;
			}
		}
	}
	return false;
}
void level(int MAX_CAPACITY, int NUMBER_OF_RESOURCES, int NUMBER_OF_PROCCESSES)
{


	std::srand(std::time(0));
	vector<int> resources;
	for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
	{
		int res = std::rand() % MAX_CAPACITY + 1;
		resources.push_back(res);
	}

	vector<vector<int>> base_table(NUMBER_OF_PROCCESSES, vector<int>(NUMBER_OF_RESOURCES, 0));
	for (int i = 0; i < NUMBER_OF_PROCCESSES; i++)
	{
		for (int x = 0; x < NUMBER_OF_RESOURCES; x++)
		{
			int res = std::rand() % resources[x] + 1;
			base_table[i][x] = res;
		}
	}

	for (int i = 0; i < NUMBER_OF_PROCCESSES; i++)
	{
		cout << "proccess_" << i + 1;
		for (int x = 0; x < NUMBER_OF_RESOURCES; x++)
		{
			if (base_table[i][x] < 0)
			{
				cout << " 0 ";
				continue;
			}
			cout << " " << base_table[i][x];
		}
		cout << endl;
	}

	bool answer;
	int counter = 0;
	while (true)
	{
		int require_process = std::rand() % NUMBER_OF_PROCCESSES + 1;
		int require_res = std::rand() % NUMBER_OF_RESOURCES + 1;
		if (base_table[require_process - 1][require_res - 1] < 0) continue;
		for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
		{
			cout << resources[i] << " ";
		}
		cout << endl;
		cout << "process " << require_process << " requires " << base_table[require_process - 1][require_res - 1] << " res_" << require_res << endl;
		cout << "Answer: ";
		cin >> answer;
		system("cls");
		if (answer)
		{
			if (resources[require_res - 1] >= base_table[require_process - 1][require_res - 1])
			{
				resources[require_res - 1] -= base_table[require_process - 1][require_res - 1];
				base_table[require_process - 1][require_res - 1] *= -1;
				bool flag = true;
				for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
				{
					if (base_table[require_process - 1][i] > 0)
					{
						flag = false;
						break;
					}
				}
				if (flag)
				{
					for (int i = 0; i < NUMBER_OF_RESOURCES; i++)
					{
						resources[i] -= base_table[require_process - 1][i];
					}
					counter++;
				}
			}
			else
			{
				cout << "You can't do this" << endl;
			}
		}
		if (counter == NUMBER_OF_PROCCESSES)
		{
			system("cls");
			cout << "You win";
			break;
		}

		if (!checker(base_table, resources))
		{
			system("cls");
			cout << "You lose";
			break;
		}

		for (int i = 0; i < NUMBER_OF_PROCCESSES; i++)
		{
			cout << "proccess_" << i + 1;
			for (int x = 0; x < NUMBER_OF_RESOURCES; x++)
			{
				if (base_table[i][x] < 0)
				{
					cout << " 0";
					continue;
				}
				cout << " " << base_table[i][x];
			}
			cout << endl;
		}

	}

}


