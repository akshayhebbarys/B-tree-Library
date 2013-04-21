/*
 * btree.h
 *      Authors: Akshay hebbar Y S
 */

#ifndef BTREE_H
#define BTREE_H

#include <stack>				//required to store path of traversal
#include <deque>				//required to display our data structure as tree
#include <cstdlib>				//used for 'exit' if client chooses bad '_Order'
#include <vector>				//used for bulk-loading
#include <algorithm>			//to sort bulk loaded data

template<typename> class More;				//forward declaration of predicate
template<typename> class Less;				//predicate

template <typename T, int = 6 , typename = Less<T> > class Tree ;		// default values

template <typename T, int _Order>			//Node class
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

	template <typename _ptr>
		Tree(_ptr begin, _ptr end);


	//---------- Iterator begins ----------

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
					back_trace.pop();
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
					back_trace.pop();
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
						back_trace.pop();

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
						back_trace.pop();

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

	//---------- Iterator ends ------------

	Iterator begin()
	{
		stack <pair<Node<T,_Order>*, int> > trace;
		Node<T,_Order>* temp = root;
		while(temp)
		{
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

	Iterator find(T key)
	{
		if(root == NULL)
		{
			return end();
		}
		else
		{
			Node<T,_Order> *temp = root;
			stack <pair<Node<T,_Order>*, int> > back_trace;
			int low, high, mid;
			while(temp)
			{
				low = 0;
				high = temp->num_of_elem - 1;
				mid = (low + high)/2;
				while(low <= high )
				{
					if(key == temp->keys[mid])
					{
						back_trace.push(pair<Node<T, _Order>*,int >(temp,mid));
						return Iterator(back_trace.top().first,mid,back_trace, root);
					}
					else if(key < temp->keys[mid])
					{
						high = mid - 1;
					}
					else low = mid + 1;

					mid = (low + high)/2;
				}
				if(key < temp->keys[mid])
				{
					back_trace.push(pair<Node<T,_Order>*,int>(temp,mid));
					temp = temp->links[mid];
				}
				else
				{
					back_trace.push(pair<Node<T,_Order>*,int>(temp,mid+1));
					temp = temp->links[mid+1];
				}
			}
			return end();
		}
	}

private:
	Node<T,_Order>* split(Node<T,_Order>*, int,int,T,stack<pair<Node<T,_Order>*, int> > &);
	void setlinks(Node<T,_Order>*,Node<T,_Order>*,Node<T,_Order>*,Node<T,_Order>*,int,int);					//when parent gets split
	void setlinks(Node<T,_Order>*,Node<T,_Order>*,Node<T,_Order>*,int);								//when only big child is split
	template <typename _low, typename _high>
		Node<T, _Order>* link(_low& begin, _high& end);
	template <typename _ptr>
		int get_length(_ptr begin, _ptr end) const;
	void inorder_disp(Node<T,_Order>*) const;
	void show_deque(deque<Node<T,_Order>*> &) const;
	void free_all_nodes(Node<T,_Order>*);											//uses post-order

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
}

/**************************************************************************************************************/

template <typename T, int _Order , typename _Predicate>
Tree<T,_Order,_Predicate>::Tree():root(0)
{
	predicate = _Predicate();
	if(_Order < 2)
	{
		std::cerr << "\nCould not create B-tree\nOrder should be at-least 2 "
				"[ie Number of elements in each node should be at-least 1]\nAborting\n";
		exit(1);
	}
}


template <typename T, int _Order , typename _Predicate>
template <typename _ptr>
Tree<T,_Order, _Predicate>::Tree(_ptr begin, _ptr end)
{
	predicate = _Predicate();
	if(_Order < 2)
	{
		std::cerr << "\nCould not create B-tree\nOrder should be at-least 2 "
				"[ie Number of elements in each node should be at-least 1]\nAborting\n";
		exit(1);
	}

	int length = get_length(begin,end);

	sort(begin,end,predicate);

	int mod = length%_Order;
	int num_of_nodes = (length/_Order) + 1;
	vector < Node<T, _Order>* > leaves(num_of_nodes);
	vector < T > non_leaves;
	if(length < _Order)							// If tree has single node
	{
		this->root = new Node<T, _Order>;
		int j=0;
		while(begin != end && this->root->insert(*begin++,j))
		{
			++j;
		}
	}
	else if(mod >= (_Order-1)/2 )				// If no splitting occurs in base level (leaves)
	{
		for(int i=0; i<num_of_nodes; ++i)
		{
			leaves[i] = new Node<T, _Order>;
			int j=0;
			while(begin != end && leaves[i]->insert(*begin,j))
			{
				++j;
				++begin;
			}
			non_leaves.push_back(*begin++);
		}
		non_leaves.pop_back();
		this->root= link(leaves, non_leaves);		// For linking 	leaves and non-leaves
	}
	else										// splitting occurs in base level
	{
		int i, last;
		last = mod +_Order;
		for (i = 0; i < num_of_nodes-2; ++i)	// Follows normal insertion till last but 2 nodes
		{
			leaves[i] = new Node<T, _Order>;
			int j=0;
			while(leaves[i]->insert(*begin,j))
			{
				++j;
				++begin;
			}
			non_leaves.push_back(*begin++);
		}
		leaves[i] = new Node<T, _Order>;		// creates last but 1 node
		for (int j = 0; j < (last/2); ++j)
		{
			leaves[i]->insert(*begin++, j);
		}
		non_leaves.push_back(*begin++);
		i++;
		leaves[i] = new Node<T, _Order>;		// creates last node
		for(int j=0; begin != end; j++)
		{
			leaves[i]->insert(*begin++, j);
		}
		this->root= link(leaves, non_leaves);
	}
}


template <typename T, int _Order, typename _Predicate>
template <typename _low, typename _high>
Node<T, _Order>* Tree<T,_Order, _Predicate>::link(_low& low_level, _high& high_level)
{
	int high_size = high_level.size();

	if(high_size < _Order)							// base condition : Node is root
	{
		Node<T, _Order>* root = new Node<T, _Order>;
		int i = 0;
		while (i < high_size)
		{
			root->insert(high_level[i],i);
			root->links[i] = low_level[i];
			++i;
		}
		root->links[i] = low_level[i];
		return root;
	}
	else
	{
		int mod = high_size % _Order;
		int num_of_nodes = (high_size/_Order) + 1;

		vector < Node<T, _Order>* > leaves(num_of_nodes);
		vector < T > non_leaves;

		if(mod >= (_Order-1)/2)					// If no splitting occurs in high_level
		{

			int key_count=0, link_count=0;

			for(int i=0; i<num_of_nodes; ++i)
			{
				leaves[i] = new Node<T, _Order>;
				int j=0;
				while(key_count != high_size && leaves[i]->insert(high_level[key_count] ,j))
				{
					leaves[i]->links[j] = low_level[link_count++];
					++j;
					++key_count;
				}
				leaves[i]->links[j] = low_level[link_count++];
				non_leaves.push_back(high_level[key_count++]);
			}
			non_leaves.pop_back();
		}
		else
		{
			int i, last;
			int key_count=0, link_count=0;
			last = mod +_Order;
			for (i = 0; i < num_of_nodes-2; ++i)
			{
				leaves[i] = new Node<T, _Order>;
				int j=0;
				while(leaves[i]->insert(high_level[key_count],j))
				{
					leaves[i]->links[j] = low_level[link_count++];
					++j;
					++key_count;
				}
				leaves[i]->links[j] = low_level[link_count++];
				non_leaves.push_back(high_level[key_count++]);
			}
			leaves[i] = new Node<T, _Order>;
			int j = 0;
			for (; j < (last/2); ++j)
			{
				leaves[i]->insert(high_level[key_count++], j);
				leaves[i]->links[j] = low_level[link_count++];
			}
			leaves[i]->links[j] = low_level[link_count++];
			non_leaves.push_back(high_level[key_count++]);

			i++;

			leaves[i] = new Node<T, _Order>;
			for(j=0; key_count != high_size; j++)
			{
				leaves[i]->insert(high_level[key_count++], j);
				leaves[i]->links[j] = low_level[link_count++];
			}
			leaves[i]->links[j] = low_level[link_count++];
		}
		return link(leaves, non_leaves);
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
			if(i < temp->num_of_elem && predicate(temp->keys[i], new_key))
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

/*
 * 	Called when a node is full
 */
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

	if(big_child->isempty())
		delete big_child;

	return return_value;
}

/*
 *	When both big child and big parent are split
 */
template <typename T, int _Order , typename _Predicate>
void Tree<T,_Order,_Predicate>::setlinks(Node<T,_Order>* parent,Node<T,_Order>* new_parent,Node<T,_Order>* big_child,Node<T,_Order>* new_rhs, int parent_pos,int median)
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


/*
 *	When only big child is split
 */
template <typename T, int _Order , typename _Predicate>
void Tree<T,_Order,_Predicate>::setlinks(Node<T,_Order>* parent,Node<T,_Order>* big_child,Node<T,_Order>* new_rhs,int parent_pos)
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
	std::cout<<"\n";
}

template <typename T, int _Order , typename _Predicate>
void Tree<T,_Order,_Predicate>::inorder_disp(Node<T,_Order> *temp) const
{
	if(temp)
	{
		inorder_disp(temp->links[0]);
		for(int i=0;i<temp->num_of_elem;++i)
		{
			std::cout << temp->keys[i]<< " " ;
			inorder_disp(temp->links[i+1]);
		}
	}
}

template <typename T, int _Order , typename _Predicate>
void Tree<T,_Order,_Predicate>::show_deque(deque<Node<T,_Order>*> &dq) const
{
	while(!dq.empty())
	{
		int size = dq.size(),j;
		for(int i=0; i< size;++i)
		{
			Node<T,_Order>* temp = dq.front();
			dq.pop_front();
			if(temp)
			{
				std::cout<< "|";
				for(j=0;j<temp->num_of_elem;++j)
				{
					std::cout<<temp->keys[j] << "|";
					dq.push_back(temp->links[j]);
				}
				dq.push_back(temp->links[j]);
				std::cout<< "\t";
			}
		}
		std::cout << "\n";
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
		free_all_nodes(temp->links[0]);
		for(int i=0;i<temp->num_of_elem;++i)
			free_all_nodes(temp->links[i+1]);
		delete temp;
	}
}

template <typename T, int _Order , typename _Predicate>
template <typename _ptr>
int Tree<T,_Order,_Predicate>::get_length(_ptr begin, _ptr end) const
{
	int count = 0;
	while(begin != end)
	{
		++begin;
		++count;
	}
	return count;
}

template <typename T, int _Order , typename _Predicate>
Tree<T,_Order,_Predicate>::~Tree()
{
	free_all_nodes(root);
	root =0;
}

/**************************************************************************************************************/
//Predicates

template<typename T>
class Less
{
public:
	bool operator()(const T& a, const T& b)
	{
		return a < b;
	}
};

template<typename T>
class More
{
public:
	bool operator()(const T& a, const T& b)
	{
		return a > b;
	}
};

#endif /* BTREE_H */
