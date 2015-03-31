#include <stdlib.h>
#include <string>
#include <iostream>
#include <ctime>
using namespace std;


int x,y,i,z;
//char asd[80]="abc";
char dir_large[8][10] = {"R ", "R' ", "L ", "L' ", "B ","B' ", "U ", "U' "};
char dir_r[2][10] ={"r ","r' "};
char dir_l[2][10] ={"l ","l' "};
char dir_u[2][10] ={"b ","b' "};
char dir_b[2][10] ={"u ","u' "};

//char display[15][10];
int display[15];
void scramble(int*display, int k)
{
  int y=((int)(rand()%8));
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

void final_display()
{
  for(i=0;i<11;i++)
  scramble(display, i);
for (int i=0;i<11;i++)
{

    cout << dir_large[display[i]];
}
 cout << dir_r[(int)rand()%2];

  cout << dir_l[(int)rand()%2];

  cout << dir_u[(int)rand()%2];

  cout << dir_b[(int)rand()%2];
  cout << endl;
}
//cout<<" "<<display;
int main()
{
    srand(static_cast<unsigned int>(time(0)));


    final_display();
}
// return (0);