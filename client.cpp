/*
 * client.cpp
 *      Author: akshay hebbar
 */

#include<iostream>
using namespace std;


#include <typeinfo>
#include <stack>
#include "btree.h"
#include <cstdlib>

template <typename T>
void disp(T begin, T end)
{
	while(begin != end)
		cout << *begin++ <<" ";
	cout << endl;
}


template<typename T>
class mycomp
{
public:
	bool operator()(const T& a, const T& b) const
	{
		return a < b;
	}
};

int main()
{

	cout << "Welcome to b-tree library !" << endl;

	Tree <int > t;

	Tree <char,3,mycomp<char> > c;


	srand(time(0));
	for(int i=0;i<30;++i)
	{
		int k=rand()%26;
		cout << k<<" ";
		t.push(k);
		c.push(65+k);
	}

	cout << endl;

	Tree<int>::Iterator it = t.end();

///*
	cout<<endl;
	t.disp();
	cout<<endl;
	c.disp();
	cout<<endl;
//*/


	disp(c.begin(),c.end());



//	auto it = str.begin();

//	auto itt = str.end();

	while(it != t.end())
	{
		cout << *it << " ";
		++it;
	}
	cout<<endl;


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

//	t.disp_like_tree();

//	c.disp_like_tree();

//	str.disp_like_tree();

//	disp(c.begin(),c.end());

//	for(int i=0;i<10;++i)
//		cout<<*++it<<endl;

}
