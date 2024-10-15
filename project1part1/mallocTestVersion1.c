// This is the testing code for version1
// It tests both the memory dumping functions as well as
//   the difference between FIRST_FIT and BEST_FIT
// Note that you need to comment/uncomment the init in the main
//   to switch between the FIRST_FIT and BEST_FIT and re-compile/run

#include <stdio.h>  // printf
#include <stdlib.h>  // exit

// Load in the dumalloc interface
// Will need to be compiled with the dumalloc code as well
//   but for this lab we use a makefile to create a library
//   and link with that.  See the makefile.

#define FIRST_FIT 0
#define BEST_FIT 1

#include "dumalloc.h"

void test() {
	printf("\nduMalloc a0\n");
	char* a0 = (char*)duMalloc(128);
	if (a0 == NULL) {
		printf("Call to DuMalloc failed\n");
		exit(1);
	}
	duMemoryDump();

	printf("\nduMalloc a1\n");
	char* a1 = (char*)duMalloc(30);
	if (a1 == NULL) {
		printf("Call to DuMalloc failed\n");
		exit(1);
	}
	duMemoryDump();

	printf("\nduMalloc a2\n");
	char* a2 = (char*)duMalloc(80);
	if (a2 == NULL) {
		printf("Call to DuMalloc failed\n");
		exit(1);
	}
	duMemoryDump();

	printf("\nduMalloc a3\n");
	char* a3 = (char*)duMalloc(120);
	if (a3 == NULL) {
		printf("Call to DuMalloc failed\n");
		exit(1);
	}
	duMemoryDump();

	// Heap is: a0(128), a1(32), a2(80), a3(120)

	printf("\nduFree a0\n");
	duFree(a0);
	duMemoryDump();

	// Heap is: free(128), a1(32), a2(80), a3(120)

	printf("\nduFree a2\n");
	duFree(a2);
	duMemoryDump();

	// Heap is: free(128), a1(32), free(80), a3(120)

	// First fit will use the free 128 block leaving:
	//   Heap is: a4(40), free(72), a1(32), free(80), a3(120)
	// Best fit will skip the 128 block and find the 80 block leaving:
	//   Heap is: free(128), a1(32), a4(40), free(24), a3(120)
	printf("\nduMalloc a4\n");
	char* a4 = (char*)duMalloc(40);
	if (a4 == NULL) {
		printf("Call to DuMalloc failed\n");
		exit(1);
	}
	duMemoryDump();
}

int main(int argc, char* argv[]) {

	// Must be first call in the program to get DuMalloc going
	printf("\nduInitMalloc\n");
	// duInitMalloc(FIRST_FIT);
	// OR
	duInitMalloc(BEST_FIT);
	duMemoryDump();

	test();
}
