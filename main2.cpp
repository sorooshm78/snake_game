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

#define EASY 300
#define MEDIUM 200
#define HARD 100

using namespace std;

class page;
class food;
class snake;

class food
{
public:
	int get_count(){ return count;}
//private:
	vector<pair<int, int>> coordinates;
	int count = 1;
	char shape = '*';
};

class snake
{
public:
snake(int x, int y)
{
	for(int i = 0; i < primitive_size; i++)
	{
		coordinates.push_back(pair<int, int>(x + i, y));
	}
}

bool check_get_score(food *f)
{
    int x_head_snake = coordinates[0].first;
    int y_head_snake = coordinates[0].second;

    for(size_t i = 0; i < coordinates.size(); i++)
    {
        if(f->coordinates[i].first ==  x_head_snake and f->coordinates[i].second ==  y_head_snake)
        {
            // Delete food from coordinates
            f->coordinates.erase(f->coordinates.begin() + i);
            return true;
        }
    }
    return false;
}

bool check_crash_to_self_body()
{
    int x_head_snake = coordinates[0].first;
    int y_head_snake = coordinates[0].second;

    for(size_t i = 1; i < coordinates.size(); i++)
    {
        if(x_head_snake == coordinates[i].first and y_head_snake == coordinates[i].second)
            return true;
    }
    return false;
}

void GET_SCORE()
{
	score += food_value;
}

void GET_NOT_SCORE()
{
	coordinates.pop_back();
}

void handle_crash_wall(int lenght, int width) 
{
    int x_head_snake = coordinates[0].first;
    int y_head_snake = coordinates[0].second;

    int margins_up = 0;
    int margins_down = width - 1;
    int margins_left = 0;
    int margins_right = lenght - 1;

    // Wall left
    if(x_head_snake == margins_left)
         coordinates[0].first = margins_right - 1;

    // Wall right
    if(x_head_snake == margins_right)
         coordinates[0].first = margins_left + 1;

    // Wall up
    if(y_head_snake == margins_up)
         coordinates[0].second = margins_down - 1;

    // Wall down
    if(y_head_snake == margins_down)
         coordinates[0].second = margins_up + 1;
}


void move(string& move_type)
{
	int x_head_snake = coordinates[0].first;
    int y_head_snake = coordinates[0].second;

    if(move_type == "left")
        coordinates.insert(coordinates.begin(), pair<int, int>(x_head_snake - 1, y_head_snake));

    if(move_type == "right")
        coordinates.insert(coordinates.begin(), pair<int, int>(x_head_snake + 1, y_head_snake));

    if(move_type == "up")
        coordinates.insert(coordinates.begin(), pair<int, int>(x_head_snake, y_head_snake - 1));

    if(move_type == "down")
        coordinates.insert(coordinates.begin(), pair<int, int>(x_head_snake, y_head_snake + 1));
}

void to_corruct_move_type(string& move_type)
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

int get_score() {return score;}
//private:
	vector<pair<int, int>> coordinates;
	char shape = '+';
	int primitive_size = 5;
	int score = 0;
	int food_value = 1;
	string last_move = "left";
};



class page
{
public:
void print(snake *s, food *f)
{
	system("clear");
	cout << "player 1" << endl;
	cout << "score: "<< s->get_score() << endl;

	for(int y = 0; y < width; y++)
	{
		for(int x = 0; x < lenght; x++)
		{
			bool print_empty = true;
			// margins
			if(x == 0 or y == 0 or x == lenght - 1 or y == width - 1)
			{
				cout << margins_shape << " ";
				continue;
			}

			// snake
			for(int i = 0; i < s->coordinates.size(); i++)
			{
				if(x == s->coordinates[i].first and y == s->coordinates[i].second)
				{
					print_empty = false;
					cout << s->shape << " ";
					break;
				}
			}

			// food 
			for(int i = 0; i < f->coordinates.size(); i++)
			{
				if(x == f->coordinates[i].first and y == f->coordinates[i].second)
				{
					print_empty = false;
					cout << f->shape << " ";
					break;
				}
			}

			if(print_empty)
			{
				cout << empty_shape << " ";
			}
		}
		cout << endl;
	}
}

int get_lenght(){ return lenght;}
int get_width(){ return width;}

private:
	int lenght = 30;
	int width = 20;
	char margins_shape = '#';
	char empty_shape = '.';
};


bool check_corruct_coordinates(int x_food, int y_food, snake *s)
{
	for(int i = 0; i < s->coordinates.size(); i++)
	{
		if(x_food == s->coordinates[i].first and y_food == s->coordinates[i].second)
			return false;		
	}
	return true;
}

void insert_food(snake *s, food *f, int lenght, int width, int count) 
{
	for(int i = 0; i < count; i++)
	{
        while(true)
        {
			int x_food = rand() % (lenght - 3) + 2;
            int y_food = rand() % (width - 3) + 2;
							
			if(check_corruct_coordinates(x_food, y_food, s))
	        {	
				f->coordinates.push_back(pair<int, int>(x_food, y_food));
				break;
			}
        }			
	}
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

void message_game_over(string player)
{
    cout << endl;
    if(player == "EQUAL")
        cout << "\t\t\tEQUAL GAME\t\t\t" << endl;
    else     
        cout << "\t\t\t" << player << " GAME OVER\t\t\t" << endl;
}

void GAME_OVER(string player, atomic<bool>& end_game, thread& thread_for_read_input)
{
    message_game_over(player);
    end_game = true;
    thread_for_read_input.join();
}

void read_input(string& move_type1, atomic<bool>& END_GAME)
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

int main()
{
	srand(time(0));	

	atomic<bool> END_GAME {false};
	string move_type_player1 = "left";
	int LEVEL = EASY;
	snake s(5, 5);
	food f;
	page p;

	insert_food(&s, &f, p.get_lenght(), p.get_width(), f.get_count());

	thread thread_for_read_input(read_input, ref(move_type_player1), ref(END_GAME));

    while(true)
    {
        this_thread::sleep_for(chrono::milliseconds(LEVEL));

        s.to_corruct_move_type(move_type_player1);
        s.move(move_type_player1);

        s.handle_crash_wall(p.get_lenght(), p.get_width());

	    if(s.check_crash_to_self_body())
        {
            GAME_OVER("PLAYER 1", END_GAME, thread_for_read_input);
            return 0;
        }

        if(s.check_get_score(&f))
        {   
			s.GET_SCORE();
			insert_food(&s, &f, p.get_lenght(), p.get_width(), 1); 					
		}
        else
            s.GET_NOT_SCORE();

		p.print(&s, &f);
	}
}
