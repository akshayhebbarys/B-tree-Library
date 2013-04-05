/*
 * btree.h
 *      Author: Akshay hebbar
 */

#ifndef BTREE_H
#define BTREE_H

#include <stack>				//required to store path of traversal
#include <deque>				//required to display our data structure as tree
#include <cstdlib>				//used for 'exit' if client chooses bad '_Order'

/*
 * Things to do:
 * 		insertion : done
 * 		Traversal and display : done
 * 		find 			//- return iterator
 * 		bulk-loading
 * 		deletion
 * 		iterator	: done
 * 		providing a predicate for sort	: done
 * 		making it generic	: done
 */



template<typename> class More;
template<typename> class Less;

template <typename T, int = 6 , typename = More<T> > class Tree ;

template <typename T, int _Order>
class Node
{
private:
	Node();
	bool insert(T new_key, int pos);
	~Node();

	bool isempty()
	{
		return !num_of_elem;
	}

	int num_of_elem;
	T *keys;
	Node<T,_Order>* *links;

	template < typename,int,typename> friend class Tree;
};

template <typename T, int _Order , typename _Predicate>
class Tree
{
public:													//interface to btree
	Tree();
	void push(T new_key);
	void disp() const;
	void disp_like_tree() const;
	~Tree();

	class Iterator
	{
	public:
		explicit Iterator() :ptr(0),pos(0),root(0){}

		Iterator(Node<T,_Order>* Nptr, int position, stack <pair<Node<T,_Order>*, int> > trace, Node<T,_Order> * Root):ptr(Nptr),pos(position),back_trace(trace),root(Root){}

		Iterator(const Iterator& i)
		{
			ptr = i.ptr;
			pos = i.pos;
			back_trace = i.back_trace;
			root = i.root;
		}

		T operator*() const
		{
			return ptr->keys[pos];
		}

		bool operator==(const Iterator& rhs) const
		{
			return (ptr == rhs.ptr && pos == rhs.pos);
		}

		bool operator!=(const Iterator& rhs) const
		{
			return !(*this == rhs);
		}

		Iterator& operator++()
		{
			Node<T,_Order> *temp = ptr;
//			std::cout << "iterator : " << temp->keys[0]<<" index : " << pos <<endl;
//			std::cout << "Back trace top : " << back_trace.top().first->keys[0]<<" index : " << back_trace.top().second <<std::endl;
			if(pos+1 <= temp->num_of_elem && temp->links[pos+1])
			{
//				std::cout << "popping2 : "<<back_trace.top().first->keys[0] << " index : " << back_trace.top().second << endl;
				back_trace.pop();
//				std::cout << "pushing2 : "<<temp->keys[0] << " index : " <<pos+1<< endl;
				back_trace.push(pair<Node<T,_Order>*, int> (temp,pos+1));
				temp = temp->links[pos+1];
				while(temp)
				{
//					std::cout << "pushing : "<<temp->keys[0] << " index : " <<0<< endl;
					back_trace.push(pair<Node<T,_Order> *,int> (temp,0));
					temp = temp->links[0];
				}
			}
			else if(pos + 1 < temp->num_of_elem)
			{
				back_trace.top().second += 1;
			}
			else
			{
				while(!back_trace.empty() && ((back_trace.top().second + 1 == back_trace.top().first->num_of_elem  && back_trace.top().first->links[back_trace.top().second+1] == 0 )||
						back_trace.top().second + 1 > back_trace.top().first->num_of_elem ))
				{
//					std::cout << "popping3 : "<<back_trace.top().first->keys[0] << " index : " << back_trace.top().second << endl;
					back_trace.pop();
				}
			}

			if(back_trace.empty())						//if the stack becomes empty, make iterator point to end node
			{
				while(temp)
				{
					back_trace.push(pair<Node<T,_Order>*,int>(temp,temp->num_of_elem));
					temp = temp->links[temp->num_of_elem];
				}
				ptr = temp;
				pos = 0;
			}
			else
			{
				ptr = back_trace.top().first;
				pos = back_trace.top().second;
			}
			return *this;
		}

		Iterator operator++(int)
		{
			Iterator prev_val = *this;
			Node<T,_Order> *temp = ptr;
			if(pos+1 <= temp->num_of_elem && temp->links[pos+1])
			{
				back_trace.pop();
				back_trace.push(pair<Node<T,_Order>*, int> (temp,pos+1));
				temp = temp->links[pos+1];
				while(temp)
				{
					back_trace.push(pair<Node<T,_Order> *,int> (temp,0));
					temp = temp->links[0];
				}
			}
			else if(pos + 1 < temp->num_of_elem)
			{
				back_trace.top().second += 1;
			}
			else
			{
				while(!back_trace.empty() && ((back_trace.top().second + 1 == back_trace.top().first->num_of_elem  && back_trace.top().first->links[back_trace.top().second+1] == 0 )||
						back_trace.top().second + 1 > back_trace.top().first->num_of_elem ))
				{
					back_trace.pop();
				}
			}

			if(back_trace.empty())						//if the stack becomes empty, make iterator point to end node
			{
				while(temp)
				{
					back_trace.push(pair<Node<T,_Order>*,int>(temp,temp->num_of_elem));
					temp = temp->links[temp->num_of_elem];
				}
				ptr = temp;
				pos = 0;
			}
			else
			{
				ptr = back_trace.top().first;
				pos = back_trace.top().second;
			}
			return prev_val;
		}

		Iterator& operator--()
		{
			Node<T,_Order> *temp = ptr;
			if(temp != NULL && pos == 0)
			{
				back_trace.pop();
				if(temp->links[pos])
				{
					back_trace.push(pair<Node<T,_Order>*, int>(temp,pos));
					temp = temp->links[pos];
					while(temp)
					{
						back_trace.push(pair<Node<T,_Order>*,int>(temp,temp->num_of_elem));
						temp=temp->links[temp->num_of_elem];
					}
					back_trace.top().second -= 1;
				}
				else
				{
					while( !back_trace.empty() && back_trace.top().second == 0)
					{
						back_trace.pop();
					}
					if(back_trace.empty())
					{
						Node<T,_Order>* temp = root;
						while(temp)
						{
							back_trace.push(pair<Node<T,_Order>*, int>(temp,0));
							temp=temp->links[0];
						}
					}
					else
						back_trace.top().second -= 1;
				}
			}
			else
			{
				back_trace.top().second -= 1;
				if(temp && temp->links[pos])
				{
					back_trace.push(pair<Node<T,_Order>*, int>(temp,pos));
					temp = temp->links[pos];
					while(temp)
					{
						back_trace.push(pair<Node<T,_Order>*,int>(temp,temp->num_of_elem));
						temp=temp->links[temp->num_of_elem];
					}
					back_trace.top().second -= 1;
				}
			}

			ptr = back_trace.top().first;
			pos = back_trace.top().second;

			return *this;
		}

		Iterator operator--(int)
		{
			Iterator prev_val = *this;
			Node<T,_Order> *temp = ptr;
			if(temp != NULL && pos == 0)
			{
				back_trace.pop();
				if(temp->links[pos])
				{
					back_trace.push(pair<Node<T,_Order>*, int>(temp,pos));
					temp = temp->links[pos];
					while(temp)
					{
						back_trace.push(pair<Node<T,_Order>*,int>(temp,temp->num_of_elem));
						temp=temp->links[temp->num_of_elem];
					}
					back_trace.top().second -= 1;
				}
				else
				{
					while( !back_trace.empty() && back_trace.top().second == 0)
					{
						back_trace.pop();
					}
					if(back_trace.empty())
					{
						Node<T,_Order>* temp = root;
						while(temp)
						{
							back_trace.push(pair<Node<T,_Order>*, int>(temp,0));
							temp=temp->links[0];
						}
					}
					else
						back_trace.top().second -= 1;
				}
			}
			else
			{
				back_trace.top().second -= 1;
				if(temp && temp->links[pos])
				{
					back_trace.push(pair<Node<T,_Order>*, int>(temp,pos));
					temp = temp->links[pos];
					while(temp)
					{
						back_trace.push(pair<Node<T,_Order>*,int>(temp,temp->num_of_elem));
						temp=temp->links[temp->num_of_elem];
					}
					back_trace.top().second -= 1;
				}
			}

			ptr = back_trace.top().first;
			pos = back_trace.top().second;

			return prev_val;
		}

	private:
		Node<T,_Order> *ptr;
		int pos;
		stack <pair<Node<T,_Order>*, int> > back_trace;
		Node<T,_Order> *root;
	};

	Iterator begin()
	{
		stack <pair<Node<T,_Order>*, int> > trace;
		Node<T,_Order>* temp = root;
		while(temp)
		{
//			std::cout << "pushing : "<<temp->keys[0] << " index : " <<0<< endl;
			trace.push(pair<Node<T,_Order>*, int>(temp,0));
			temp=temp->links[0];
		}
		return Iterator(trace.top().first,0,trace, root);
	}

	Iterator end()
	{
		stack<pair<Node<T,_Order>*, int> > trace;
		Node<T,_Order>* temp = root;
		while(temp)
		{
			trace.push(pair<Node<T,_Order>*,int>(temp,temp->num_of_elem));
			temp = temp->links[temp->num_of_elem];
		}
		return Iterator(temp,0,trace, root);
	}

	Iterator find(T key)											//not implemented
	{
		std::cout << key;
		return Iterator();
	}

private:
	void inorder_disp(Node<T,_Order>*) const;
	Node<T,_Order>* split(Node<T,_Order>*, int,int,T,stack<pair<Node<T,_Order>*, int> > &);
	void setlinks(Node<T,_Order>*,Node<T,_Order>*,Node<T,_Order>*,Node<T,_Order>*,int,int);					//when parent gets split
	void setlinks(Node<T,_Order>*,Node<T,_Order>*,Node<T,_Order>*,int);								//when only big child is split
	void free_all_nodes(Node<T,_Order>*);											//uses post-order
	void show_deque(deque<Node<T,_Order>*> &) const;

private:
	Node<T,_Order>* root;
	_Predicate predicate;
};

/**************************************************************************************************************/
template <typename T, int _Order>
Node<T,_Order>::Node()
{
	num_of_elem = 0;
	keys = new T[_Order-1];
	links = new Node<T,_Order>*[_Order];
	for(int i=0;i<_Order;++i)
		links[i] = 0;
//	std::cout << "ctor\n";
}

template <typename T, int _Order>
bool Node<T,_Order>::insert(T new_key, int pos)
{
	if(num_of_elem < (_Order-1))						//handles both insertions : 1)insert at end	2)insert in middle
	{
		links[num_of_elem+1] = links[num_of_elem];	//shifting keys and links by 1 place
		for(int i=num_of_elem;i>pos;--i)
		{
			keys[i] = keys[i-1];
			links[i] = links[i-1];
		}
		keys[pos] = new_key;

		++num_of_elem;

		return true;
	}
	return false;									//node is full
}

template <typename T, int _Order>
Node<T,_Order>::~Node()
{
	delete [] keys;
	delete [] links;
//	std::cout << "dtor\n";
}

/**************************************************************************************************************/

template <typename T, int _Order , typename _Predicate>
Tree<T,_Order,_Predicate>::Tree():root(0)
{
	predicate = _Predicate();
	if(_Order < 2)
	{
		std::cerr << "\nCould not create B-tree\nOrder should be at-least 2 [ie Number of elements in each node should be at-least 1]\nAborting\n";
		exit(1);
	}
}

template <typename T, int _Order , typename _Predicate>
void Tree<T,_Order,_Predicate>::push(T new_key)
{
	bool found_pos = false;
	if(root == NULL)
	{
		root = new Node<T,_Order>();
		root->insert(new_key,0);
	}
	else
	{
		Node<T,_Order> *temp = root;
		stack <pair<Node<T,_Order>*, int> > back_trace;
		int i=0;
		while(found_pos != true)
		{
			if(i < temp->num_of_elem && predicate(new_key,temp->keys[i]))
			{
				++i;
				if(i == (_Order- 1))
				{
					if(temp->links[i])
					{
						back_trace.push( pair<Node<T,_Order>*, int> (temp,i));
						temp=temp->links[i]; i=0;
					}
					else
						found_pos = true;
				}
			}
			else if(temp->links[i])
			{
				back_trace.push(pair<Node<T,_Order>*, int> (temp,i));
				temp=temp->links[i];i=0;
			}
			else found_pos = true;
		}

		if(temp->insert(new_key,i) == false)			//here i : position to insert new element
		{
			int median = (_Order-1)/2;
			split(temp, median,i, new_key, back_trace);
		}
	}
}

template <typename T, int _Order , typename _Predicate>
Node<T,_Order>* Tree<T,_Order,_Predicate>::split(Node<T,_Order>* big_child, int median, int pos, T new_key, stack<pair<Node<T,_Order>*, int> > &back_trace)
{
	Node<T,_Order> *parent,*new_rhs, *new_parent=0,*return_value;
	int parent_pos,i,j;
	bool new_root_created = false;

	if(back_trace.empty())
	{
		parent_pos = 0;
		parent =  new Node<T,_Order>();
		new_root_created=true;
	}
	else
	{
		parent = back_trace.top().first;
		parent_pos = back_trace.top().second;
	}

	new_rhs =  new Node<T,_Order>();

	if(median == pos)
	{
		for(i=median, j=0;i<_Order-1;++i,++j)
		{
			new_rhs->insert(big_child->keys[i],j);
			new_rhs->links[j] = big_child->links[i];
		}
		new_rhs->links[j] = big_child->links[i];
		big_child->num_of_elem -= j;

		if(parent->insert(new_key,parent_pos) == false)
		{
			back_trace.pop();
			new_parent = split(parent,median,parent_pos,new_key,back_trace);
			setlinks(parent,new_parent,big_child,new_rhs,parent_pos,median);
		}
		else
			setlinks(parent,big_child,new_rhs,parent_pos);

		return_value = new_rhs;
	}
	else if(median > pos)
	{
		for(i=median, j=0;i<_Order-1;++i,++j)
		{
			new_rhs->insert(big_child->keys[i],j);
			new_rhs->links[j] = big_child->links[i];
		}
		new_rhs->links[j] = big_child->links[i];
		big_child->num_of_elem = big_child->num_of_elem - j -1;


		if(parent->insert(big_child->keys[median-1],parent_pos) == false)
		{
			back_trace.pop();
			new_parent = split(parent,median,parent_pos,big_child->keys[median-1],back_trace);
			big_child->insert(new_key,pos);
			setlinks(parent,new_parent,big_child,new_rhs,parent_pos,median);
		}
		else
		{
			big_child->insert(new_key,pos);
			setlinks(parent,big_child,new_rhs,parent_pos);
		}


		return_value = big_child;
	}
	else
	{
		for(i=median+1, j=0;i<_Order-1;++i,++j)
		{
			new_rhs->insert(big_child->keys[i],j);
			new_rhs->links[j] = big_child->links[i];
		}
		new_rhs->links[j] = big_child->links[i];
		big_child->num_of_elem = big_child->num_of_elem - j -1;
		new_rhs->insert(new_key,pos-median-1);

		if(parent->insert(big_child->keys[median],parent_pos) == false)
		{
			back_trace.pop();
			new_parent = split(parent,median,parent_pos,big_child->keys[median],back_trace);
			setlinks(parent,new_parent,big_child,new_rhs,parent_pos,median);
		}
		else
			setlinks(parent,big_child,new_rhs,parent_pos);

		return_value = new_rhs;
	}

	if(new_root_created)
		root = parent;

	return return_value;
}

template <typename T, int _Order , typename _Predicate>
void Tree<T,_Order,_Predicate>::setlinks(Node<T,_Order>* parent,Node<T,_Order>* new_parent,Node<T,_Order>* big_child,Node<T,_Order>* new_rhs, int parent_pos,int median)	//when both big child and big parent are split
{
	if(parent_pos == median)
	{
		if(big_child->isempty())
			parent->links[parent_pos] = 0;
		else
			parent->links[parent_pos] = big_child;
		new_parent->links[0] = new_rhs;
	}
	else if(parent_pos < median)
	{
		if(big_child->isempty())
			new_parent->links[parent_pos] = 0;
		else
			new_parent->links[parent_pos] = big_child;
		new_parent->links[parent_pos+1] = new_rhs;
	}
	else
	{
		if(big_child->isempty())
			new_parent->links[parent_pos -median -1] = 0;
		else
			new_parent->links[parent_pos -median -1] = big_child;
		new_parent->links[parent_pos-median] = new_rhs;
	}
}

template <typename T, int _Order , typename _Predicate>
void Tree<T,_Order,_Predicate>::setlinks(Node<T,_Order>* parent,Node<T,_Order>* big_child,Node<T,_Order>* new_rhs,int parent_pos)		//when only big child is split
{
	if(big_child->isempty())
		parent->links[parent_pos] = 0;
	else
		parent->links[parent_pos] = big_child;

	parent->links[parent_pos+1] = new_rhs;
}

template <typename T, int _Order , typename _Predicate>
void Tree<T,_Order,_Predicate>::disp() const
{
	inorder_disp(root);
}

template <typename T, int _Order , typename _Predicate>
void Tree<T,_Order,_Predicate>::inorder_disp(Node<T,_Order> *temp) const
{
	if(temp)
	{
		for(int i=0;i<temp->num_of_elem;++i)
		{
			if(i == 0)
				inorder_disp(temp->links[i]);
			std::cout << temp->keys[i]<< " " ;
			inorder_disp(temp->links[i+1]);
		}
	}
}

template <typename T, int _Order , typename _Predicate>
void Tree<T,_Order,_Predicate>::show_deque(deque<Node<T,_Order>*> &dq) const
{
	if(!dq.empty())
	{
		int size = dq.size(),j;
		for(int i=0; i< size;++i)
		{
			Node<T,_Order>* temp = dq.front();
			dq.pop_front();
			if(temp)
			{
				for(j=0;j<temp->num_of_elem;++j)
				{
					std::cout<<temp->keys[j] << " ";
					dq.push_back(temp->links[j]);
				}
				dq.push_back(temp->links[j]);
				std::cout<< "\t";
			}
		}
		std::cout << "\n";
		show_deque(dq);
	}
}

template <typename T, int _Order , typename _Predicate>
void Tree<T,_Order,_Predicate>::disp_like_tree() const
{
	deque<Node<T,_Order>* > dq;
	dq.push_back(root);
	show_deque(dq);
}

template <typename T, int _Order , typename _Predicate>
void Tree<T,_Order,_Predicate>::free_all_nodes(Node<T,_Order>* temp)
{
	if(temp)
	{
		for(int i=0;i<temp->num_of_elem;++i)
		{
			if(i ==0 )
				free_all_nodes(temp->links[i]);
			free_all_nodes(temp->links[i+1]);
		}
		delete temp;
	}
}

template <typename T, int _Order , typename _Predicate>
Tree<T,_Order,_Predicate>::~Tree()
{
	free_all_nodes(root);
	root =0;
}

/**************************************************************************************************************/

template<typename T>
class More
{
public:
	bool operator()(const T& a, const T& b)
	{
		return a > b;
	}
};

template<typename T>
class Less
{
public:
	bool operator()(const T& a, const T& b)
	{
		return a < b;
	}
};


#endif /* BTREE_H */
