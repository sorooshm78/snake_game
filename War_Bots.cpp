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

#define BOT true
#define NOT_BOT false

enum Move{LEFT, RIGHT, UP, DOWN};

using namespace std;

class Page;
class Food;
class Snake;

//////////////////////////////////////////////////////////////////

class Food
{
public:
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
	Snake(int x, int y, int size, string name, char shape, string color, bool bot);
	bool check_crash_to_self_body() const;
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
	bool is_body(int x, int y) const;
	bool is_bot() const { return bot; }	

	void move(Page *page);
	virtual Move determine_direction_move_bot();
	void handle_crash_wall(Page *page);	
	bool check_eat_food(Page *page);
	void handle_eat_food(Page *page) 

//private:
	void to_corruct_direction(Move& direction);
	void add_new_head(Move& direction);
	void cut_tail();

	vector<pair<int, int>> coordinates;
	Move last_move;
	int score;
	int increase_lenght;
	const char shape;
	const int primitive_size;
	const string name;
	const string color;
	const bool bot;
};

Snake::Snake(int x, int y, int size, string name, char shape, string color, bool bot)
:color(color)
,name(name)
,increase_lenght(0)
,primitive_size(size)
,shape(shape)
,score(0)
,last_move(LEFT)
,bot(bot)
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

	return  pair<int, int>(-1, -1);
}

bool Snake::is_body(int x, int y) const 
{
	for(int i = 1; i < coordinates.size(); i++)
		if(coordinates[i].first == x and coordinates[i].second == y)
			return true;
	return false;
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

void Snake::add_new_head(Move& direction)
{
	int x_head_snake = get_x_head();
    int y_head_snake = get_y_head();

    if(direction == LEFT)
        coordinates.insert(coordinates.begin(), pair<int, int>(x_head_snake - 1, y_head_snake));

    if(direction == RIGHT)
        coordinates.insert(coordinates.begin(), pair<int, int>(x_head_snake + 1, y_head_snake));

    if(direction == UP)
        coordinates.insert(coordinates.begin(), pair<int, int>(x_head_snake, y_head_snake - 1));

    if(direction == DOWN)
        coordinates.insert(coordinates.begin(), pair<int, int>(x_head_snake, y_head_snake + 1));
}

void Snake::cut_tail()
{
	if(increase_lenght == 0)
		coordinates.pop_back();
	else if(increase_lenght != 0)
		increase_lenght --;
}

void Snake::move(Page *page)
{
	Move direction = determine_direction_move_bot();

	handle_crash_wall(page);
	handle_eat_food(page);

	to_corruct_direction(direction);
	add_new_head(direction);
	cut_tail();
}

void Snake::to_corruct_direction(Move& direction)
{
    // RIGHT
    if(direction == LEFT and last_move == RIGHT)
    {
        direction = RIGHT;
        return;
    }

    // LEFT
    if(direction == RIGHT and last_move == LEFT)
    {
        direction = LEFT;
        return;
    }

    // DOWN
    if(direction == UP and last_move == DOWN)
    {
        direction = DOWN;
        return;
    }

    // UP
    if(direction == DOWN and last_move == UP)
    {
        direction = UP;
        return;
    }
    last_move = direction;
}

void Snake::handle_crash_wall(Page *page)
{
    // Wall left
    if(get_x_head() == page->margins_left)
		change_x_head(page->margins_right - 1);

    // Wall right
    if(get_x_head() == page->margins_right)
		change_x_head(page->margins_left + 1);

    // Wall up
    if(get_y_head() == page->margins_up)
		change_y_head(page->margins_down - 1);

    // Wall down
    if(get_y_head() == page->margins_down)
		change_y_head(page->margins_up + 1);
}

bool Snake::check_eat_food(Page *page) 
{
	for(int i = 0; i < page->foods.size(); i++)
		if(get_x_head() == page->foods[i]->get_x() and get_y_head() == page->foods[i]->get_y())	
			return true;
	return false;
}

void Snake::handle_eat_food(Page *page) 
{
	for(int i = 0; i < page->foods.size(); i++)
	{
		if(get_x_head() == page->foods[i]->get_x() and get_y_head() == page->foods[i]->get_y())
		{	
			increase_size(page->foods[i]->get_val());
			page->insert_new_food(page->foods[i]);
		}
	}
}


//////////////////////////////////////////////////////////////////

class Sbot : public Snake
{
public:
	Move determine_direction_move_bot_soroosh();
	Sbot(int x, int y, int size, string name, char shape, string color, bool bot);
	void print(){cout << name << endl;}
};

Move Page::determine_direction_move_bot()
{
	vector<Move> candidate;
	for(int direction = Move(LEFT); direction <= Move(DOWN); direction++)
	{
		pair<int, int> next = snake->next_move_coordinates(Move(direction));
		Snake s(next.first, next.second, 5, snake->get_name(), '+', RED, BOT);	
		Snake *virtual_snake = &s;

		handle_crash_wall(virtual_snake);

		if(snake->is_coordinates(virtual_snake->get_x_head(), virtual_snake->get_y_head()))
			continue;

		if(check_crash_to_another_snakes_body(virtual_snake))
			continue;

		if(check_crash_to_another_snakes_head(virtual_snake))
			continue; 
		
		if(check_eat_food(virtual_snake))
			return Move(direction);
		
		candidate.push_back(Move(direction));
	}

	if(candidate.size() == 0)
		return Move(UP);
	else	
		return candidate[rand()%candidate.size()];
}

Sbot::Sbot(int x, int y, int size, string name, char shape, string color, bool bot)
:Snake(x, y, size, name, shape, color, bot){}

//////////////////////////////////////////////////////////////////

class Page
{
public:
	Page(vector<Snake*> &snakes, vector<Food*> &foods);
	void print() const;                  
	void handle_eat_food(Snake *snake);
	void insert_new_food(Food *food); 
	int get_lenght() const { return lenght;}
	int get_width() const { return width;}
	void move_once(Move& direction_1, Move& direction_2, bool& END_GAME);
	bool is_coordinates_snakes(int x_food, int y_food);
	int find_index_snake(Snake *snake);
	void check_end_game(bool &end_game);
	Move determine_direction_move_bot_soroosh(Snake *snake);
	Move determine_direction_move_bot_mehran(Snake *snake);
	bool check_crash_to_another_snakes_body(Snake *snake);
	bool check_crash_to_another_snakes_head(Snake *snake);
	void check_and_handle_crash_to_another_snakes_head(Snake *snake); 			
//	int get_margins_up{return margins_up;}
//	int get_margins_down{return margins_down;}
//	int get_margins_right{return margins_right;}
//	int get_margins_left{return margins_left;}

private:
	vector<Snake*> snakes;
	vector<Food*> foods;
	const int lenght;
	const int width;
	const char margins_shape;
	const char empty_shape;
	const int margins_up;
    const int margins_down;
    const int margins_left;
    const int margins_right;
	string message;
};

Page::Page(vector<Snake*>& snakes, vector<Food*>& foods)
:snakes(snakes)
,foods(foods)
,lenght(30)
,width(20)
,margins_shape('#')
,empty_shape('.')
,margins_up(0)
,margins_down(width - 1)
,margins_left(0)
,margins_right(lenght - 1)
{
}	 

int Page::find_index_snake(Snake *snake)
{
	for(int i = 0; i < snakes.size(); i++)
		if(snakes[i] == snake)
			return i;
	return -1;
}

bool Page::check_crash_to_another_snakes_head(Snake *snake)
{
	for(int i = 0; i < snakes.size(); i++)
	{
		if(snakes[i]->get_name() == snake->get_name())
			continue;

		if(snakes[i]->get_x_head() == snake->get_x_head() and snakes[i]->get_y_head() == snake->get_y_head())
			return true;
	}
	return false;
}

void Page::check_and_handle_crash_to_another_snakes_head(Snake *snake)
{
	for(int i = 0; i < snakes.size(); i++)
	{
		if(snakes[i]->get_name() == snake->get_name())
			continue;

		if(snakes[i]->get_x_head() == snake->get_x_head() and snakes[i]->get_y_head() == snake->get_y_head())
		{
			if(snakes[i]->get_score() == snake->get_score())
			{
				snakes.erase(snakes.begin() + i);
				snakes.erase(snakes.begin() + find_index_snake(snake));
				continue;
			}
			if(snakes[i]->get_score() > snake->get_score())
			{
				snakes.erase(snakes.begin() + find_index_snake(snake));
				continue;		
			}
			if(snakes[i]->get_score() < snake->get_score())
			{
				snakes.erase(snakes.begin() + i);		
				continue;
			}
		}
	}
}

Move Page::determine_direction_move_bot_soroosh(Snake *snake)
{
	vector<Move> candidate;
	for(int direction = Move(LEFT); direction <= Move(DOWN); direction++)
	{
		pair<int, int> next = snake->next_move_coordinates(Move(direction));
		Snake s(next.first, next.second, 5, snake->get_name(), '+', RED, BOT);	
		Snake *virtual_snake = &s;

		handle_crash_wall(virtual_snake);

		if(snake->is_coordinates(virtual_snake->get_x_head(), virtual_snake->get_y_head()))
			continue;

		if(check_crash_to_another_snakes_body(virtual_snake))
			continue;

		if(check_crash_to_another_snakes_head(virtual_snake))
			continue; 
		
		if(check_eat_food(virtual_snake))
			return Move(direction);
		
		candidate.push_back(Move(direction));
	}

	if(candidate.size() == 0)
		return Move(UP);
	else	
		return candidate[rand()%candidate.size()];
}

/////////////////////////////////////////////////////////////////////////////////////

Move Page::determine_direction_move_bot_mehran(Snake *snake)
{
	vector<Move> candidate;
	for(int direction = Move(LEFT); direction <= Move(DOWN); direction++)
	{
		pair<int, int> next = snake->next_move_coordinates(Move(direction));
		Snake s(next.first, next.second, 5, snake->get_name(), '+', RED, BOT);	
		Snake *virtual_snake = &s;

		handle_crash_wall(virtual_snake);

		if(snake->is_coordinates(virtual_snake->get_x_head(), virtual_snake->get_y_head()))
			continue;

		if(check_crash_to_another_snakes_body(virtual_snake))
			continue;

		if(check_crash_to_another_snakes_head(virtual_snake))
			continue; 
		
		if(check_eat_food(virtual_snake))
			return Move(direction);
		
		candidate.push_back(Move(direction));
	}

	if(candidate.size() == 0)
		return Move(UP);
	else	
		return candidate[rand()%candidate.size()];
}

/////////////////////////////////////////////////////////////////////////////////////

bool Page::check_crash_to_another_snakes_body(Snake *snake)
{
	for(int i = 0; i < snakes.size(); i++)
	{
		if(snakes[i]->get_name() == snake->get_name())
			continue;

		if(snakes[i]->is_body(snake->get_x_head(), snake->get_y_head()))
			return true;
	}
	return false;
}

void Page::move_once(Move& direction_1, Move& direction_2, bool& END_GAME)
{
	for(int i = 0; i < snakes.size(); i++)
	{
		snakes[i]->move(this);

/*		
		if(snakes[i]->get_name() == "Soroosh BOT 0" or snakes[i]->get_name() == "Soroosh BOT o")
			snakes[i]->move(determine_direction_move_bot_soroosh(snakes[i]));

		else if(snakes[i]->get_name() == "Mehran BOT 0" or snakes[i]->get_name() == "Mehran BOT o")
			snakes[i]->move(determine_direction_move_bot_mehran(snakes[i]));
*/
//		go to snake
//		handle_crash_wall(snakes[i]);
//		handle_eat_food(snakes[i]);


//		stay in page		
		if(snakes[i]->check_crash_to_self_body())
		{
			snakes.erase(snakes.begin() + i);
			continue;
		}
				
//		stay in page
		if(check_crash_to_another_snakes_body(snakes[i]))
		{
			snakes.erase(snakes.begin() + i);
			continue;
		}

//		saty in page
		check_and_handle_crash_to_another_snakes_head(snakes[i]);
	}

	check_end_game(END_GAME);
}

void Page::check_end_game(bool &end_game)
{
	if(snakes.size() == 1)
	{
		message = snakes[0]->get_name() +  " WIN";
		end_game = true;
	}

	else if(snakes.size() == 0)
	{
		message = "EQAUL";
		end_game = true;
	}
}


void Page::print() const
{
	system("clear");

	for(int i = 0; i < snakes.size(); i++)
			cout << snakes[i]->get_color() << snakes[i]->get_name() << " : " << snakes[i]->get_score() << RESET << endl;

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
					if(x == snakes[i]->get_x_head() and y == snakes[i]->get_y_head())
						cout << WHITE << snakes[i]->get_shape() << RESET << " ";	
		
					else
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
	cout << message << endl;
}

bool Page::is_coordinates_snakes(int x_food, int y_food)
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
		
		if(is_coordinates_snakes(x_food, y_food))
		{
			food->change_coordinates(x_food, y_food);		
			break;
		}
	}				
}

//////////////////////////////////////////////////////////////////

void menu(bool& bots)
{
    string num = "0"; 
    while(num != "1") 
    { 
        system("clear"); 
        cout << "********(" << "MENU" << ")********" << endl; 
        cout << "1.Start War bots" << endl; 
        cout << "**********************" << endl; 
        cout << "Select the number you want : "; 

        cin >> num; 
	}
	bots = true;
}

int main()
{
	srand(time(0));	

    // Setting
	bool END_GAME {false};
	Move direction_1 = LEFT;
	Move direction_2 = LEFT;
	int LEVEL = EASY;
	bool bots = false;

	vector<Snake*> snakes;
	vector<Food*> foods;

	Sbot B1(22, 3, 5, "Soroosh BOT 0", '0', BLUE, BOT);

//	Snake B1(22, 3, 5, "Soroosh BOT 0", '0', BLUE, BOT);
	Snake B2(3, 16, 5, "Soroosh BOT o", 'o', BLUE, BOT);

	Snake B3(3, 3, 5, "Mehran BOT 0", '0', GREEN, BOT);
	Snake B4(22, 16, 5, "Mehran BOT o", 'o', GREEN, BOT);

	Food f1(15, 10, 1, '1', BOLDYELLOW);
	Food f2(10, 10, 3, '3', BOLDYELLOW);
	Food f3(20, 10, 5, '5', BOLDYELLOW);

	foods.push_back(&f1);
	foods.push_back(&f2);
	foods.push_back(&f3);

	menu(bots);
	
	snakes.push_back(&B1);
	snakes.push_back(&B2);
	snakes.push_back(&B3);
	snakes.push_back(&B4);

	
	// Page setting
	Page page(snakes, foods);
	page.print();

	while(!END_GAME)
	{
        this_thread::sleep_for(chrono::milliseconds(LEVEL));
		page.move_once(direction_1, direction_2, END_GAME);
		page.print();	
	}

}
