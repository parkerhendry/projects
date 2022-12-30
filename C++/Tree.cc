#include <iostream>
#include "Tree.h"
using namespace std;

template <class T>
void Tree<T>::insert(T data)
{
	Node *newNode = new Node;
	
	newNode->data = data;
	newNode->left = NULL;
	newNode->right = NULL;
	
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

template <class T>
void Tree<T>::callPrint()
{
	print(head);
}

template <class T>
void Tree<T>::print(Node *current)
{
	if (current != NULL)
	{
		print(current->left);
		cout << current->data << endl;
		print(current->right);
	}
}

