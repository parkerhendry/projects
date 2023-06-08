//Parker Hendry
/*
This program is an implementation of the lossless data compression algorithm created by David Huffman. The algorithm uses frequencies of symbols to assign variable-length prefix codes. This program will accept a file as a command line argument. Huffman encodings are then assigned to all of the symbols in this file. I used a prority queue in the form of a min-heap to select the smallest frequencies before transforming the heap into a Huffman tree. The bottom up heap construction algorithm was used to transform a complete binary tree into a min-heap. 
*/

#include <stdio.h>
#include <stdlib.h>

//Each node in heap has a symbol and frequency
struct Node 
{
	char symbol;
	int frequency;
	
	struct Node *left;
	struct Node *right;
};	

//Function for swapping nodes in heap
void swap(struct Node **a, struct Node **b)
{
	struct Node *temp = *a;
	
	*a = *b;
	
	*b = temp;
}

//Function for creating a min heap
//This is considered the bottom up heap construction algorithm
void heapify(struct Node **heap, int i, int size)
{
	//root index
	int smallest = i;
		
	//left child index
	int left = 2 * i + 1;
	
	//right child index
	int right = 2 * i + 2;
	
	//If left child has a smaller frequency than root
	if (left < size && heap[left]->frequency < heap[smallest]->frequency)
	{
		smallest = left;
	}
		
	//If right child has a smaller frequency than root
	if (right < size && heap[right]->frequency < heap[smallest]->frequency)
	{
		smallest = right;
	}
		
	//If a child has a smaller frequency than root
	if (smallest != i)
	{
		//swap root and smallest child
		swap(&heap[smallest], &heap[i]);
		
		//sift down until parental dominance is met
		heapify(heap, smallest, size);
	}	
}

//Function to transform min heap into Huffman tree
void createTree(struct Node **heap, int size)
{
	//Nodes for left child, right child, and a new node to serve as the sum of frequencies
	struct Node *left;
	struct Node *right;
	struct Node *new;
	
	int i;

	//While heap is not Huffman tree
	//Fetch 2 smallest frequencies from min-heap
	//Re-heapify
	//Make new node with the sum of the 2 smallest frequencies serving as root
	//Sift up until in correct location
	//Insert new node
	while (size != 1)
	{
		left = heap[0];
		
		heap[0] = heap[size - 1];
		
		size--;
		
		heapify(heap, 0, size);
		
		right = heap[0];
		
		heap[0] = heap[size - 1];
		
		size--;
		
		heapify(heap, 0, size);
		
		new = (struct Node*)malloc(sizeof(struct Node));
		
		new->left = left;
		new->right = right;
		
		new->symbol = '$';
		new->frequency = left->frequency + right->frequency;
		
		i = size;
		size++;	
		
		while (i & new->frequency < heap[(i - 1) / 2]->frequency)
		{
			heap[i] = heap[(i - 1) / 2];
			i = (i - 1) / 2;
		}
		
		heap[i] = new;
	}
	
}

//Function for calculating and printing encodings
void print(struct Node *node, int bar[], int baz)
{
	//If node has left child
	//Assign 0 to left edge
	//Follow left subtree
	if (node->left)
	{
		bar[baz] = 0;
		print(node->left, bar, baz + 1);
	}
	
	//If node has right child
	//Assign 1 to right edge
	//Follow right subtree
	if (node->right)
	{
		bar[baz] = 1;
		print(node->right, bar, baz + 1);
	}
	
	//If node has no children
	//Print node symbol and edge weights leading to that node
	if (!(node->left) && !(node->right))
	{
		printf("%c: ", node->symbol);
		
		for (int i = 0; i < baz; i++)
		{
			printf("%d", bar[i]);
		}
		printf("\n");
		
	}
}

int main(int argc, char *argv[])
{
	//Input validation for command line arguments
	if (argc < 2)
	{
		printf("Please enter a file to compress as a command line argument.\n");
		return -1;
	}
	else if (argc > 2)
	{
		printf("Please enter only 1 file to compress as a command line argument.\n");
		return -1;
	}
	else
	{
		printf("File to compress: %s\n", argv[1]);
	}	

	
	
	FILE *file;
	char ch;
	
	//Attempt to open file from command line argument
	file = fopen(argv[1], "r");	
	if (file == NULL)
	{
		printf("The file entered cannot be found.\n");
		return -1;
	}
		
	//Size of heap
	int size = 0;
	
	//Arrays to hold symbols and their frequencies
	int freq[255] = {0};
	int frequency[255] = {0};
	char symbol[255];
		
	//While read character is not EOF
	//Read character
	//Add symbol
	//Increment frequency
	//Close file
	while (ch != EOF)
	{
		ch = fgetc(file);
		
		freq[ch]++;
		
		if (freq[ch] == 1)
		{
			symbol[size] = ch;
			size++;
		}

	}
	fclose(file);
	
	
	//Use ASCII values of characters get frequencies
	for (int i = 0; i < size; i++)
	{
		frequency[i] = freq[symbol[i]];
	}	
	
	//Allocate mem for heap
	struct Node **heap = (struct Node**)malloc(size * sizeof(struct Node*));
	
	//Create complete binary tree
	for (int i = 0; i < size; i++)
	{
		heap[i] = (struct Node*)malloc(sizeof(struct Node));
		
		heap[i]->left = NULL;
		heap[i]->right = NULL;
		
		heap[i]->symbol = symbol[i];
		heap[i]->frequency = frequency[i];		
	}	
	
	//Heapify the complete binary tree
	for (int i = 0; i < size / 2 - 1; i++)
	{			
		heapify(heap, i, size);		
	}
	
	//Transform heap into Huffman tree
	createTree(heap, size);
		
	//Array to hold encodings of symbols
	int bar[size - 1];
	
	//Print Huffman tree
	print(heap[0], bar, 0);
	
	
	
		
	
	
}
