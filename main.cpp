#include <iostream>
#include <thread>
#include <chrono>
#include <stdlib.h>
#include <string>
#include <vector>
#include <time.h>
#include <iostream>
#include <termios.h>
#include <atomic>

#define STDIN_FILENO 0

#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68

#define EASY 500

using namespace std;

// Initialize game page for begin
void initialize_page(vector<vector<string>>& page, int lenght, int width, string& empty)
{
	vector <string> x_page(lenght, empty);
	for(int i = 0; i < width; i++)
	{
		page.push_back(x_page);
	}
}

// Insert margins in game page
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

// First insert snake coordinates in matrix page
void initialize_snake_in_page(vector<vector<string>>& page, vector<pair<int, int>>& snake_coordinates, string& snake, int snake_size)
{
	int x_head_snake = page[0].size() / 2;
	int y_head_snake = page.size() / 2;

	for(int i = 0; i < snake_size ; i++)
	{
		snake_coordinates.push_back(pair<int, int>(y_head_snake, x_head_snake + i));
		page[y_head_snake][x_head_snake + i] = snake;
	}
}

// Insert food coordinates in matrix page
void insert_food_in_page(vector<vector<string>>& page, vector<pair<int, int>>& food_coordinates, string& empty, string& food, int count_food)
{
	int lenght = page[0].size();
	int width = page.size();
	
	for(int i = 0; i < count_food ;i++) 
	{
		while(true)
		{	
			int x_food = rand() % lenght;
			int y_food = rand() % width;
			if(page[y_food][x_food] == empty)
			{	
				page[y_food][x_food] = food;
				food_coordinates.push_back(pair<int, int>(y_food, x_food));
				break;
			}
		}
	}
}

// Insert snake coordinates in matrix page
void insert_snake_in_page(vector<vector<string>>& page, vector<pair<int, int>>& snake_coordinates, string& snake)
{
	for(size_t i = 0; i < snake_coordinates.size(); i++)
	{
		page[snake_coordinates[i].first][snake_coordinates[i].second] = snake;
	}
}

// Move snake in page
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
}

// Clear snake in matrix game page
void clear_page_from_snake(vector<vector<string>>& page, string& empty, string& snake)
{
	for(size_t i = 0; i < page.size(); i++)
	{
		for(size_t j = 0; j < page[i].size(); j++)
		{	
			if(page[i][j] == snake)
				page[i][j] = empty;
		}
	}
}

// Clear food in matrix game page
void clear_page_from_food(vector<vector<string>>& page, string& empty, string& food)
{
	for(size_t i = 0; i < page.size(); i++)
	{
		for(size_t j = 0; j < page[i].size(); j++)
		{	
			if(page[i][j] == food)
				page[i][j] = empty;
		}
	}
}

// Print matrix game page in output
void print_page(vector<vector<string>>& page, int& score_player1)
{
	system("clear");
	cout << "player 1" << "				" << "player 2" << endl;
	cout << "score : " << score_player1 << endl;
	for(size_t i = 0; i < page.size(); i++)
	{
		for(size_t j = 0; j < page[i].size(); j++)
		{	
			cout << page[i][j] << " " ;
		}
		cout << endl;
	}
}

// Check snake eat food
bool check_get_score(vector<pair<int, int>>& snake_coordinates, vector<pair<int, int>>& food_coordinates)
{
	int x_head_snake = snake_coordinates[0].second;
	int y_head_snake = snake_coordinates[0].first;

	for(size_t i = 0; i < food_coordinates.size(); i++)
	{ 
		if(food_coordinates[i].first ==  y_head_snake and food_coordinates[i].second ==  x_head_snake)
		{
			// Delete food from coordinates
			food_coordinates.erase(food_coordinates.begin() + i);	
			return true;	
		}
	}
	return false;
}

// Add score_player1 when get score_player1
void add_score(int& score, int value)
{
	score += value;
}

// Check game over
bool check_game_over(vector<pair<int, int>>& snake_coordinates)
{
	int x_head_snake = snake_coordinates[0].second;
	int y_head_snake = snake_coordinates[0].first;

	for(size_t i = 1; i < snake_coordinates.size(); i++)
	{
		if(y_head_snake == snake_coordinates[i].first and x_head_snake == snake_coordinates[i].second)
			return true;
	}
	return false;
}

// Change input move type to move type
void change_move_type_player1(string& move_type, char input_move_type)
{
	if(input_move_type == KEY_UP)
		move_type = "up";

	if(input_move_type == KEY_DOWN)
		move_type = "down";

	if(input_move_type == KEY_LEFT)
		move_type = "left";

	if(input_move_type == KEY_RIGHT)
		move_type = "right";
}

// Change input move type to move type
void change_move_type_player2(string& move_type, char input_move_type)
{
	if(input_move_type == 'w')
		move_type = "up";

	if(input_move_type == 's')
		move_type = "down";

	if(input_move_type == 'a')
		move_type = "left";

	if(input_move_type == 'd')
		move_type = "right";
}

// Check crash wall and handle it
void handle_crash_wall(vector<vector<string>>& page, vector<pair<int, int>>& snake_coordinates)
{
	int x_head_snake = snake_coordinates[0].second;
	int y_head_snake = snake_coordinates[0].first;

	int margins_up = 0;
	int margins_down = page.size() - 1;
	int margins_left = 0;
	int margins_right = page[0].size() - 1;

	// Wall left
	if(x_head_snake == margins_left)
		 snake_coordinates[0].second = margins_right - 1;

	// Wall right
	if(x_head_snake == margins_right)
		 snake_coordinates[0].second = margins_left + 1;
	
	// Wall up
	if(y_head_snake == margins_up)
		 snake_coordinates[0].first = margins_down - 1;

	// Wall down
	if(y_head_snake == margins_down)
		 snake_coordinates[0].first = margins_up + 1;
}

// Show game over message
void message_game_over()
{
	cout << endl;
	cout << "you are is game over!!!" << endl;
}

// Cut snake tail in each move
void cut_snake_tail(vector<pair<int, int>>& snake_coordinates)
{
	snake_coordinates.pop_back();	
}

// Check correct input move type
bool check_correct_input_move_type_player1(string& move_type, char input_move_type)
{
	if(move_type == "left" and input_move_type == KEY_RIGHT)
	{	
		move_type == "left";
		return false;
	}
	
	if(move_type == "right" and input_move_type == KEY_LEFT)
	{	
		move_type == "right";
		return false;
	}

	if(move_type == "up" and input_move_type == KEY_DOWN)
	{	
		move_type == "up";
		return false;
	}
	
	if(move_type == "down" and input_move_type == KEY_UP)
	{
		move_type == "down";
		return false;
	}
	return true;
}

// Check correct input move type
bool check_correct_input_move_type_player2(string& move_type, char input_move_type)
{
	if(move_type == "left" and input_move_type == 'd')
	{	
		move_type == "left";
		return false;
	}
	
	if(move_type == "right" and input_move_type == 'a')
	{	
		move_type == "right";
		return false;
	}

	if(move_type == "up" and input_move_type == 's')
	{	
		move_type == "up";
		return false;
	}
	
	if(move_type == "down" and input_move_type == 'w')
	{
		move_type == "down";
		return false;
	}
	return true;
}

void read_input_player1(string& move_type, atomic<bool>& end_game)
{
	while(!end_game)
	{	
		// Black magic to prevent Linux from buffering keystrokes.
		struct termios t;
		tcgetattr(STDIN_FILENO, &t);
		t.c_lflag &= ~ICANON;
		tcsetattr(STDIN_FILENO, TCSANOW, &t);

		// Once the buffering is turned off, the rest is simple.
		char first,second,input_move_type;
		
		cin >> first;
		cin >> second;
		cin >> input_move_type;

		// Using 3 char type, Cause up down right left consist with 3 character
		if ((first == 27) and (second == 91)) 
		{
			if(check_correct_input_move_type_player1(move_type, input_move_type))
				change_move_type_player1(move_type, input_move_type);
		}
		this_thread::sleep_for(chrono::milliseconds(EASY));
	}
}
			
void read_input_player2(string& move_type)
{
	while (true)
	{	
		// Black magic to prevent Linux from buffering keystrokes.
		struct termios t;
		tcgetattr(STDIN_FILENO, &t);
		t.c_lflag &= ~ICANON;
		tcsetattr(STDIN_FILENO, TCSANOW, &t);

		char input_move_type;
		// Get input move type
		cin >> input_move_type;

		// Check correct input and if correct moved
		if(check_correct_input_move_type_player2(move_type, input_move_type))
			change_move_type_player2(move_type, input_move_type);

		this_thread::sleep_for(chrono::milliseconds(1000));
	}
}


int main()
{
	srand(time(0));

	// Configure setting snake game
	atomic<bool> end_game {false};
	vector<pair<int, int>> snake_coordinates_player1;
	vector<pair<int, int>> food_coordinates;
	vector<vector<string>> page;
	string move_type_player1 = "left"; 
	string margins = "#";
	string empty = ".";
	string snake = "+";
	string food = "*";
	int length_page = 30;
	int value_score = 1;
	int width_page = 20;
 	int count_food = 1;
	int snake_size_palyer1 = 5;
	int score_player1 = 0;

	// Initialize Primitive Game Page
	initialize_page(page, length_page, width_page, empty);
	initialize_margins_page(page, margins);
	initialize_snake_in_page(page, snake_coordinates_player1, snake, snake_size_palyer1);
	insert_food_in_page(page, food_coordinates, empty, food, count_food);
	
	// Primitive Print
	print_page(page, score_player1);

	thread thread_for_read_input_player1(read_input_player1, ref(move_type_player1), ref(end_game));

	// Every time snake move
	while(true)
	{
		this_thread::sleep_for(chrono::milliseconds(EASY));

		// Clear snake in maxrix game page
		clear_page_from_snake(page, empty, snake);
	
		// Move snake by input move type 
		move(snake_coordinates_player1, move_type_player1);
		
		// Check and handle crash wall
		handle_crash_wall(page, snake_coordinates_player1);

		// Check game over and end game
		if(check_game_over(snake_coordinates_player1))
		{
			message_game_over();
			end_game = true;
			thread_for_read_input_player1.join();	
			return 0;
		}

		// Check get score_player1 when snake eat food
		if(check_get_score(snake_coordinates_player1, food_coordinates))
		{
			// Add score_player1 and cut not snake tail
			snake_size_palyer1 ++; 
			add_score(score_player1, value_score);
			insert_food_in_page(page, food_coordinates, empty, food, 1);
		}
		else
			cut_snake_tail(snake_coordinates_player1);

		// Insert snake in matrix page when snake move
		insert_snake_in_page(page, snake_coordinates_player1, snake);
		
		// Print martix game page
		print_page(page, score_player1);
		//this_thread::sleep_for(chrono::milliseconds(1000));
	}
}
