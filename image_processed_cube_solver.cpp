#include <iostream>
#include <fstream>
#include "pyraminx_solver_functions.cpp"
using namespace std;
int sticker_colors[4][9]; //Pyraminx array
//String of move names
string moves[16]={"U ", "U' ", "L ", "L' ","R ", "R' ", "B ","B' ","u ","u' ","l ","l' ","r ","r' ","b ","b' "};
int main()
{
    /*
The main function reads test.txt and stores the data in sticker_colors.
It then uses the functions in pyraminx_solver_functions.cpp to generate the shortest solution. 
The shortest solution is then printed and stored in solved_moves.txt
*/
    //Code to receive test.txt Explained in algorithm_finder.cpp
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
        ofstream ofile("solved_moves.txt");
        ofile << "";
        ofile.close();
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