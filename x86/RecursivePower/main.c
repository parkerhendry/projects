//Parker Hendry
/*
This program implements a recursive power function in x86 assembly (GNU Asssembler). The main.c file is used as a driver to make the initial call to the power function.
*/

#include <stdio.h>

//Declare power function from power.s
unsigned power(unsigned, unsigned);


int main()
{
	unsigned num, pow;

	//Prompt user for number and power
	printf("Please enter a number: ");
	scanf("%d", &num);
	
	printf("What power would you like %d raised to? ", num);
	scanf("%d", &pow);

	//Call power function and print result
	printf("Result: %d\n", power(num, pow));
}
