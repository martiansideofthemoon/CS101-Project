#include <fstream>
#include <iostream>
using namespace std;
 
int main ()
{
    
   char data[100]="why the fuck";

   // open a file in write mode.
  
   // open a file in read mode.
   ifstream infile; 
   infile.open("test.txt"); 
   char c;
   infile.get(c);
   cout << c << endl;
  // cout << "Reading from the file" << endl; 
   for (int i=0;i<4;i++)
   {
   infile >> data; 

   // write the data at the screen.
   cout << data << endl;
   }
   // again read the data from the file and display it.
   

   // close the opened file.
   infile.close();

   return 0;
}