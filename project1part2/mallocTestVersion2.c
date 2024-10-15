// This is the testing code for version2
// It tests the Managed interface for our memory allocation

#include <stdio.h>  // printf
#include <stdlib.h>  // exit



// Load in the dumalloc interface
// Will need to be compiled with the dumalloc code as well
//   but for this lab we use a makefile to create a library
//   and link with that.  See the makefile.
#include "dumalloc.h"

void test() {
	// Without the managed macros
	printf("\nduMalloc a0\n");
	char** a0 = (char**)duManagedMalloc(48);
	if (a0 == NULL) {
		printf("Call to DuMalloc failed\n");
		exit(1);
	}
	printf("a0 memory addres is: %p\n", a0);
	duMemoryDump();

	// Test to store and retrieve data from a1
	// without the managed macro
	(*a0)[0] = 'D';
	(*a0)[1] = 'e';
	(*a0)[2] = 'n';
	(*a0)[3] = 'v';
	(*a0)[4] = 'e';
	(*a0)[5] = 'r';
	(*a0)[6] = '\0';
	printf("\nMemory access is: %s\n", *a0);

	printf("\nduFree a0\n");
	duManagedFree((void**)a0);
	duMemoryDump();

	// All the rest of the test include the managed macros to do
	//   the extra pointers and dereference
	printf("\nduMalloc a1\n");
	Managed_t(char *) a1 = (Managed_t(char*))duManagedMalloc(64);
	if (a1 == NULL) {
		printf("Call to DuMalloc failed\n");
		exit(1);
	}
	duMemoryDump();

	// Test to store and retrieve data from a1
	Managed(a1)[0] = 'D';
	Managed(a1)[1] = 'e';
	Managed(a1)[2] = 'n';
	Managed(a1)[3] = 'v';
	Managed(a1)[4] = 'e';
	Managed(a1)[5] = 'r';
	Managed(a1)[6] = '\0';
	printf("\nMemory access is: %s\n", Managed(a1));

	printf("\nduMalloc a2\n");
	Managed_t(char*) a2 = (Managed_t(char*))duManagedMalloc(80);
	if (a2 == NULL) {
		printf("Call to DuMalloc failed\n");
		exit(1);
	}
	duMemoryDump();

	printf("\nduFree a2\n");
	duManagedFree((Managed_t(void*))a2);
	// or duManagedFree((void**)a3);
	duMemoryDump();
}

int main(int argc, char* argv[]) {

	// Must be first call in the program to get DuMalloc going
	printf("\nduInitMalloc\n");
	// We will always use FIRST_FIT going forward just to focus
	//   our testing on the latest things we have implemented
	duManagedInitMalloc(FIRST_FIT);
	duMemoryDump();

	test();
}
