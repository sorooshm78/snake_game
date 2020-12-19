#include <iostream>
#include <vector>
#include <string>

using namespace std;

class snake
{
public:
snake(int x, int y)
{
	for(int i = 0; i < primitive_size; i++)
	{
		coordinates.push_back(pair<int, int>(x + 1, y));
	}
}

//private:
	vector<pair<int, int>> coordinates;
	char shape = '*';
	int primitive_size = 4;
	int score = 0;
};

class page
{
public:
void print(snake *s)
{
	for(int y = 0; y < width; y++)
	{
		for(int x = 0; x < lenght; x++)
		{
			bool flag = true;
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
					flag = false;
					cout << s->shape << " ";
					//break;
				}
			}

			if(flag)
			{
				cout << empty_shape << " ";
			}
		}
		cout << endl;
	}
}

private:
	int lenght = 10;
	int width = 5;
	char margins_shape = '#';
	char empty_shape = '.';
};

class food
{
public:
private:
	vector<pair<int, int>> coordinates;
	int count = 1;
	char shape = '*';
};

int main()
{
	snake s(2, 2);
	page p;
	p.print(&s);
}
