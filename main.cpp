#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <time.h>

using namespace std;

void initialize_page(vector<vector<string>>& page, int lenght, int width, string& inside)
{
	vector <string> x_page(lenght, inside);
	for(int i = 0; i < width; i++)
	{
		page.push_back(x_page);
	}
}

void initialize_margins_page(vector<vector<string>>& page, string& margins)
{
	int width = page.size();
	int lenght = page[0].size();

	// up margins	
	for(int i = 0; i < lenght; i++)
		page[0][i] = margins;

	// down margins
	for(int i = 0; i < lenght; i++)
		page[width - 1][i] = margins;

	// right margins
	for(int i = 0; i < width; i++)
		page[i][0] = margins;			

	// left margins
	for(int i = 0; i < width; i++)
		page[i][lenght - 1] = margins;
}

void initialize_snake_in_page(vector<vector<string>>& page, string& snake, int snake_size)
{
	int x_snake = page[0].size() / 2;
	int y_snake = page.size() / 2;

	for(int i = 1; i < snake_size ; i++)
		page[y_snake][x_snake + i] = snake;
}

void initialize_food_in_page(vector<vector<string>>& page, string& inside, string& food)
{
	int lenght = page[0].size();
	int width = page.size();

	while(true)
	{	
		int x_food = rand() % lenght;
		int y_food = rand() % width;
		if(page[y_food][x_food] == inside)
		{	
			page[y_food][x_food] = food;
			return;
		}
	}
}

void print_page(vector<vector<string>>& page)
{
	for(size_t i = 0; i < page.size(); i++)
	{
		for(size_t j = 0; j < page[i].size(); j++)
		{	
			cout << page[i][j] << " " ;
		}
		cout << endl;
	}
}

int main()
{
	// Srand
	srand(time(0));

	// Confegur setting
	vector <vector<string>> page;
	string margins = "#";
	string inside = " ";
	string snake = ">";
	string food = "*";
	int length_page = 30;
	int width_page = 20;
	int snake_size = 5;

	initialize_page(page, length_page, width_page, inside);
	initialize_margins_page(page, margins);
	initialize_snake_in_page(page, snake, snake_size);
	initialize_food_in_page(page, inside, food);
	print_page(page);
}
