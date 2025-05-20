#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#define max_capacity 15
using namespace std;
bool checker(vector<vector<int>>& table, vector<int>& resources)
{
	for (int i = 0; i < 4; i++)
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
int main()
{	
	std::srand(std::time(0));
	vector<int> resources;
	for (int i = 0; i < 2; i++)
	{
		int res = std::rand() % max_capacity + 1;
		resources.push_back(res);
	}
	//int resource_1 = std::rand() % max_capacity + 1;
	//int resource_2 = std::rand() % max_capacity + 1;
	vector<vector<int>> base_table(4, vector<int>(2,0));
	for (int i = 0; i < 4; i++)
	{
		int res_1 = std::rand() % resources[0] + 1;
		int res_2 = std::rand() % resources[1] + 1;
		base_table[i][0] = res_1;
		base_table[i][1] = res_2;
	}
	for (int i = 0; i < 2; i++)
	{
		cout << "res_" << i + 1<< endl;
		for (int x = 0; x < 4; x++)
		{
			cout << "proccess_" << x + 1 << " " << base_table[x][i] << endl;
		}
	}
	
	bool answer;
	int counter = 0;
	while (true)
	{
		int require_process = std::rand() % 4 + 1;
		int require_res = std::rand() % 2 + 1;
		if (base_table[require_process - 1][require_res - 1] <= 0) continue;
		cout << resources[0] << " " << resources[1] << endl;
		cout << "process " << require_process << " requires " << base_table[require_process - 1][require_res - 1] << " res_" << require_res << endl;
		cout << "Answer: ";
		cin >> answer;
		system("cls");
		if (answer)
		{
			if (require_res - 1 == 1)
			{
				if (resources[1] >= base_table[require_process - 1][require_res - 1])
				{
					resources[1] -= base_table[require_process - 1][require_res - 1];
					base_table[require_process - 1][require_res - 1] *= -1;
					if (base_table[require_process - 1][0] < 0)
					{
						resources[0] -= base_table[require_process - 1][0];
						resources[1] -= base_table[require_process - 1][require_res - 1];
						base_table[require_process - 1][0] = 0;
						base_table[require_process - 1][1] = 0;
						counter += 1;
					}
				}
				else
				{
					cout << "You can't do this" << endl;
				}
			}
			else
			{
				if (resources[0] >= base_table[require_process - 1][require_res - 1])
				{
					resources[0] -= base_table[require_process - 1][require_res - 1];
					base_table[require_process - 1][require_res - 1] *= -1;
					if (base_table[require_process - 1][1] < 0)
					{
						resources[1] -= base_table[require_process - 1][1];
						resources[0] -= base_table[require_process - 1][require_res - 1];
						base_table[require_process - 1][0] = 0;
						base_table[require_process - 1][1] = 0;
						counter += 1;
					}
				}
				else
				{
					cout << "You can't do this" << endl;
				}
			}
		}
		if (counter == 4)
		{
			system("cls");
			cout << "You win";
			break;
		}

		if (checker(base_table, resources) == false)
		{
			system("cls");
			cout << "You lose";
			break;
		}
		
		for (int i = 0; i < 2; i++)
		{
			cout << "res_" << i + 1 << endl;
			for (int x = 0; x < 4; x++)
			{
				if (base_table[x][i] < 0)
				{
					cout << "proccess_" << x + 1 << " " << 0 << endl;
					continue;
				}
				cout << "proccess_" << x + 1 << " " << base_table[x][i] << endl;
			}
		}

	}
	
}