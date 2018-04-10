// Chapter 3 bonus marks

#include <stdio.h>
#include <stdlib.h>

int hello_world(int n);

int main(int argc, char** argv) 
{
	// Use a for loop to print out Hello World! five times.
	// for (int i = 0; i<5; i++)
	// {
	// 	printf("Hello, world!\n");
	// }

	 // Use a while loop to print out Hello World! five times.
	// int i = 0;
	// while (i < 5)
	// {
	// 	printf("Hello, world!\n");
	// 	i++;
	// }

	if (argc != 2)
	{
		fprintf(stderr, "Usage: chap3 n\n");
		return 1;
	}

	int n = atoi(argv[1]);


	hello_world(n);

	return 0;

}

// Declare a function that outputs Hello World! n number of times. Call this from main.
int hello_world(int n)
{
	for (int i = 0; i < n; i++)
	{
		printf("Hello, world!\n");
	}
	
	return 0;
}

