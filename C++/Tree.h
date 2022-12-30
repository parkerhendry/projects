#ifndef TREE_H
#define TREE_H

template <class T>
class Tree
{
	struct Node
	{
		T data;
		Node *left;
		Node *right;
	};

	private: 
		Node *head;
	
	public:
		Tree() { head = NULL; }
		
		void insert(T);
		void balance();
		void callPrint();
		void print(Node*);
		
};

#endif
