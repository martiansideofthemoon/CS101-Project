#include <iostream>
#include <fstream>
#include "pyraminx_solver_functions.cpp"
using namespace std;
int sticker_colors[4][9];
string moves[16]={"U ", "U' ", "L ", "L' ","R ", "R' ", "B ","B' ","u ","u' ","l ","l' ","r ","r' ","b ","b' "};
int main()
{
	int number_characters=0;
	while (number_characters<40)
	{
	number_characters=0;
	ifstream infile; 
    infile.open("test.txt"); 
    char c;
    infile.get(c);
    int count=0;
    while (count<4)
    {
    	if (c=='\n') count++;
    	number_characters++;
    	infile.get(c);
    	//cout << "abc" << endl;
    	
    }
	infile.close();
	}
	ifstream infile; 
    infile.open("test.txt"); 
    char c;
    for (int i=0;i<4;i++)
    {
    	for (int j=0;j<9;j++)
    	{
    		infile.get(c);
    		sticker_colors[i][j]=c-'0';
    	}
    	infile.get(c);
    }
    cout << "Input received, finding fastest solution..." << endl;
    int *sol=execute(sticker_colors);
    bool sol_found=solution_found();
	if (sol_found==false)
	{
		cout << "Solution not found. Please enter valid input. " << endl;
	}
	else
	{
	int sol_len=sol_length();
	ofstream ofile("solved_moves.txt");
	for (int i=0;i<sol_len;i++)
	{
		cout << moves[sol[i]];
		ofile << sol[i] << " ";
	}
	ofile.close();
	cout << endl;
	}
}