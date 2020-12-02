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
	string page_inside = ".";
	string page_margins = "#";
	vector <vector<string>> page;

	initialize_page(page, length_page, width_page, page_inside);
	initialize_margins_page(page, page_margins);
	print_page(page);
}
