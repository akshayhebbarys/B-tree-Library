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
	int n = 2;
	Tree t(n);

/*
	t.push(6);
	t.push(5);
	t.push(8);
	t.push(3);
	t.push(1);
	t.push(2);
	t.push(4);
	t.push(7);
	t.push(10);
	t.push(9);

*/

///*
	for(int i=0;i<20;++i)
	{
		int k=rand()%30;
//		cout << k<<"\n";
		t.push(k);
	}
//*/

/*
		t.push(43);
		t.push(45);	t.push(3);	t.push(0);	t.push(5);	t.push(67);	t.push(54);	t.push(90);	t.push(-78);
*/
/*
	t.push(10);
	t.push(20);
	t.push(30);
	t.push(40);
	t.push(50);
	t.push(60);
	t.push(70);
	t.push(80);
	t.push(31);
	t.push(32);
	t.push(33);

*/

/*
	t.push(100);
	t.push(90);
	t.push(80);
	t.push(70);
	t.push(60);
	t.push(50);
	t.push(40);
	t.push(30);
	t.push(20);
	t.push(10);
	t.push(9);
	t.push(8);
	t.push(7);
	t.push(6);
*/
	cout<<endl;
	t.disp();
}
