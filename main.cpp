#include <iostream>
#include <string>
#include <vector>

using namespace std;

void initialize_page(vector<vector<string>>& point, int lenght, int width, string& inside)
{
	vector <string> x_page(lenght, inside);
	for(int i = 0; i < width; i++)
	{
		point.push_back(x_page);
	}
}

void initialize_margins_page(vector<vector<string>>& point, string& margins)
{
	int width = point.size();
	int lenght = point[0].size();

	// up margins	
	for(int i = 0; i < lenght; i++)
		point[0][i] = margins;

	// down margins
	for(int i = 0; i < lenght; i++)
		point[width - 1][i] = margins;

	// right margins
	for(int i = 0; i < width; i++)
		point[i][0] = margins;			

	// left margins
	for(int i = 0; i < width; i++)
		point[i][lenght - 1] = margins;
}

void print_page(vector<vector<string>>& point)
{
	for(int i = 0; i < point.size(); i++)
	{
		for(int j = 0; j < point[i].size(); j++)
		{	
			cout << point[i][j] << " " ;
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
