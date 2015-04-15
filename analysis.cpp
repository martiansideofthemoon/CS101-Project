#include <iostream>
#include <fstream>
#include "pyraminx_solver_functions.cpp"

using namespace std;
int sticker_colors[4][9]= {{0,0,0,0,0,0,0,0,0},
                           {1,1,1,1,1,1,1,1,1},
                           {2,2,2,2,2,2,2,2,2},
                           {3,3,3,3,3,3,3,3,3}}; //Pyraminx Array. Solved initally
string moves[16]={"U ", "U' ", "L ", "L' ","R ", "R' ", "B ","B' ","u ","u' ","l ","l' ","r ","r' ","b ","b' "}; // set of moves
int move_number_array[16]; // Set of moves used to shuffle Pyraminx
int attempt_sequence[1000]; // Set of moves attempted by user
int num_attempts=0; // number of moves attempted by user
void adjacency1(int ip[4][9],int adj[16][4][9]); // Prototype to generate adjacency array. This array is the set of all
                                                 //16 states which can be reached by performing one move on array
void execute_move(int); // Run the desired move
int main()
{
	ifstream ifile("Analysis.txt");
	cout << "Your shuffling algorithm was ";
	for (int i=0;i<15;i++)
	{
		string move="";
		ifile >> move; //Hold move ID
		int move_number=0;
    //Converting string to integer. Storing in move_number
		if (move[1]=='\0')
		{
			move_number=(int)move[0]-48;		
		}
		else if (move[2]=='\0')
		{
			move_number=((int)move[0]-48)*10 + ((int)move[1]-48);
		}
		cout << moves[move_number]; // Printing move name using move array and move ID which is in move_number
		move_number_array[i]=move_number; // Update move_number array
		execute_move(move_number); // Run this move to update sticker_colors array
	}
	cout << endl;
	string attempt_string="";
  //Reading user attempt in a similar manner
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
	cout << "You attempted the following sequence of moves : ";
	for (int i=0;i<num_attempts;i++)
		cout << moves[attempt_sequence[i]] << " ";
	cout << endl;
	int active_move=0; // Represents active move in user's solution
	cout << "Shortest solution from the start state is : ";
  
	int *sol=execute(sticker_colors); // Generate inital shortest solution. Execute finds shortest solution
		int sol_len=sol_length();
		//cout << "Shortest solution after this move is ";
		for (int i=0;i<sol_len;i++)
		{
			cout << moves[sol[i]];
		}
		cout << endl;
    // We now execute the user's moves one by one and run execute() each time
	for (active_move=0;active_move<num_attempts;active_move++)
	{
		cout << "Analyzing move number " << (active_move+1) << " (" << moves[attempt_sequence[active_move]] << ")..." << endl;
		
		execute_move(attempt_sequence[active_move]); // Change sticker_colors array

		sol=execute(sticker_colors); // Find shortest solution

		sol_len=sol_length();
		cout << "Shortest solution after this move is ";
		for (int i=0;i<sol_len;i++)
		{
			cout << moves[sol[i]]; // Print it in readable format
		}
		cout << endl;
	}
	ifile.close();
}

void execute_move(int moveid)
{
	int adj[16][4][9];
      adjacency1(sticker_colors,adj); //Generate an array of all possible states achievable after executing one move on array
      for (int a=0;a<4;a++)
      {
         for (int b=0;b<9;b++)
            sticker_colors[a][b]=adj[moveid][a][b]; // set sticker_colors array to the element in adjacency1 array corresponding to moveid
      }
}
void adjacency1(int ip[4][9],int adj[16][4][9])
{
  //int adj[8][4][9];

  for (int i=0; i<16;i++)
  {
    for(int j=0;j<4;j++)
    {
      for(int k=0;k<9;k++)
      {
        adj[i][j][k]=ip[j][k];      // initialization of ajacency list.
      }
    }
  }

  // Set of instructions written for each of the sixteen moves.
  
  // for U
  for(int i=0;i<4;i++)
    adj[0][0][i]=ip[1][i];
  for(int i=0;i<4;i++)
    adj[0][1][i]=ip[2][i];
  for(int i=0;i<4;i++)
    adj[0][2][i]=ip[0][i];


  // for U'

  for(int i=0;i<4;i++)
    adj[1][0][i]=ip[2][i];
  for(int i=0;i<4;i++)
    adj[1][1][i]=ip[0][i];
  for(int i=0;i<4;i++)
    adj[1][2][i]=ip[1][i];

  // for L

  adj[2][0][1]=ip[2][6];
  adj[2][0][4]=ip[2][8];              // face 1
  adj[2][0][5]=ip[2][7];
  adj[2][0][6]=ip[2][3];

  adj[2][2][3]=ip[3][6];
  adj[2][2][6]=ip[3][1];              // face 3
  adj[2][2][7]=ip[3][5];
  adj[2][2][8]=ip[3][4];

  adj[2][3][1]=ip[0][1];
  adj[2][3][4]=ip[0][4];              // face 4
  adj[2][3][5]=ip[0][5];
  adj[2][3][6]=ip[0][6];

  // for L'

  adj[3][3][1]=ip[2][6];
  adj[3][3][4]=ip[2][8];              // face 4
  adj[3][3][5]=ip[2][7];
  adj[3][3][6]=ip[2][3];

  adj[3][2][3]=ip[0][6];
  adj[3][2][6]=ip[0][1];              // face 3
  adj[3][2][7]=ip[0][5];
  adj[3][2][8]=ip[0][4];

  adj[3][0][1]=ip[3][1];
  adj[3][0][4]=ip[3][4];              // face 1
  adj[3][0][5]=ip[3][5];
  adj[3][0][6]=ip[3][6];

  // for R

  adj[4][0][3]=ip[3][1];
  adj[4][0][6]=ip[3][3];              // face 1
  adj[4][0][7]=ip[3][2];
  adj[4][0][8]=ip[3][0];

  adj[4][1][1]=ip[0][6];
  adj[4][1][4]=ip[0][8];              // face 2
  adj[4][1][5]=ip[0][7];
  adj[4][1][6]=ip[0][3];

  adj[4][3][0]=ip[1][4];
  adj[4][3][1]=ip[1][6];              // face 4
  adj[4][3][2]=ip[1][5];
  adj[4][3][3]=ip[1][1];

  //for R'

  adj[5][0][3]=ip[1][6];
  adj[5][0][6]=ip[1][1];              // face 1
  adj[5][0][7]=ip[1][5];
  adj[5][0][8]=ip[1][4];

  adj[5][1][1]=ip[3][3];
  adj[5][1][4]=ip[3][0];              // face 2
  adj[5][1][5]=ip[3][2];
  adj[5][1][6]=ip[3][1];

  adj[5][3][0]=ip[0][8];
  adj[5][3][1]=ip[0][3];              // face 4
  adj[5][3][2]=ip[0][7];
  adj[5][3][3]=ip[0][6];

  // for B
  adj[6][1][3]=ip[3][3];
  adj[6][1][6]=ip[3][6];              // face 2
  adj[6][1][7]=ip[3][7];
  adj[6][1][8]=ip[3][8];

  adj[6][2][1]=ip[1][6];
  adj[6][2][4]=ip[1][8];              // face 3
  adj[6][2][5]=ip[1][7];
  adj[6][2][6]=ip[1][3];

  adj[6][3][3]=ip[2][6];
  adj[6][3][6]=ip[2][1];              // face 4
  adj[6][3][7]=ip[2][5];
  adj[6][3][8]=ip[2][4];

  // for B'

  adj[7][1][3]=ip[2][6];
  adj[7][1][6]=ip[2][1];              // face 2
  adj[7][1][7]=ip[2][5];
  adj[7][1][8]=ip[2][4];

  adj[7][2][1]=ip[3][6];
  adj[7][2][4]=ip[3][8];              // face 3
  adj[7][2][5]=ip[3][7];
  adj[7][2][6]=ip[3][3];

  adj[7][3][3]=ip[1][3];
  adj[7][3][6]=ip[1][6];              // face 4
  adj[7][3][7]=ip[1][7];
  adj[7][3][8]=ip[1][8];

  adj[8][0][0]=ip[1][0];
  adj[8][1][0]=ip[2][0];             //for u
  adj[8][2][0]=ip[0][0];

  adj[9][0][0]=ip[2][0];
  adj[9][1][0]=ip[0][0];             //for u'
  adj[9][2][0]=ip[1][0];

  adj[10][0][4]=ip[2][8];
  adj[10][2][8]=ip[3][4];            //for l
  adj[10][3][4]=ip[0][4];

  adj[11][0][4]=ip[3][4];
  adj[11][2][8]=ip[0][4];              //for l'
  adj[11][3][4]=ip[2][8];

  adj[12][0][8]=ip[3][0];
  adj[12][1][4]=ip[0][8];           //for r
  adj[12][3][0]=ip[1][4];

  adj[13][0][8]=ip[1][4];
  adj[13][1][4]=ip[3][0];           //for r'
  adj[13][3][0]=ip[0][8];

  adj[14][1][8]=ip[3][8];
  adj[14][2][4]=ip[1][8];            //for b
  adj[14][3][8]=ip[2][4];

  adj[15][3][8]=ip[1][8];
  adj[15][1][8]=ip[2][4];            //for b'
  adj[15][2][4]=ip[3][8];




}