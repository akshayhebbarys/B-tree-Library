/*
 * client.cpp
 *      Author: akshay hebbar
 */

#include<iostream>
using namespace std;

#include<typeinfo>
#include<stack>
#include "btree.h"
#include<cstdlib>

int main()
{
	cout << "Welcome to b-tree library !" << endl;
	int n = 3;
	Tree t(n);

	srand(time(0));
	for(int i=0;i<20;++i)
	{
		int k=rand()%100;
		cout << k<<" ";
		t.push(k);
	}


	cout<<endl;
	t.disp();
}
