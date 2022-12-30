#include <iostream>
#include "Tree.h"
#include "Tree.cc"
using namespace std;

int main()
{
	Tree<string> fun;
	
	fun.insert("Hello");
	fun.insert("Fish");
	fun.insert("Dog");
	fun.insert("Cat");
	
	/*
	fun.insert(32.4342);
	fun.insert(41.54);	
	fun.insert(3.564);
	fun.insert(13.432);
	*/
	
	fun.callPrint();
}	
