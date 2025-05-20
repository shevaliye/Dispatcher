#include <iostream>
#include "level.h"
int main()
{
	cout << "Input parametres: resource_capacity | number_of_recources | number_of_proccesses" << endl;;
	int resource_capacity, number_of_recources, number_of_proccesses;
	cin >> resource_capacity>> number_of_recources>> number_of_proccesses;
	level(resource_capacity, number_of_recources, number_of_proccesses);

}