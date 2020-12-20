#include <iostream>
#include <vector>
#include <string>
#include <time.h>
#include <stdlib.h>

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

void move()
{


}

//private:
	vector<pair<int, int>> coordinates;
	char shape = '+';
	int primitive_size = 5;
	int score = 0;
	string move_type = "left";
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

int main()
{
	srand(time(0));	

	snake s(5, 5);
	food f;
	page p;
	insert_food(&p, &s, &f);
	p.print(&s, &f);
}
