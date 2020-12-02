#include <iostream>
#include <string>
#include <vector>

using namespace std;

void initialize_page(vector<vector<string>>& point, int lenght, int width)
{
	vector <string> x_page(lenght, ".");
	for(int i = 0; i < width; i++)
	{
		point.push_back(x_page);
	}
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
	int length_page = 20;
	int width_page = 10;
	vector <vector<string>> page;
	initialize_page(page, length_page, width_page);
	print_page(page);
}
