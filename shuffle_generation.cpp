#include <stdlib.h>
#include <string>
#include <iostream>
#include <ctime>
using namespace std;


int x,y,i,z;
//char asd[80]="abc";
char dir_large[8][10] = {"U ", "U' ", "L ", "L' ","R ", "R' ", "B ","B' "};
char dir_r[2][10] ={"r ","r' "};
char dir_l[2][10] ={"l ","l' "};
char dir_u[2][10] ={"b ","b' "};
char dir_b[2][10] ={"u ","u' "};

//char display[15][10];
int display[15];
void scramble(int*display, int k) //Execute the bigger eight moves on Pyraminx
{
  int y=((int)(rand()%8)); //Choose among 8 numbers
  //Ensure that the two consecutive moves aren't same or complementary
    if(y%2==0)
     z=y+1;
     else
     z=y-1;
     if (k>0)
     {
      while(display[k-1]==z || display[k-1]==y)
     {
         y=((int)(rand()%8));
        if(y%2==0)
     z=y+1;
     else
     z=y-1;
     }
     }
      display[k]=y;

  //cout << display[k] << "," << k << endl;
}

int* final_display()
{
  srand(static_cast<unsigned int>(time(0))); //Random seed
  for(i=0;i<11;i++)
  scramble(display, i);
  //return display;

 display[11]=8+(int)rand()%2; // For u or u'

  display[12]=10+(int)rand()%2; // For l or l'

  display[13]=12+(int)rand()%2; // For r or r'

  display[14]=14+(int)rand()%2; // For b or b'
  return display;
}
//cout<<" "<<display;
void scramble_pyraminx()
{
    


    final_display();
}

//int adj[8][4][9];

void adjacency(int ip[4][9],int adj[16][4][9])
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
// return (0);