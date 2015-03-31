#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#define SIZE 1000000

class node{
public:
    int key[4][9];
    node* left_child;
    node* right_child;
    node* parent;

    int height;
    bool isNull;												// cant use a null pointer so used this.isNull is true only for null
    int level;
    int* guardian[4][9];
    node *bfs_parent;
    int v;
    node(){
    	height=0;
    	isNull=false;
    	}
};

bool found;													// to check whether desired array is found.
int desired[4][9],move_required[11];											// the desired array
int currnodes[1000000][4][9],next[1000000][4][9];
int next_parents[1000000],currnodes_parents[1000000];
	  /*checked stores all the  nodes that have been checked
		currnodes stores all the nodes that are to be checked and are reached by making one move on the previous config.
		next stores all the nodes that can be reached from a given config by making one move

		***** will probably need to initialize level.******/


node null;															// since no null pointer
node* root;														// root of the tree(it gets updated in some cases)so need of a class
node checked[SIZE];
int nodecounter=1,currnodes_counter=1;

string shortest_path[]={"U","U'","L","L'","R","R'","B","B'"};

void fill_array(){
	for (int i = 0; i < 4; ++i)
	{
		for(int j=0;j<9;j++)
		{
			cin>>desired[i][j];
		}
	}
	return;
}

void height(node* h);

void fix_insert(node* s);

void height(node* h){
	h->height=max(h->left_child->height,h->right_child->height)+1;	// this is obvious.
}

void left_rotate(node* a){
	node* b;
	b=a->right_child;
	a->right_child=b->left_child;
	if(b->left_child!=&null)							// beacause we dont want to change parent of null .
	b->left_child->parent=a;							// to make changes in b->left as according to previous statement.
	b->parent=a->parent	;
	if(b->parent==&null)
	root=b;												// make b the root;
	else if(a==a->parent->left_child)					
		a->parent->left_child=b;						
	else a->parent->right_child=b;
	b->left_child=a;									// easy to understand when blocks are drawn.
	a->parent=b;										//	mainly changing elements are of a & b and parents of their children				
	height(a);											// important to update height of b first then a
	height(b);
}

void right_rotate(node* m){
	node*n;
	n=m->left_child;
	m->left_child=n->right_child;
	if(n->right_child!=&null)
	n->right_child->parent=m;
	n->parent=m->parent;
	if(n->parent==&null)
		root=n;
	else if(m==m->parent->left_child)
		m->parent->left_child=n;
	else m->parent->right_child=n;
	n->right_child=m;
	m->parent=n;
	height(m);
	height(n);
}

void fix_insert(node* s){
node* q;
q=s;
int count=0;

while(!(q==&null))
{
	
	height(q);
	if(q->right_child->height - q->left_child->height>=2)
	{
		

		if(q->right_child->right_child->height >= q->right_child->left_child->height)
		{
			
			left_rotate(q);
		}
		else 
		{
			
			right_rotate(q->right_child);
			left_rotate(q);
		}
	}
	else if(q->left_child->height - q->right_child->height>=2)
	{
		if(q->left_child->left_child->height >= q->left_child->right_child->height)
		{
			
			right_rotate(q);
		}
		else 
		{
			
			left_rotate(q->left_child);
			right_rotate(q);
		}
	}

	q=q->parent;
}
}

int compare(int key1[4][9],int key2[4][9])
{
	int var=1;
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<9;j++)
		{
			if(key1[i][j]==key2[i][j])
				var=0;
			else if(key1[i][j]>key2[i][j])
				return 1;
			else return 2;
		}
	}
	return var;
}

bool insert(int key[4][9],int level,int parent_index)
{	
	int comp;
	node* y;
	node* x;
	y=&null;														// initialize y for computations.
	x=root;	
																	// initialize x for computations.(must be initialised for )	
	while(x->isNull==false)											// in this loop we go down the tree to find where is the new key
	{																// to be inserted and y kind of stores the parent position of x 
		comp=compare(key,x->key);
			
		y=x;														// as the loop iterates x goes down tracking and x crosses 
		if(comp==0)
			break;
		else if(comp==1)												// the node position where key is to be input and y reaches there.
			x=x->right_child;
		else x=x->left_child;
	}
	if(comp==0)
	{	
		return false;
	}
	comp=compare(key,y->key);
	checked[nodecounter].parent=y;									// parent of the  new node is y.
	if(comp==1)
		y->right_child=&checked[nodecounter];						// assign appropriate value to leftchild/rightchild of y.
	else y->left_child=&checked[nodecounter];
	
	checked[nodecounter].level=level;
	checked[nodecounter].bfs_parent=&checked[parent_index];
	for(int a=0;a<4;a++)
	{
	for(int b=0;b<9;b++)
	{
		checked[nodecounter].key[a][b]=key[a][b];				// updating the parent list 
	}
	}
	
	fix_insert(&checked[nodecounter]);
																// fix the tree to balance the height.
	return true;												// so tht next time the function is called new node is stored in
																	// the next counter


}


int adj[8][4][9];

int adjacency(int ip[4][9])
{
	//int adj[8][4][9];

	for (int i=0; i<8;i++)
	{
		for(int j=0;j<4;j++)
		{
			for(int k=0;k<9;k++)
			{
				adj[i][j][k]=ip[j][k];			// initialization of ajacency list.
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
	adj[2][0][4]=ip[2][8];							// face 1
	adj[2][0][5]=ip[2][7];
	adj[2][0][6]=ip[2][3];

	adj[2][2][3]=ip[3][6];
	adj[2][2][6]=ip[3][1];							// face 3
	adj[2][2][7]=ip[3][5];
	adj[2][2][8]=ip[3][4];

	adj[2][3][1]=ip[0][1];
	adj[2][3][4]=ip[0][4];							// face 4
	adj[2][3][5]=ip[0][5];
	adj[2][3][6]=ip[0][6];

	// for L'

	adj[3][3][1]=ip[2][6];
	adj[3][3][4]=ip[2][8];							// face 4
	adj[3][3][5]=ip[2][7];
	adj[3][3][6]=ip[2][3];

	adj[3][2][3]=ip[0][6];
	adj[3][2][6]=ip[0][1];							// face 3
	adj[3][2][7]=ip[0][5];
	adj[3][2][8]=ip[0][4];

	adj[3][0][1]=ip[3][1];
	adj[3][0][4]=ip[3][4];							// face 1
	adj[3][0][5]=ip[3][5];
	adj[3][0][6]=ip[3][6];

	// for R

	adj[4][0][3]=ip[3][1];
	adj[4][0][6]=ip[3][3];							// face 1
	adj[4][0][7]=ip[3][2];
	adj[4][0][8]=ip[3][0];

	adj[4][1][1]=ip[0][6];
	adj[4][1][4]=ip[0][8];							// face 2
	adj[4][1][5]=ip[0][7];
	adj[4][1][6]=ip[0][3];

	adj[4][3][0]=ip[1][4];
	adj[4][3][1]=ip[1][6];							// face 4
	adj[4][3][2]=ip[1][5];
	adj[4][3][3]=ip[1][1];

	//for R'

	adj[5][0][3]=ip[1][6];
	adj[5][0][6]=ip[1][1];							// face 1
	adj[5][0][7]=ip[1][5];
	adj[5][0][8]=ip[1][4];

	adj[5][1][1]=ip[3][3];
	adj[5][1][4]=ip[3][0];							// face 2
	adj[5][1][5]=ip[3][2];
	adj[5][1][6]=ip[3][1];

	adj[5][3][0]=ip[0][8];
	adj[5][3][1]=ip[0][3];							// face 4
	adj[5][3][2]=ip[0][7];
	adj[5][3][3]=ip[0][6];

	// for B
	adj[6][1][3]=ip[3][3];
	adj[6][1][6]=ip[3][6];							// face 2
	adj[6][1][7]=ip[3][7];
	adj[6][1][8]=ip[3][8];

	adj[6][2][1]=ip[1][6];
	adj[6][2][4]=ip[1][8];							// face 3
	adj[6][2][5]=ip[1][7];
	adj[6][2][6]=ip[1][3];

	adj[6][3][3]=ip[2][6];
	adj[6][3][6]=ip[2][1];							// face 4
	adj[6][3][7]=ip[2][5];
	adj[6][3][8]=ip[2][4];

	// for B'

	adj[7][1][3]=ip[2][6];
	adj[7][1][6]=ip[2][1];							// face 2
	adj[7][1][7]=ip[2][5];
	adj[7][1][8]=ip[2][4];

	adj[7][2][1]=ip[3][6];
	adj[7][2][4]=ip[3][8];							// face 3
	adj[7][2][5]=ip[3][7];
	adj[7][2][6]=ip[3][3];

	adj[7][3][3]=ip[1][3];
	adj[7][3][6]=ip[1][6];							// face 4
	adj[7][3][7]=ip[1][7];
	adj[7][3][8]=ip[1][8];
	return 1 ;
}

bool check(int arr1[4][9],int arr2[4][9])
{
	bool flag=false;
	for (int i = 0; i < 4; ++i)
	{
		for(int j=0;j<9;j++)
		{
			if(arr1[i][j]==arr2[i][j])
				flag=true;
			else 
			{
				flag=false;
				return flag;
			}
		}
	}
	return flag;
}

bool stop_for_ip(int arr[4][9]){
	bool flag=false;
	//cout<<"yo ";
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<9;j++)
		{
			if(arr[i][j]==arr[i][(j+1)%9])
				flag=true;
			else 
			{
				flag=false;
				return flag;
			}
			//cout<<flag<<endl;	
		}
	}
	return flag;
}
int backtrack_counter;

void shortest_sol(int q);

void backtrack(node* x)
{
	int i=0;
	//cout<<"hi";
	while(x->bfs_parent!=&null){
		move_required[i]=x->v;
		i++;
		x=x->bfs_parent;
	}
	backtrack_counter=i-1;
	shortest_sol(backtrack_counter);
}

void shortest_sol(int q){
	//cout<<"hi";
	for (int i = q; i >=0; i--)
	{
		cout<<shortest_path[move_required[i]]<<" ";
	}
	cout << endl;
}

void bfs(int ip[4][9])
{
	//cout<<"call";
	int x=1,next_counter;
	int arr1[4][9];						// x is to save that how many moves got u there
	for(int i=0;i<4;i++)
	{
		for(int j=0;j<9;j++)
		{
			checked[0].key[i][j]=ip[i][j];					// initialisation of checked as ip
			currnodes[0][i][j]=ip[i][j];
		}
	}
	checked[0].bfs_parent=&null;
	while (currnodes_counter )
	{											
		int x;
		next_counter=0;									// initialize next_counter to 0 to keep it updated
		for(int i=0;i<currnodes_counter;i++)
		{
			int arr[4][9];
			for(int j=0;j<4;j++)
			{
				for(int k=0;k<9;k++)
				{
					arr[j][k]=currnodes[i][j][k];		// arr is defined just so that it can be passed in the adjacency function.
				}
			}
			adjacency(arr);
			for(int v=0;v<8;v++)						// v<8 because at each config 8 moves are possible.
			{	
				for(int j=0;j<4;j++)
				{
					for(int k=0;k<9;k++)
					{
					arr1[j][k]=adj[v][j][k];		// arr is defined just so that it can be passed in the adjacency function.
					}
				}
				bool flag;
				flag=insert(arr1,x,currnodes_parents[i]);
				if(!flag)
					continue;
				checked[nodecounter].v=v;
				//cout<<v<<endl;
				/*for(int a=0;a<4;a++)
				{
					for(int b=0;b<9;b++)
					{
						cout<<arr1[a][b]<<"\t";
					}
					cout<<endl;
				}*/
				if(stop_for_ip(arr1)==true)						// new line.
					{	
						backtrack(&checked[nodecounter]);
						return;	
					}												
				for(int a=0;a<4;a++)
				{
					for(int b=0;b<9;b++)
					{
						next[next_counter][a][b]=adj[v][a][b];	
						next_parents[next_counter]=nodecounter;				// 	updating the next counter
					}
				}
				next_counter++;
				nodecounter++;
			}
		}
		for(int i=0;i<next_counter;i++)
		{
			for(int j=0;j<4;j++)
			{
				for(int k=0;k<9;k++)
					currnodes[i][j][k]=next[i][j][k];
					currnodes_parents[i]=next_parents[i];
			}
		}
		currnodes_counter=next_counter;
		x++;
		//cout<<nodecounter<<endl;
	}


}

int main(){
	
	int arr1[4][9];
	null.isNull=true;
	null.height=-1;
	root=checked;
	for(int i=0;i<SIZE;i++)
	{	
		checked[i].parent=&null;
		checked[i].left_child=&null;
		checked[i].right_child=&null;
	}
	/*for(int i=0;i<4;i++)
	{
		for(int j=0;j<9;j++)
		{
			cin>>arr1[i][j];
		}
	}*/
	//fill_array();

	/*ofstream outfile;
   outfile.open("test.txt");
   outfile << "" ;
   outfile.close();*/
   
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
    		arr1[i][j]=c-'0';
    	}
    	infile.get(c);
    }
	cout << "Input received, finding shortest solution.." << endl;
	bfs(arr1);
	cout<<"khatam" << endl;
}