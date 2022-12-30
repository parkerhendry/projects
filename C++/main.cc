//Parker Hendry
/*
This program is an implementation of a binary search tree. This implementation uses a template class for the tree enabling it to accept multiple data types. The currently supported data types are int, double, char, and string. After the user is prompted for the data items, the items are inserted into the tree. The tree is printed using preorder traversal(root, left subtree, right subtree), inorder traversal(left subtree, root, right subtree), and postorder traversal(left subtree, right subtree, root).
*/

#include <iostream>
#include <algorithm>
#include "Tree.h"
#include "Tree.cc"
using namespace std;

int main()
{
	
	int n;
	string type;
		
	//Prompt user for size and type
	cout << "How many data items would you like to store?\n";
	cin >> n;
	
	cout << "What is the datatype of these items?\n";
	cin >> type;
	
	//Use transform function from STL to turn type all lowercase
	transform(type.begin(), type.end(), type.begin(), ::tolower);

	//Depending on the type of user input:
	//Create Tree<type> instance
	//Prompt user for data items and insert them
	//Print tree
	if (type == "int")
	{
		Tree<int> foo;
		
		int num;
		
		for (int i = 0; i < n; i++)
		{
			cout << "Enter an int to store: ";
			cin >> num;
			
			foo.insert(num);
		}
		
		foo.callPrint();
	}
	else if (type == "double")
	{
		Tree<double> foo;
		
		double num;
		
		for (int i = 0; i < n; i++)
		{
			cout << "Enter a double to store: ";
			cin >> num;
			
			foo.insert(num);
		}
		
		foo.callPrint();		
		
	}
	else if (type == "char")
	{
		Tree<char> foo;
		
		char c;
		
		for (int i = 0; i < n; i++)
		{
			cout << "Enter a char to store: ";
			cin >> c;
			
			foo.insert(c);
		}
		
		foo.callPrint();		
	}
	else if (type == "string")
	{
		Tree<string> foo;
		
		string s;
		
		cin.ignore();
		
		for (int i = 0; i < n; i++)
		{
			cout << "Enter a string to store: ";
			getline(cin, s);
			
			foo.insert(s);
		}
		
		foo.callPrint();
	}
}	
