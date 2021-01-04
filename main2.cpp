#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include <termios.h>

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

enum Move{LEFT, RIGHT, UP, DOWN};

using namespace std;

class Page;
class Food;
class Snake;

//////////////////////////////////////////////////////////////////

class Food
{
public:
	Food(int x, int y);
	Food(int x, int y, int val, char shape, string color);
	bool is_coordinates(int x, int y) const;
	void change_coordinates(int x, int y);
	int get_x() const { return x;}
	int get_y() const { return y;}
	char get_shape() const { return shape;}
	int get_val() const { return value;}
	string get_color() const { return color;}

private:
	int x;
	int y;
	const int value;
	const char shape;
	const string color;
};

Food::Food(int x, int y)
:x(x)
,y(y)
,value(1)
,shape('*')
,color(BOLDYELLOW)
{
}

Food::Food(int x, int y, int val, char shape, string color)
:x(x)
,y(y)
,value(val)
,shape(shape)
,color(color)
{
}

bool Food::is_coordinates(int x, int y) const
{
	if(this->x == x and this->y == y)
		return true;
	return false;
}

void Food::change_coordinates(int x, int y)
{
	this->x = x;
	this->y = y;
}

//////////////////////////////////////////////////////////////////

class Snake
{
public:
	Snake(int x, int y);
	Snake(int x, int y, int size, string name, char shape, string color);
	bool check_crash_to_self_body() const;
	void move(Move move_type);
	bool is_coordinates(int x, int y) const;
	void change_x_head(int x);
	void change_y_head(int y);
	void increase_size(int val);
	int get_score() const {return score;}
	char get_shape() const {return shape;}
	int get_size() const {return coordinates.size();}
	int get_x_head() const {return coordinates[0].first;}
	int get_y_head() const {return coordinates[0].second;}
	string get_color() const {return color;}
	string get_name() const {return name;}
	pair<int, int> next_move_coordinates(Move move);
	
private:
	void to_corruct_move_type(Move& move_type);
	void add_new_head(Move& move_type);
	void cut_tail();

	vector<pair<int, int>> coordinates;
	Move last_move;
	int score;
	int increase_lenght;
	const char shape;
	const int primitive_size;
	const string name;
	const string color;
};

Snake::Snake(int x, int y, int size, string name, char shape, string color)
:color(color)
,name(name)
,increase_lenght(0)
,primitive_size(size)
,shape(shape)
,score(0)
,last_move(LEFT)
{
	for(int i = 0; i < primitive_size; i++)
	{
		coordinates.push_back(pair<int, int>(x + i, y));
	}
}

Snake::Snake(int x, int y)
:color(BOLDBLUE)
,name("PLAYER")
,increase_lenght(0)
,primitive_size(5)
,shape('+')
,score(0)
,last_move(LEFT)
{
	for(int i = 0; i < primitive_size; i++)
	{
		coordinates.push_back(pair<int, int>(x + i, y));
	}
}

pair<int, int> Snake::next_move_coordinates(Move direction)
{
	int x_head_snake = get_x_head();
    int y_head_snake = get_y_head();

    if(direction == LEFT)
	 	return pair<int, int>(x_head_snake - 1, y_head_snake);

    if(direction == RIGHT)
        return pair<int, int>(x_head_snake + 1, y_head_snake);

    if(direction == UP)
        return pair<int, int>(x_head_snake, y_head_snake - 1);

    if(direction == DOWN)
        return pair<int, int>(x_head_snake, y_head_snake + 1);
}

void Snake::increase_size(int val)
{
	increase_lenght += val;
	score += val;
}

void Snake::change_x_head(int x)
{
	coordinates[0].first = x;
}

void Snake::change_y_head(int y)
{
	coordinates[0].second = y;
}

bool Snake::is_coordinates(int x, int y) const
{
	for(int i = 0; i < coordinates.size(); i++)
	{
		if(x == coordinates[i].first and y == coordinates[i].second)
			return true;
	}
	return false; 
}

bool Snake::check_crash_to_self_body() const
{
    int x_head_snake = get_x_head();
    int y_head_snake = get_y_head();

    for(size_t i = 1; i < coordinates.size(); i++)
    {
        if(x_head_snake == coordinates[i].first and y_head_snake == coordinates[i].second)
            return true;
    }
    return false;
}

void Snake::add_new_head(Move& move_type)
{
	int x_head_snake = get_x_head();
    int y_head_snake = get_y_head();

    if(move_type == LEFT)
        coordinates.insert(coordinates.begin(), pair<int, int>(x_head_snake - 1, y_head_snake));

    if(move_type == RIGHT)
        coordinates.insert(coordinates.begin(), pair<int, int>(x_head_snake + 1, y_head_snake));

    if(move_type == UP)
        coordinates.insert(coordinates.begin(), pair<int, int>(x_head_snake, y_head_snake - 1));

    if(move_type == DOWN)
        coordinates.insert(coordinates.begin(), pair<int, int>(x_head_snake, y_head_snake + 1));
}

void Snake::cut_tail()
{
	if(increase_lenght == 0)
		coordinates.pop_back();
	else if(increase_lenght != 0)
		increase_lenght --;
}

void Snake::move(Move move_type)
{
	to_corruct_move_type(move_type);
	add_new_head(move_type);
	cut_tail();
}

void Snake::to_corruct_move_type(Move& move_type)
{
    // RIGHT
    if(move_type == LEFT and last_move == RIGHT)
    {
        move_type = RIGHT;
        return;
    }

    // LEFT
    if(move_type == RIGHT and last_move == LEFT)
    {
        move_type = LEFT;
        return;
    }

    // DOWN
    if(move_type == UP and last_move == DOWN)
    {
        move_type = DOWN;
        return;
    }

    // UP
    if(move_type == DOWN and last_move == UP)
    {
        move_type = UP;
        return;
    }
    last_move = move_type;
}

//////////////////////////////////////////////////////////////////

class Page
{
public:
	Page(vector<Snake*> &snakes, vector<Food*> &foods);
	void print() const;
	void handle_eat_food(Snake *snake);
	void message_game_over(string player) const;
	void game_over(string player, bool& end_game, thread& thread_for_read_input);
	void insert_new_food(Food *food); 
	int get_lenght() const { return lenght;}
	int get_width() const { return width;}
	void move_once(Move& move_type, bool& END_GAME, thread& thread_for_read_input);
	void handle_crash_wall(Snake *snake);	
	bool is_inside_snakes(int x_food, int y_food);
	Move define_direction_move(Snake *snake);

private:
	vector<Snake*> snakes;
	vector<Food*> foods;
	const int lenght;
	const int width;
	const char margins_shape;
	const char empty_shape;
};

Page::Page(vector<Snake*>& snakes, vector<Food*>& foods)
:snakes(snakes)
,foods(foods)
,lenght(30)
,width(20)
,margins_shape('#')
,empty_shape('.')
{
}	 

/*bool Page::is_get_score_next_move(Snake *snake, Move direction)
{
	for(int i = 0; i < foods.szie(); i++)
	{
		if(snake->get_x_head() == foods[i]->get_x() and snake->get_y_head() == foods[i]->get_y())

	}
}*/

Move Page::define_direction_move(Snake *snake)
{
	vector<Move> candidate;
	for(int direction = Move(LEFT); direction <= Move(DOWN); direction++)
	{
		pair<int, int> next = snake->next_move_coordinates(Move(direction));

		if(snake->is_coordinates(next.first, next.second))
			continue;
		
/*		for(int i = 0; i < foods.size(); i++)
		{
			if(directoin 		


		}
*/
		candidate.push_back(Move(direction));
	}
	return candidate[rand()%candidate.size()];
}

void Page::move_once(Move& move_type, bool& END_GAME, thread& thread_for_read_input)
{
	for(int i = 0; i < snakes.size(); i++)
	{
//		snakes[i]->move(move_type);	

		snakes[i]->move(define_direction_move(snakes[i]));	
		handle_crash_wall(snakes[i]);
		handle_eat_food(snakes[i]);
		if(snakes[i]->check_crash_to_self_body())
			game_over(snakes[i]->get_name(), END_GAME, thread_for_read_input);
	}
}

void Page::handle_eat_food(Snake *snake) 
{
	for(int i = 0; i < foods.size(); i++)
	{
		if(snake->get_x_head() == foods[i]->get_x() and snake->get_y_head() == foods[i]->get_y())
		{	
			snake->increase_size(foods[i]->get_val());
			insert_new_food(foods[i]);
		}
	}
}

void Page::handle_crash_wall(Snake *snake)
{
    int margins_up = 0;
    int margins_down = width - 1;
    int margins_left = 0;
    int margins_right = lenght - 1;

    // Wall left
    if(snake->get_x_head() == margins_left)
		snake->change_x_head(margins_right - 1);

    // Wall right
    if(snake->get_x_head() == margins_right)
		snake->change_x_head(margins_left + 1);

    // Wall up
    if(snake->get_y_head() == margins_up)
		snake->change_y_head(margins_down - 1);

    // Wall down
    if(snake->get_y_head() == margins_down)
		snake->change_y_head(margins_up + 1);
}

void Page::print() const
{
	system("clear");
	for(int i = 0; i < snakes.size(); i++)
			cout << snakes[0]->get_color() << snakes[i]->get_name() << " : " << snakes[i]->get_score() << RESET << endl;

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

			// Snakes
			for(int i = 0; i < snakes.size(); i++)
			{
				if(snakes[i]->is_coordinates(x, y))
				{	
					cout << snakes[i]->get_color() << snakes[i]->get_shape() << RESET << " ";			
					print_empty = false;
					break;
				}
			}
	
			// Foods
			for(int i = 0; i < foods.size(); i++)
			{
				if(foods[i]->is_coordinates(x, y))
				{	
					cout << foods[i]->get_color() << foods[i]->get_shape() << RESET << " ";			
					print_empty = false;
					break;
				}
			}
	
			if(print_empty)
				cout << empty_shape << " ";
		}
		cout << endl;
	}
}

bool Page::is_inside_snakes(int x_food, int y_food)
{
	for(int i = 0; i < snakes.size(); i++)
	{				
		if(snakes[i]->is_coordinates(x_food, y_food))
			return false;	
	}
	return true;
}

void Page::insert_new_food(Food *food) 
{
	while(true)
	{
		int x_food = rand() % (lenght - 3) + 2;
		int y_food = rand() % (width - 3) + 2;
		
		if(is_inside_snakes(x_food, y_food))
		{
			food->change_coordinates(x_food, y_food);		
			break;
		}
	}				
}

void Page::message_game_over(string player) const
{
    cout << endl;
    if(player == "EQUAL")
        cout << "\t\t\tEQUAL GAME\t\t\t" << endl;
    else     
        cout << "\t\t\t" << player << " GAME OVER\t\t\t" << endl;
}

void Page::game_over(string player, bool& end_game, thread& thread_for_read_input)
{
    message_game_over(player);
    end_game = true;
    thread_for_read_input.join();
}

//////////////////////////////////////////////////////////////////

void change_move_type(Move& move_type, char input_move_type)
{
    if(input_move_type == KEY_UP)
        move_type = UP;

    if(input_move_type == KEY_DOWN)
        move_type = DOWN;

    if(input_move_type == KEY_LEFT)
        move_type = LEFT;

    if(input_move_type == KEY_RIGHT)
        move_type = RIGHT;
}

void read_input(Move& move_type, bool& END_GAME)
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
                change_move_type(move_type, input_char3);
        }
    }
}

int main()
{
	srand(time(0));	

    // Setting
	bool END_GAME {false};
	Move move_type = LEFT;
	int LEVEL = EASY;

	// Object food game
	vector<Snake*> snakes;
	
	Snake s1(15, 5);
//	Snake s2(15, 15, 5, "sm", 'o', YELLOW);

	snakes.push_back(&s1);
//	snakes.push_back(&s2);


	// Object food game
	vector<Food*> foods;

	Food f1(15, 10);
	Food f2(10, 10, 3, '3', BOLDWHITE);
	Food f3(20, 10, 5, '5', BOLDMAGENTA);

	foods.push_back(&f1);
	foods.push_back(&f2);
	foods.push_back(&f3);
	
	// Page setting
	Page page(snakes, foods);
		
	thread thread_for_read_input(read_input, ref(move_type), ref(END_GAME));

	while(!END_GAME)
	{
        this_thread::sleep_for(chrono::milliseconds(LEVEL));
		page.move_once(move_type, END_GAME, thread_for_read_input);
		if(!END_GAME)
			page.print();	
	}
}
