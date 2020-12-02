#include <iostream>
#include <string>
#include <vector>

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

void initialize_snake_in_page(vector<vector<string>>& page, string& snake, int snake_size)
{
	int y = page[0].size() / 2;
	int x = page.size() / 2;
	// snake head
	page[x][y] = "<";
	for(int i = 1; i < snake_size ; i++)
		page[x][y + i] = snake;
}

void print_page(vector<vector<string>>& page)
{
	for(int i = 0; i < page.size(); i++)
	{
		for(int j = 0; j < page[i].size(); j++)
		{	
			cout << page[i][j] << " " ;
		}
		cout << endl;
	}
}

int main()
{
	// Confegur setting
	int length_page = 30;
	int width_page = 20;
	string page_inside = " ";
	string page_margins = "#";
	string snake = "-";
	int snake_size = 5;
	vector <vector<string>> page;

	initialize_page(page, length_page, width_page, page_inside);
	initialize_margins_page(page, page_margins);
	initialize_snake_in_page(page, snake, snake_size);
	print_page(page);
}
