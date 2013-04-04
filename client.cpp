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


template <typename T>
void disp(T begin, T end)
{
	while(begin != end)
	{
		cout << *begin++ <<" ";
//		++begin;
	}
	cout << endl;
}


int main()
{
	cout << "Welcome to b-tree library !" << endl;
	int n = 4;												//n is order
	Tree t(n);

	srand(time(0));
	for(int i=0;i<20;++i)
	{
		int k=rand()%100;
		cout << k<<" ";
		t.push(k);
	}

//	Tree::Iterator it = t.begin();
//	Tree::Iterator it = t.end();

	cout<<endl;
	t.disp();
	cout<<endl;


/*
	while(it != t.end())
	{
		cout << *it << " ";
		++it;
	}
*/
	disp(t.begin(),t.end());

//	cout << *it << endl;
/*
	cout<<*it<<endl; ++it;
	cout<<*it<<endl; ++it;
	cout<<*it<<endl; ++it;
	cout<<*it<<endl; ++it;
	cout<<*it<<endl; ++it;
	cout<<*it<<endl; ++it;
	cout<<*it<<endl; ++it;
	cout<<*it<<endl; ++it;
	cout<<*it<<endl; ++it;
	cout<<*it<<endl;
*/

/*
	--it;
	while(it != t.begin())
	{
		cout << *it-- << " ";
	}
*/


//	for(int i=0;i<10;++i)
//		cout<<*++it<<endl;

}
