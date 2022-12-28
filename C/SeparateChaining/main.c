//Parker Hendry
/*
This program is an implementation of an integer hash table using the separate chaining collision resolution technique. When maintaining a load factor close to 1, this technique provides very efficient dictionary operations. While in the worst case these operations can degrade to O(n), they have best and average case time efficiencies of O(1).
*/
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "List.h"

int keys = 0;

//Function for inserting into table
void insert(struct List *table[], int n, int num)
{

	//Compute hash address
	int mod = num % n;
		
	//Allocate memory for new node
	struct List *node = (struct List*)malloc(sizeof(struct List));
	node->data = num;
	node->next = NULL;
		

	//Add node to table
	if (table[mod] == NULL)
	{
		table[mod] = node;
	}
	else
	{
		struct List *foo = table[mod];	
			
		while (foo->next != NULL)
		{
			foo = foo->next;	
		}
		
		foo->next = node;			
	}
	
	printf("***Successfully inserted %d at address %d.***\n", num, mod);
}

//Function for deleting from table
void delete(struct List *table[], int n)
{
	int search;
	printf("Enter a number to delete.\n");
	scanf("%d", &search);
	
	//Compute hash address
	int mod = search % n;
	
	int count = 0;
	
	struct List *temp;
	
	//Delete node and deallocate memory
	if (table[mod] == NULL)
	{
		printf("***The number to delete cannot be found at address %d after zero key comparisons.***\n", mod);
		return;
	}
	
	if (table[mod]->data == search)
	{
		count++;
		
		temp = table[mod];
		
		table[mod] = table[mod]->next;
		
		free(temp);
		
		printf("***Successfully deleted at address %d after %d key comparison.***\n", mod, count);
		keys--;
	}
	else
	{
		count++;
		
		struct List *foo = table[mod];
		
		while (foo->next != NULL)
		{	
			count++;
			
			if (foo->next->data == search)
			{
			
				temp = foo->next;
				
				foo->next = foo->next->next;
				
				free(temp);
				
				printf("***Successfully deleted at address %d after %d key comparisons.***\n", mod, count);
				keys--;
				
				return;	
			}
			else
			{
				foo = foo->next;
			}
		}
		
		printf("***The number to delete cannot be found at address %d after %d key comparisons.***\n", mod, count);
		
	} 
	
	
}

//Function for searching hash table
void search(struct List *table[], int n)
{
	int search;
	printf("Enter a number to search for.\n");
	scanf("%d", &search);
	
	//Compute hash address
	int mod = search % n;
	
	struct List *foo = table[mod];
	
	int count = 0;
	
	//Find key
	if (foo == NULL)
	{
		printf("***The number cannot be found at address %d after zero key comparisons.***\n", mod);
		return;
	}
	
	if (foo->data == search)
	{
		count++;
	
		printf("***The number has been found at address %d after %d key comparison.***\n", mod, count);
	}
	else
	{
		count++;
		
		while (foo->next != NULL)
		{
			foo = foo->next;
			
			count++;
			
			if (foo->data == search)
			{
				printf("***The number has been found at address %d after %d key comparisons.***\n", mod, count);
				return;
			}
		}
		
		printf("***The number cannot be found at address %d after %d key comparisons.***\n", mod, count);
		
	}
}

//Function for clearing table and deallocating memory
void clear(struct List *table[], int n)
{
	for (int i = 0; i < n; i++)
	{
	
		if (table[i] != NULL)
		{
			if (table[i]->next == NULL)
			{	
				free(table[i]);
				continue;
			}
		
		
			while (table[i]->next != NULL)
			{	
				struct List *temp = table[i];
				
				table[i] = table[i]->next;
				
				free(temp);
			}
			
			free(table[i]);
		}
	}
	
	printf("***Table successfully cleared.***\n");
}

//Function for copying tables
void copy(struct List *table[], struct List *new[], int n)
{
	for (int i = 0; i < n; i++)
	{
		table[i] = new[i];	
	}
}

//Function for rehashing table and reallocating memory
void rehash(struct List *table[], struct List *new[], int n, int old)
{
	
	for (int i = 0; i < old; i++)
	{
		if (table[i] != NULL)
		{
			if (table[i]->next == NULL)
			{
				insert(new, n, table[i]->data);
				continue;
				
			}		
			
			while (table[i]->next != NULL)
			{	
				struct List *temp = table[i];
				
				table[i] = table[i]->next;
				
				insert(new, n, temp->data);
			}
			
			insert(new, n, table[i]->data);
			
		}
	}
	
	table = realloc(table, n);
	
	for (int i = 0; i < n; i++)
	{
		table[i] = NULL;		
	}
	
	copy(table, new, n);
}

int main()
{	
	int n, num;

	//Prompt user for initial storage
	printf("How many numbers would you like to store in the table initially?\n");
	scanf("%d", &n);
	
	struct List **table = (struct List**)malloc(n * sizeof(struct List));
	
	for (int i = 0; i < n; i++)
	{
		table[i] = NULL;
	}
	
	for (int i = 0; i < n; i++)
	{
		printf("Enter a number to insert: ");
		scanf("%d", &num);
	
		insert(table, n, num);
		keys++;					
		
	}

	char choice;
	
	//Give user choice prompts
	while (choice != 'f')
	{
		printf("\n");
		printf("-----------------------------------\n");
		printf("I: Insert number\n");
		printf("D: Delete number\n");
		printf("S: Search for number\n");
		printf("R: Rehash table (resize)\n");
		printf("F: Clear and finish\n");
		printf("\n");
		printf("Current table size: %d\n", n);
		printf("Current number of keys stored: %d\n", keys);
		printf("Load factor: %f\n", ((double)keys / (double)n));
		printf("-----------------------------------\n");
		printf("Now please enter a letter to perform an operation: \n");
		scanf(" %c", &choice);	
		
		choice = tolower(choice);
		
		if (choice == 'i')
		{
			printf("Enter a number to insert: ");
			scanf("%d", &num);
	
			insert(table, n, num);	
			keys++;
		}
		else if (choice == 'd')
		{
			delete(table, n);
		}
		else if (choice == 's')
		{
			search(table, n);
		}
		else if (choice == 'r')
		{
			int old = n;
	
			printf("What size would you like the new table to be?\n");
			scanf("%d", &n);
	
			struct List **new = (struct List**)malloc(n * sizeof(struct List));
		
			for (int i = 0; i < n; i++)
			{
				new[i] = NULL;
			}
	
			rehash(table, new, n, old);
			
			printf("***Rehashing successful with a new table size of %d.***\n", n);			
		}
			
	}
	
	clear(table, n);		
	
	
	
	
}
