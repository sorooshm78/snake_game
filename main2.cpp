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

#define RESET       "\033[0m"
#define BLACK       "\033[30m"      
#define RED         "\033[31m"     
#define GREEN       "\033[32m"     
#define YELLOW      "\033[33m"     
#define BLUE        "\033[34m"     
#define MAGENTA     "\033[35m"     
#define CYAN        "\033[36m"     
#define WHITE       "\033[37m"     
#define BOLDBLACK   "\033[1m\033[30m"     
#define BOLDRED     "\033[1m\033[31m"     
#define BOLDGREEN   "\033[1m\033[32m"     
#define BOLDYELLOW  "\033[1m\033[33m"    
#define BOLDBLUE    "\033[1m\033[34m"     
#define BOLDMAGENTA "\033[1m\033[35m"    
#define BOLDCYAN    "\033[1m\033[36m"     
#define BOLDWHITE   "\033[1m\033[37m"      

#define EASY 300
#define MEDIUM 200
#define HARD 100

using namespace std;

class page;
class food;
class snake;

//////////////////////////////////////////////////////////////////

class food
{
public:
	bool is_this_the_coordinates_of_food(int x, int y);
	void add_food(int x, int y);
	void delete_food(int x, int y);
	int get_count(){ return count;}
	char get_shape(){ return shape;}
	string get_color(){ return color;}

private:
	vector<pair<int, int>> coordinates;
	int count = 1;
	char shape = '*';
	string color = BOLDYELLOW; 
};

void food::delete_food(int x, int y)
{
	for(size_t i = 0; i < coordinates.size(); i++)
    {
        if(x == coordinates[i].first and y == coordinates[i].second)
            coordinates.erase(coordinates.begin() + i);
    }
}

void food::add_food(int x, int y)
{
	coordinates.push_back(pair<int, int>(x, y));
}

bool food::is_this_the_coordinates_of_food(int x, int y)
{
	for(int i = 0; i < coordinates.size(); i++)
	{
		if(x == coordinates[i].first and y == coordinates[i].second)
			return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////

class snake
{
public:
	snake(int x, int y, string name, string color);
	bool check_crash_to_self_body();
	void GET_POINT();
	void GET_NOT_POINT();
	void handle_crash_wall(int lenght, int width);
	void move(string& move_type);
	void to_corruct_move_type(string& move_type);
	bool is_this_the_coordinates_of_snake(int x, int y);
	int get_score() {return score;}
	char get_shape() {return shape;}
	int get_size() {return size;}
	int get_x_head() {return coordinates[0].first;}
	int get_y_head() {return coordinates[0].second;}
	string get_color() {return color;}
	string get_name() {return name;}
	
private:
	vector<pair<int, int>> coordinates;
	char shape = '+';
	int size = 5;
	int score = 0;
	int food_value = 1;
	string last_move = "left";
	string name = nullptr;
	string color = BOLDBLUE;
};

snake::snake(int x, int y, string name, string color)
:color(color)
,name(name)
{
	for(int i = 0; i < size; i++)
	{
		coordinates.push_back(pair<int, int>(x + i, y));
	}
}

bool snake::is_this_the_coordinates_of_snake(int x, int y)
{
	for(int i = 0; i < coordinates.size(); i++)
	{
		if(x == coordinates[i].first and y == coordinates[i].second)
			return true;
	}
	return false; 
}

bool snake::check_crash_to_self_body()
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

void snake::GET_POINT()
{
	score += food_value;
	size ++;
}

void snake::GET_NOT_POINT()
{
	coordinates.pop_back();
}

void snake::handle_crash_wall(int lenght, int width) 
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


void snake::move(string& move_type)
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

void snake::to_corruct_move_type(string& move_type)
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

//////////////////////////////////////////////////////////////////

class page
{
public:
	page(snake *Snake, food *Food);
	void print();
	bool check_corruct_food_coordinates(int x_food, int y_food);
	void message_game_over(string player);
	void GAME_OVER(string player, atomic<bool>& end_game, thread& thread_for_read_input);
	bool check_eat_food();
	void insert_food(int count); 
	int get_lenght(){ return lenght;}
	int get_width(){ return width;}

private:
	snake *s = nullptr;
	food *f = nullptr;
	int lenght = 30;
	int width = 20;
	char margins_shape = '#';
	char empty_shape = '.';
};

page::page(snake *Snake, food *Food)
:s(Snake),f(Food)
{
}	 

void page::print()
{
	system("clear");
	cout << s->get_color() << s->get_name() << RESET << endl;
	cout << s->get_color() << "score: " << s->get_score() << RESET << endl;

	for(int y = 0; y < width; y++)
	{
		for(int x = 0; x < lenght; x++)
		{
			bool print_empty = true;
			// Margins
			if(x == 0 or y == 0 or x == lenght - 1 or y == width - 1)
			{
				cout << margins_shape << " ";
				continue;
			}

			// Snake	
			if(s->is_this_the_coordinates_of_snake(x, y))
			{	
				cout << s->get_color() << s->get_shape() << RESET << " ";			
				continue;
			}

			// Food
			if(f->is_this_the_coordinates_of_food(x, y))
			{	
				cout << f->get_color() << f->get_shape() << RESET << " ";			
				continue;
			}

			cout << empty_shape << " ";
		}
		cout << endl;
	}
}

void page::insert_food(int count) 
{
	for(int i = 0; i < count; i++)
	{
        while(true)
        {
			int x_food = rand() % (lenght - 3) + 2;
            int y_food = rand() % (width - 3) + 2;
							
			if(!s->is_this_the_coordinates_of_snake(x_food, y_food))
	        {	
				f->add_food(x_food, y_food);
				break;
			}
        }			
	}
}

void page::message_game_over(string player)
{
    cout << endl;
    if(player == "EQUAL")
        cout << "\t\t\tEQUAL GAME\t\t\t" << endl;
    else     
        cout << "\t\t\t" << player << " GAME OVER\t\t\t" << endl;
}

void page::GAME_OVER(string player, atomic<bool>& end_game, thread& thread_for_read_input)
{
    message_game_over(player);
    end_game = true;
    thread_for_read_input.join();
}

bool page::check_eat_food()
{
    int x_head_snake = s->get_x_head();
    int y_head_snake = s->get_y_head();

	if(f->is_this_the_coordinates_of_food(x_head_snake, y_head_snake))
	{
		f->delete_food(x_head_snake, y_head_snake);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////

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

    // Setting
	atomic<bool> END_GAME {false};
	string move_type_player1 = "left";
	int LEVEL = EASY;
	snake s(15, 5, "soroosh", BOLDBLUE);
	food f;
	page p(&s, &f);

	p.insert_food(f.get_count());

	thread thread_for_read_input(read_input, ref(move_type_player1), ref(END_GAME));

    while(true)
    {
        this_thread::sleep_for(chrono::milliseconds(LEVEL));

        s.to_corruct_move_type(move_type_player1);
        s.move(move_type_player1);

        s.handle_crash_wall(p.get_lenght(), p.get_width());

	    if(s.check_crash_to_self_body())
        {
            p.GAME_OVER(s.get_name(), END_GAME, thread_for_read_input);
            return 0;
        }

        if(p.check_eat_food())
        {   
			s.GET_POINT();
			p.insert_food(1); 					
		}
        else
            s.GET_NOT_POINT();

		p.print();
	}
}
