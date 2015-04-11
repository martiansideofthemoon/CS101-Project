#include <iostream>
#include <fstream>
#include "shuffle_generation.cpp"
using namespace std;
int sticker_colors[4][9]= {{0,0,0,0,0,0,0,0,0},
                           {1,1,1,1,1,1,1,1,1},
                           {2,2,2,2,2,2,2,2,2},
                           {3,3,3,3,3,3,3,3,3}};
string moves[16]={"U ", "U' ", "L ", "L' ","R ", "R' ", "B ","B' ","u ","u' ","l ","l' ","r ","r' ","b ","b' "};
int move_number_array[16];
int attempt_sequence[1000];
int num_attempts=0;
void execute_move(int);
int main()
{
	ifstream ifile("Analysis.txt");
	cout << "Your shuffling algorithm was ";
	for (int i=0;i<15;i++)
	{
		string move="";
		ifile >> move;
		int move_number=0;
		if (move[1]=='\0')
		{
			move_number=(int)move[0]-48;		
		}
		else if (move[2]=='\0')
		{
			move_number=((int)move[0]-48)*10 + ((int)move[1]-48);
		}
		cout << moves[move_number] << " ";
		move_number_array[i]=move_number;
		execute_move(move_number);
	}
	cout << endl;
	string attempt_string="";
	while (ifile >> attempt_string)
	{
		int move_number=0;
		if (attempt_string[1]=='\0')
		{
			move_number=(int)attempt_string[0]-48;		
		}
		else if (attempt_string[2]=='\0')
		{
			move_number=((int)attempt_string[0]-48)*10 + ((int)attempt_string[1]-48);
		}
		attempt_sequence[num_attempts]=move_number;
		num_attempts++;
	}
	for (int i=0;i<num_attempts;i++)
		cout << moves[attempt_sequence[i]] << " ";
	cout << endl;

	ifile.close();
}
void execute_move(int moveid)
{
	int adj[16][4][9];
      adjacency(sticker_colors,adj);
      for (int a=0;a<4;a++)
      {
         for (int b=0;b<9;b++)
            sticker_colors[a][b]=adj[moveid][a][b];
      }
}