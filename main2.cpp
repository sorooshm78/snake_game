#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <stdlib.h>
#include <atomic>

using namespace std;

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

//private:
	vector<pair<int, int>> coordinates;
	char shape = '+';
	int primitive_size = 5;
	int score = 0;
	string last_move = "left";
};

class food
{
public:

//private:
	vector<pair<int, int>> coordinates;
	int count = 1;
	char shape = '*';
};

class page
{
public:
void print(snake *s, food *f)
{
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

void insert_food(page *p, snake *s, food *f)
{
	for(int i = 0; i < f->count; i++)
	{
        while(true)
        {
			int x_food = rand() % (p->get_lenght() - 3) + 2;
            int y_food = rand() % (p->get_width() - 3) + 2;
							
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

/*        // PLAYER 2
        if(input_char1 == 'w' or input_char1 == 's' or input_char1 == 'd' or input_char1 == 'a')
           change_move_type_player2(move_type2, input_char1);
*/
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


















int main()
{
	srand(time(0));	

	atomic<bool> END_GAME {false};
	string move_type_player1 = "left";
	snake s(5, 5);
	food f;
	page p;

	insert_food(&p, &s, &f);


    while(true)
    {
        this_thread::sleep_for(chrono::milliseconds(LEVEL));

        s.to_corruct_move_type(move_type_player1);
        s.move(move_type_player1);

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
	}
}
