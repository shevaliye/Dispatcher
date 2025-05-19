#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#define max_capacity 15
using namespace std;

int main()
{	
	std::srand(std::time(0));
	int resource_1 = std::rand() % max_capacity + 1;
	int resource_2 = std::rand() % max_capacity + 1;
	vector<vector<int>> base_table(4, vector<int>(2,0));
	for (int i = 0; i < 4; i++)
	{
		int res_1 = std::rand() % resource_1 + 1;
		int res_2 = std::rand() % resource_2 + 1;
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
		cout << resource_1 << " " << resource_2 << endl;
		cout << "process " << require_process << " requires " << base_table[require_process - 1][require_res - 1] << " res_" << require_res << endl;
		cout << "Answer: ";
		cin >> answer;
		system("cls");
		if (answer)
		{
			if (require_res - 1 == 1)
			{
				if (resource_2 >= base_table[require_process - 1][require_res - 1])
				{
					resource_2 -= base_table[require_process - 1][require_res - 1];
					base_table[require_process - 1][require_res - 1] *= -1;
					if (base_table[require_process - 1][0] < 0)
					{
						resource_1 -= base_table[require_process - 1][0];
						resource_2 -= base_table[require_process - 1][require_res - 1];
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
				if (resource_1 >= base_table[require_process - 1][require_res - 1])
				{
					resource_1 -= base_table[require_process - 1][require_res - 1];
					base_table[require_process - 1][require_res - 1] *= -1;
					if (base_table[require_process - 1][1] < 0)
					{
						resource_2 -= base_table[require_process - 1][1];
						resource_1 -= base_table[require_process - 1][require_res - 1];
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
		if (counter == 4)
		{
			break;
		}

	}
	
}