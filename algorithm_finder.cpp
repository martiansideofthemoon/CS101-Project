#include <iostream>
#include <fstream>
#include "pyraminx_solver_functions.cpp"
using namespace std;
int sticker_colors[4][9]; //Sticker colour array
int solved_pyraminx[4][9]= {{0,0,0,0,0,0,0,0,0},
                           {1,1,1,1,1,1,1,1,1},
                           {2,2,2,2,2,2,2,2,2},
                           {3,3,3,3,3,3,3,3,3}}; //Sticker colours in a solved Pyraminx
string moves[16]={"U ", "U' ", "L ", "L' ","R ", "R' ", "B ","B' ","u ","u' ","l ","l' ","r ","r' ","b ","b' "}; //Sequence of moves

int main()
{
	/*
The main function reads test.txt and stores the data in sticker_colors.
It then uses the functions in pyraminx_solver_functions.cpp to generate the shortest solution. 
The shortest solution is then printed and stored in solved_moves.txt
*/
	//The code below is used to read text.txt
	int number_characters=0;
	while (number_characters<40) //Check valid length of file. Loop exits when file has atleast 40 characters
	{
	number_characters=0;
	ifstream infile; 
    infile.open("test.txt"); 
    char c;
    infile.get(c);
    int count=0; // number of lines
    while (count<4) // Atleast four lines of input needed
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
    		infile.get(c); //Read file character by character and put sticker colours in array
    		sticker_colors[i][j]=c-'0';  // Convert character to number
    	}
    	infile.get(c);
    }

    
    algo_finder_config(sticker_colors); // Configure algorithm finder
	int *reverse_sol=execute(solved_pyraminx); // Get solution
	bool sol_found=solution_found(); // Check whether a solution was found
	if (sol_found==false)
	{
		ofstream ofile("solved_moves.txt");
		ofile << "";
		ofile.close();
		cout << "Solution not found. Please enter valid input. " << endl; // Solution is wrong. Input invalid
	}
	else
	{
		int sol_len=sol_length(); // Find length of solution
	//cout << endl;
	cout << "Shortest solution from the start cube state to solved cube is : ";
	//The soltion entered is reversed to compensate for white stickers. It needs to be inverted.	
	int sol[sol_len];
	for (int i=0;i<sol_len;i++)
	{
		if (reverse_sol[sol_len-i-1]%2==0)
		sol[i]=reverse_sol[sol_len-i-1]+1; // Converting clockwise move to anticlockwise
		else
		sol[i]=reverse_sol[sol_len-i-1]-1; // Converting anticlockwise move to clockwise
	}
	ofstream ofile("solved_moves.txt");
	for (int i=0;i<sol_len;i++)
	{
		cout << moves[sol[i]]; // Print to terminal
		ofile << sol[i] << " "; // Write in file
	}
	ofile.close();
	cout << endl;
	}

}