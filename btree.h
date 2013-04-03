/*
 * btree.h
 *      Author: akshay hebbar
 */

#ifndef BTREE_H
#define BTREE_H

#include<stack>

/*
 * Things to do:
 * 		insertion : done
 * 		Traversal and display : done
 * 		find 			//- return iterator
 * 		bulk-loading
 * 		deletion
 * 		iterator
 * 		may be providing a predicate for sort
 * 		improving the code efficiency
 * 		<< operator overloading for Node
 * 		making it generic
 */

class Tree;

class Node
{
private:
	Node(int n);
	bool insert(int new_key, int pos);
	~Node();

	bool isempty()
	{
		return !num_of_elem;
	}

	int order;
	int num_of_elem;
	int *keys;
	Node* *links;

	friend class Tree;
};

class Tree
{
public:													//interface to btree
	Tree(int n);
	void push(int new_key);
	bool find(int key) const;
	void disp() const;
	~Tree();

	class Iterator
	{
	public:
		Iterator(Node *Nptr, int position)
		{
			ptr = Nptr;
			pos = position;
		}
		int operator*()
		{
			return ptr->keys[pos];
		}
	private:
		Node *ptr;
		int pos;
	};

	Iterator begin();
	Iterator end();

private:
	void inorder_disp(Node*) const;
	Node* split(Node*, int,int,int,stack<pair<Node*, int> > &);
	void setlinks(Node*,Node*,Node*,Node*,int,int);					//when parent gets split
	void setlinks(Node*,Node*,Node*,int);								//when only big child is split

private:
	int order_;
	Node* root;
};

/**************************************************************************************************************/

Node::Node(int n)
{
	order= n;
	num_of_elem = 0;
	keys = new int[n-1];
	links = new Node*[n];
	for(int i=0;i<n;++i)
		links[i] = 0;
}

bool Node::insert(int new_key, int pos)
{
	if(num_of_elem < (order-1))						//handles both insertions : 1)insert at end	2)insert in middle
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

Node::~Node()
{
	delete [] keys;
	delete [] links;
}

/**************************************************************************************************************/

Tree::Tree(int n):order_(n),root(0)
{}

void Tree::push(int new_key)
{
	bool found_pos = false;
	if(root == NULL)
	{
		root = new Node(order_);
		root->insert(new_key,0);
	}
	else
	{
		Node *temp = root;
		stack <pair<Node*, int> > back_trace;
		int i=0;
		while(found_pos != true)
		{
			if(i < temp->num_of_elem && new_key > temp->keys[i])
			{
				++i;
				if(i == (order_- 1))
				{
					if(temp->links[i])
					{
						back_trace.push( pair<Node*, int> (temp,i));
						temp=temp->links[i]; i=0;
					}
					else
						found_pos = true;
				}
			}
			else if(temp->links[i])
			{
				back_trace.push(pair<Node*, int> (temp,i));
				temp=temp->links[i];i=0;
			}
			else found_pos = true;
		}
//		std::cout << "\nPushing : " << new_key << " in pos : "<<i << " of " << temp << " which has " << temp->num_of_elem<<endl;
		if(temp->insert(new_key,i) == false)			//here i : position to insert new element
		{
			int median = (order_-1)/2;
			split(temp, median,i, new_key, back_trace);
		}
	}
}

Node* Tree::split(Node* big_child, int median, int pos, int new_key, stack<pair<Node*, int> > &back_trace)
{
	Node *parent,*new_rhs, *new_parent=0,*return_value;
	int parent_pos,i,j;
	bool new_root_created = false;

//	std::cout << "\nTrying to insert : " << new_key <<endl;


	if(back_trace.empty())
	{
		parent_pos = 0;
		parent =  new Node(order_);
		new_root_created=true;
	}
	else
	{
		parent = back_trace.top().first;
		parent_pos = back_trace.top().second;
	}

//	std::cout << "\nPushing : " << new_key << " in pos : "<<pos << " of " << parent << " which has " << parent->num_of_elem << " elem that is " << parent->keys[0]<<endl;


	new_rhs =  new Node(order_);

	if(median == pos)
	{
		for(i=median, j=0;i<order_-1;++i,++j)
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
		for(i=median, j=0;i<order_-1;++i,++j)
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
			setlinks(parent,new_parent,big_child,new_rhs,parent_pos,median);
		}
		else
		{
			parent->links[parent_pos] = big_child;
			parent->links[parent_pos+1] = new_rhs;
		}

		big_child->insert(new_key,pos);

		return_value = big_child;
	}
	else
	{
		for(i=median+1, j=0;i<order_-1;++i,++j)
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

#if 0
	std::cout << "\nbig child : \n";
	for(int i=0;i<big_child->num_of_elem;++i)
		std::cout << big_child->keys[i]<<" ";
	std::cout << "\nrhs : \n";
	for(int i=0;i<new_rhs->num_of_elem;++i)
		std::cout << new_rhs->keys[i]<<" ";
	std::cout << "\nparent : \n";
	for(int i=0;i<parent->num_of_elem;++i)
		std::cout << parent->keys[i]<<" ";
	if(new_parent != NULL)
	{	std::cout << "\nnew parent : \n";
		for(int i=0;i<new_parent->num_of_elem;++i)
			std::cout << new_parent->keys[i]<<" ";
	}
	std::cout << "\nRoot : " << root  << " no of keys : " << root->num_of_elem << " first : " << root->keys[0] << " first link : " << root->links[0]<<endl;
	for(int i=0;i<root->num_of_elem;++i)
		std::cout << root->keys[i]<<" ";

	std::cout << "\n****************************************\n";
#endif

	return return_value;
}

void Tree::setlinks(Node* parent,Node* new_parent,Node* big_child,Node* new_rhs, int parent_pos,int median)
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
#if 0
	if(parent_pos == median)
	{
		parent->links[parent_pos] = big_child;
		new_parent->links[0] = new_rhs;
	}
	else if(parent_pos < median)
	{
		new_parent->links[parent_pos] = big_child;
		new_parent->links[parent_pos+1] = new_rhs;
	}
	else
	{
		new_parent->links[parent_pos -median -1] = big_child;
		new_parent->links[parent_pos-median] = new_rhs;
	}
#endif
}

void Tree::setlinks(Node* parent,Node* big_child,Node* new_rhs,int parent_pos)
{
#if 1
	if(big_child->isempty())
		parent->links[parent_pos] = 0;
	else
		parent->links[parent_pos] = big_child;

	parent->links[parent_pos+1] = new_rhs;
#endif

#if 0
	parent->links[parent_pos] = big_child;
	parent->links[parent_pos+1] = new_rhs;
#endif
}

void Tree::disp() const
{
	inorder_disp(root);
}

void Tree::inorder_disp(Node *temp) const
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

bool Tree::find(int key) const
{
	if(key == 0)
		return true;
	return false;
}

Tree::~Tree()
{
	delete root;
}

#endif /* BTREE_H */
