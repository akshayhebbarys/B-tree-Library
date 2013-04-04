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
		cout << *begin++ <<" ";
	cout << endl;
}


int main()
{
	cout << "Welcome to b-tree library !" << endl;

	Tree <int, 2>t;

	Tree<char, 2> c;

	Tree<string> str;

	srand(time(0));
	for(int i=0;i<20;++i)
	{
		int k=rand()%26;
		cout << k<<" ";
		t.push(k);
		c.push(65+k);
	}

//	Tree::Iterator it = t.begin();
//	Tree::Iterator it = t.end();

	str.push("akshay");
/*
	cout<<endl;
	t.disp();
	cout<<endl;
	c.disp();
	cout<<endl;
	str.disp();
	cout<<endl;
*/

/*
	while(it != t.end())
	{
		cout << *it << " ";
		++it;
	}
*/
//	disp(t.begin(),t.end());

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

	t.disp_like_tree();

	c.disp_like_tree();

	str.disp_like_tree();

//	disp(c.begin(),c.end());

//	for(int i=0;i<10;++i)
//		cout<<*++it<<endl;

}
