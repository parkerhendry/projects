#ifndef TREE_H
#define TREE_H

//Template class for binary search tree
template <class T>
class Tree
{
	//Nodes in the tree
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
		void callPrint();
		void inOrder(Node*);
		void preOrder(Node*);
		void postOrder(Node*);
		
};

#endif
