#include <termios.h>
#include <iostream>
#include <stdlib.h>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <time.h>
#include <atomic>

#define STDIN_FILENO 0

#define KEY_UP 65
#define KEY_DOWN 66
#define KEY_RIGHT 67
#define KEY_LEFT 68

#define RESET   	"\033[0m"
#define BLACK		"\033[30m"      
#define RED			"\033[31m"     
#define GREEN		"\033[32m"     
#define YELLOW		"\033[33m"     
#define BLUE		"\033[34m"     
#define MAGENTA		"\033[35m"     
#define CYAN		"\033[36m"     
#define WHITE		"\033[37m"     
#define BOLDBLACK   "\033[1m\033[30m"     
#define BOLDRED     "\033[1m\033[31m"     
#define BOLDGREEN   "\033[1m\033[32m"     
#define BOLDYELLOW  "\033[1m\033[33m"    
#define BOLDBLUE    "\033[1m\033[34m"     
#define BOLDMAGENTA "\033[1m\033[35m"    
#define BOLDCYAN    "\033[1m\033[36m"     
#define BOLDWHITE   "\033[1m\033[37m"      

#define EASY 300

using namespace std;

void initialize_page(vector<vector<string>>& page, int lenght, int width, string& empty)
{
	vector <string> x_page(lenght, empty);
	for(int i = 0; i < width; i++)
	{
		page.push_back(x_page);
	}
}

void initialize_margins_page(vector<vector<string>>& page, string& margins)
{
	int width = page.size();
	int lenght = page[0].size();

	// UP margins	
	for(int i = 0; i < lenght; i++)
		page[0][i] = margins;

	// DOWN margins
	for(int i = 0; i < lenght; i++)
		page[width - 1][i] = margins;

	// RIGHT margins
	for(int i = 0; i < width; i++)
		page[i][0] = margins;			

	// LEFT margins
	for(int i = 0; i < width; i++)
		page[i][lenght - 1] = margins;
}

void initialize_snake_in_page(vector<vector<string>>& page, vector<pair<int, int>>& snake_coordinates1, string& snake1, vector<pair<int, int>>& snake_coordinates2, string& snake2, int snake_size, bool two_player_game)
{
	// player 1
	int x_head_snake1 = page[0].size() / 2;
	int y_head_snake1 = page.size() / 3;

	for(int i = 0; i < snake_size ; i++)
	{
		snake_coordinates1.push_back(pair<int, int>(y_head_snake1, x_head_snake1 + i));
		page[y_head_snake1][x_head_snake1 + i] = snake1;
	}
	
	// player 2
	if(two_player_game == true)
	{
		int x_head_snake2 = page[0].size() - x_head_snake1;
		int y_head_snake2 = page.size() - y_head_snake1;

		for(int i = 0; i < snake_size ; i++)
		{
			snake_coordinates2.push_back(pair<int, int>(y_head_snake2, x_head_snake2 + i));
			page[y_head_snake2][x_head_snake2 + i] = snake2;
		}
	}
}

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
	
	if(move_type == "left")
		snake_coordinates.insert(snake_coordinates.begin(), pair<int, int>(y_head_snake, x_head_snake - 1));
	
	if(move_type == "right")
		snake_coordinates.insert(snake_coordinates.begin(), pair<int, int>(y_head_snake, x_head_snake + 1));
	
	if(move_type == "up")
		snake_coordinates.insert(snake_coordinates.begin(), pair<int, int>(y_head_snake - 1, x_head_snake));
	
	if(move_type == "down")
		snake_coordinates.insert(snake_coordinates.begin(), pair<int, int>(y_head_snake + 1, x_head_snake));
}

void to_corruct_move_type(string& move_type, string& last_move)
{
	// RIGHT
	if(move_type == "left" and last_move == "right")
	{
		move_type = "right";
		return;
	}
	
	// LEFT
	if(move_type == "right" and last_move == "left")
	{
		move_type = "left";
		return;
	}

	// DOWN
	if(move_type == "up" and last_move == "down")
	{
		move_type = "down";
		return;
	}

	// UP
	if(move_type == "down" and last_move == "up")
	{
		move_type = "up";
		return;
	}
	last_move = move_type;
}

void clear_page_from_snake(vector<vector<string>>& page, string& empty, string& snake1, string& snake2)
{
	for(size_t i = 0; i < page.size(); i++)
	{
		for(size_t j = 0; j < page[i].size(); j++)
		{	
			if(page[i][j] == snake1 or page[i][j] == snake2)
				page[i][j] = empty;
		}
	}
}

void print_page(vector<vector<string>>& page, int score1, int score2, bool two_player, string& snake1, string& snake2, string& food)
{
	system("clear");

	if(two_player == true)
	{
		cout << BOLDBLUE << "player 1" << "\t\t\t\t" << BOLDRED <<"player 2" << RESET <<endl;
		cout << BOLDBLUE <<"score : " << score1 << "\t\t\t\t" << BOLDRED <<"score : " << score2 << RESET <<endl;
	}
	else
	{
		cout << BOLDBLUE << "player 1" << RESET << endl;
		cout << BOLDBLUE <<"score : " << score1 << RESET << endl;
	}
	
	for(size_t i = 0; i < page.size(); i++)
	{
		for(size_t j = 0; j < page[i].size(); j++)
		{
			if(page[i][j] == snake1)	
				cout << BOLDBLUE << page[i][j] << RESET << " " ;

			else if (page[i][j] == snake2)
				cout << BOLDRED << page[i][j] << RESET << " " ;

			else if (page[i][j] == food)
				cout << BOLDYELLOW << page[i][j] << RESET << " " ;

			else
				cout << page[i][j] << " ";
		}
		cout << endl;
	}
}

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

void add_score(int& score, int value)
{
	score += value;
}

bool check_crash_to_self_body(vector<pair<int, int>>& snake_coordinates)
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

void message_game_over(string player)
{
	cout << endl;
	if(player == "EQUAL")
		cout << "\t\t\tEQUAL GAME\t\t\t" << endl;
	else	 
		cout << "\t\t\t" << player << " GAME OVER\t\t\t" << endl;
}

void GET_NOT_SCORE(vector<pair<int, int>>& snake_coordinates)
{
	snake_coordinates.pop_back();	
}

void read_input(string& move_type1,string& move_type2, atomic<bool>& END_GAME)
{
	while(!END_GAME)
	{	
		// Black magic to prevent Linux from buffering keystrokes.
		struct termios t;
		tcgetattr(STDIN_FILENO, &t);
		t.c_lflag &= ~ICANON;
		tcsetattr(STDIN_FILENO, TCSANOW, &t);

		char input_char1, input_char2, input_char3;		
		cin >> input_char1;
		
		// PLAYER 2
		if(input_char1 == 'w' or input_char1 == 's' or input_char1 == 'd' or input_char1 == 'a')
			change_move_type_player2(move_type2, input_char1);

		// PLAYER 1
		if(input_char1 == 27)
		{	
			cin >> input_char2;
			cin >> input_char3;
			if (input_char2 == 91) 
				change_move_type_player1(move_type1, input_char3);
		}
		//this_thread::sleep_for(chrono::milliseconds(EASY));
	}
}
			
void menu(bool& two_player_game)
{
	string num = "0";
	while(num != "1" and num != "2")
	{
		system("clear");
		cout << "********(" << "MENU" << ")********" << endl;
		cout << "1.one player game " << endl;
		cout << "2.two player game " << endl;
		cout << "**********************" << endl;
		cout << "Select the number you want : ";
		cin >> num;
		if(num == "2")
			two_player_game = true;
	}
}

bool check_crash_together_body(int x_head_snake, int y_head_snake, vector<pair<int, int>>& snake_coordinates)
{
	for(size_t i = 1 ; i < snake_coordinates.size() ; i++)
	{
		if(x_head_snake == snake_coordinates[i].second and y_head_snake == snake_coordinates[i].first)
		{	
			return true;
		}
	}
	return false;
}

bool check_crash_head_by_head(vector<pair<int, int>>& snake_coordinates1, vector<pair<int, int>>& snake_coordinates2)
{
	if(snake_coordinates1[0].first == snake_coordinates2[0].first and snake_coordinates1[0].second == snake_coordinates2[0].second)
		return true;
	return false;
}

string loser_crash_head_by_head(int score1, int score2)
{
	if(score1 == score2)
		return string("EQUAL");
	if(score1 > score2)
		return string("PLAYER 2");
	else 
		return string("PLAYER 1"); 
}

void GAME_OVER(string player, atomic<bool>& end_game, thread& thread_for_read_input)
{ 
	message_game_over(player);
	end_game = true;
	thread_for_read_input.join();							
}

void GET_SCORE(vector<vector<string>>& page, vector<pair<int, int>>& food_coordinates, string& empty, string& food, int& score, int val)
{
	add_score(score, val);
	insert_food_in_page(page, food_coordinates, empty, food, 1);
}

int main()
{
	srand(time(0));

	// Configure setting snake game
	atomic<bool> END_GAME {false};
	vector<pair<int, int>> snake_coordinates_player1;
	vector<pair<int, int>> snake_coordinates_player2;
	vector<pair<int, int>> food_coordinates;
	vector<vector<string>> page;
	string move_type_player1 = "left"; 
	string move_type_player2 = "left";
	string last_move1 = "left"; 
	string last_move2 = "left";
	string margins = "#";
	string empty = ".";
	string snake1 = "+";
	string snake2 = "O";
	string food = "*";
	int lenght_page = 30;
	int value_score = 1;
	int width_page = 20;
 	int count_food = 1;
	int snake_size = 5;
	int score1 = 0;
	int score2 = 0;
	bool two_player_game = false;

	menu(two_player_game);
	
	// Initialize Primitive Game Page
	initialize_page(page, lenght_page, width_page, empty);
	initialize_margins_page(page, margins);
	initialize_snake_in_page(page, snake_coordinates_player1, snake1, snake_coordinates_player2, snake2, snake_size, two_player_game);
	insert_food_in_page(page, food_coordinates, empty, food, count_food);
	
	// Primitive Print
	print_page(page, score1, score2, two_player_game, snake1, snake2, food);

	thread thread_for_read_input(read_input, ref(move_type_player1), ref(move_type_player2), ref(END_GAME)); 


	// Each time the snake moves
	while(true)
	{
		this_thread::sleep_for(chrono::milliseconds(EASY));

		clear_page_from_snake(page, empty, snake1, snake2);
		
		to_corruct_move_type(move_type_player1, last_move1);
		move(snake_coordinates_player1, move_type_player1);
		
		handle_crash_wall(page, snake_coordinates_player1);

		insert_snake_in_page(page, snake_coordinates_player1, snake1);

		if(check_crash_to_self_body(snake_coordinates_player1))
		{
			GAME_OVER("PLAYER 1", END_GAME, thread_for_read_input);	
			return 0;
		}

		if(check_get_score(snake_coordinates_player1, food_coordinates))
			GET_SCORE(page, food_coordinates, empty, food, score1, value_score);
		else
			GET_NOT_SCORE(snake_coordinates_player1);
		
		if(two_player_game == true)
		{
			to_corruct_move_type(move_type_player2, last_move2);
			move(snake_coordinates_player2, move_type_player2);
			
			handle_crash_wall(page, snake_coordinates_player2);

			insert_snake_in_page(page, snake_coordinates_player2, snake2);

			if(check_crash_to_self_body(snake_coordinates_player2))
			{
				GAME_OVER("PLAYER 2", END_GAME, thread_for_read_input);	
				return 0;
			}

			// PLAYER 1
			if(check_crash_together_body(snake_coordinates_player1[0].second, snake_coordinates_player1[0].first, snake_coordinates_player2))
			{
				GAME_OVER("PLAYER 1", END_GAME, thread_for_read_input);	
				return 0;
			}
			
			// PLAYER 2
			if(check_crash_together_body(snake_coordinates_player2[0].second, snake_coordinates_player2[0].first, snake_coordinates_player1))
			{
				GAME_OVER("PLAYER 2", END_GAME, thread_for_read_input);	
				return 0;
			}

			if(check_crash_head_by_head(snake_coordinates_player1, snake_coordinates_player2))
			{
				GAME_OVER(loser_crash_head_by_head(score1, score2), END_GAME, thread_for_read_input);	
				return 0;
			}	

			if(check_get_score(snake_coordinates_player2, food_coordinates))
				GET_SCORE(page, food_coordinates, empty, food, score2, value_score);
			else
				GET_NOT_SCORE(snake_coordinates_player2);			
		}

		print_page(page, score1, score2, two_player_game, snake1, snake2, food);
	}
}
