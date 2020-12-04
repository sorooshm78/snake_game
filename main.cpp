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

void initialize_snake_in_page(vector<vector<string>>& page,vector<pair<int, int>>& snake_coordinates, string& snake, int snake_size)
{
	int x_snake = page[0].size() / 2;
	int y_snake = page.size() / 2;

	for(int i = 0; i < snake_size ; i++)
	{
		snake_coordinates.push_back(pair<int, int>(y_snake, x_snake + i));
		page[y_snake][x_snake + i] = snake;
	}
}

void insert_food_in_page(vector<vector<string>>& page, vector<pair<int, int>>& food_coordinates, string& inside, string& food)
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
			food_coordinates.push_back(pair<int, int>(y_food, x_food));
			return;
		}
	}
}

void insert_snake_in_page(vector<vector<string>>& page, vector<pair<int, int>>& snake_coordinates, string& snake)
{
	for(size_t i = 0; i < snake_coordinates.size(); i++)
	{
		page[snake_coordinates[i].first][snake_coordinates[i].second] = snake;
	}
}

void move(vector<pair<int, int>>& snake_coordinates, string& move_type)
{
	int x_head_snake = snake_coordinates[0].second;
	int y_head_snake = snake_coordinates[0].first;
	
	// Snake move to left 
	if(move_type == "left")
		snake_coordinates.insert(snake_coordinates.begin(), pair<int, int>(y_head_snake, x_head_snake - 1));		
	
	// Snake move to right 
	if(move_type == "right")
		snake_coordinates.insert(snake_coordinates.begin(), pair<int, int>(y_head_snake, x_head_snake + 1));		

	// Snake move to up
	if(move_type == "up")
		snake_coordinates.insert(snake_coordinates.begin(), pair<int, int>(y_head_snake - 1, x_head_snake));		

	// Snake move to down 
	if(move_type == "down")
		snake_coordinates.insert(snake_coordinates.begin(), pair<int, int>(y_head_snake + 1, x_head_snake));		

	snake_coordinates.pop_back();	
}

void clear_page_from_snake(vector<vector<string>>& page, string& inside, string& snake)
{
	for(size_t i = 0; i < page.size(); i++)
	{
		for(size_t j = 0; j < page[i].size(); j++)
		{	
			if(page[i][j] == snake)
				page[i][j] = inside;
		}
	}
}

void clear_page_from_food(vector<vector<string>>& page, string& inside, string& food)
{
	for(size_t i = 0; i < page.size(); i++)
	{
		for(size_t j = 0; j < page[i].size(); j++)
		{	
			if(page[i][j] == food)
				page[i][j] = inside;
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
	//srand(time(0));

	// Confegur setting
	vector<pair<int, int>> snake_coordinates;
	vector<pair<int, int>> food_coordinates;
	vector<vector<string>> page;
	string move_type = "left"; 
	string margins = "#";
	string inside = ".";
	string snake = "+";
	string food = "*";
	int length_page = 30;
	int width_page = 20;
	int snake_size = 3;

	initialize_page(page, length_page, width_page, inside);
	initialize_margins_page(page, margins);
	initialize_snake_in_page(page, snake_coordinates, snake, snake_size);
	insert_food_in_page(page, food_coordinates, inside, food);

	print_page(page);
	clear_page_from_snake(page, inside, snake);
	move(snake_coordinates, move_type);
	insert_snake_in_page(page, snake_coordinates, snake);
	print_page(page);
}
