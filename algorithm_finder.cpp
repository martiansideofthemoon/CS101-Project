#include <iostream>
#include <fstream>
#include "pyraminx_solver_functions.cpp"
using namespace std;
int sticker_colors[4][9];
int solved_pyraminx[4][9]= {{0,0,0,0,0,0,0,0,0},
                           {1,1,1,1,1,1,1,1,1},
                           {2,2,2,2,2,2,2,2,2},
                           {3,3,3,3,3,3,3,3,3}};
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

    
    algo_finder_config(sticker_colors);
	int *reverse_sol=execute(solved_pyraminx);
	bool sol_found=solution_found();
	if (sol_found==false)
	{
		ofstream ofile("solved_moves.txt");
		ofile << "";
		ofile.close();
		cout << "Solution not found. Please enter valid input. " << endl;
	}
	else
	{
		int sol_len=sol_length();
	//cout << endl;
	cout << "Shortest solution from the start cube state to solved cube is : ";
		
	int sol[sol_len];
	for (int i=0;i<sol_len;i++)
	{
		if (reverse_sol[sol_len-i-1]%2==0)
		sol[i]=reverse_sol[sol_len-i-1]+1;
		else
		sol[i]=reverse_sol[sol_len-i-1]-1;
	}
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