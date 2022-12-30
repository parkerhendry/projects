#include <iostream>
#include "Tree.h"
using namespace std;

template <class T>
void Tree<T>::insert(T data)
{
	cout << "Inserting " << data << endl;

	//Create new node to be inserted
	Node *newNode = new Node();	
	newNode->data = data;
	newNode->left = NULL;
	newNode->right = NULL;
	
	//Find correct location in tree for insertion
	//Then insert using parent node
	if (head == NULL)
		head = newNode;
	else
	{
		Node *current = head;
		Node *parent = NULL;
		
		while (current != NULL)
		{
			if (data < current->data)
			{
				parent = current;
				current = current->left;
			}
			else
			{
				parent = current;
				current = current->right;
			}	
		}
		
		
		if (parent->data < data)
			parent->right = newNode;
		else
			parent->left = newNode;
		
		
	}
	
		
	
	
}	

//Function used to call printing functions due to recursion
template <class T>
void Tree<T>::callPrint()
{
	cout << "\nIn-order Traversal: \n";
	inOrder(head);
	
	cout << "\nPre-order Traversal: \n";
	preOrder(head);
	
	cout << "\nPost-order Traversal: \n";
	postOrder(head);
}

//Function for inorder traversal
//(left subtree, root, right subtree)
template <class T>
void Tree<T>::inOrder(Node *current)
{
	if (current != NULL)
	{
		inOrder(current->left);
		cout << current->data << endl;
		inOrder(current->right);
	}
}

//Function for preorder traversal
//(root, left subtree, right subtree)
template <class T>
void Tree<T>::preOrder(Node *current)
{
	if (current != NULL)
	{
		cout << current->data << endl;
		preOrder(current->left);
		preOrder(current->right);
	}
}

//Function for posorder traversal
//(left subtree, right subtree, root)
template <class T>
void Tree<T>::postOrder(Node *current)
{
	if (current != NULL)
	{
		postOrder(current->left);
		postOrder(current->right);
		cout << current->data << endl;
	}
}

