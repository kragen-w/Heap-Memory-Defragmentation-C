// This is the testing code for version3
// It tests the young generation movement with minor collections

#include <stdio.h>  // printf
#include <stdlib.h>  // exit




// Load in the dumalloc interface
// Will need to be compiled with the dumalloc code as well
//   but for this lab we use a makefile to create a library
//   and link with that.  See the makefile.
#include "dumalloc.h"

void test() {
	printf("\nduMalloc a0\n");
	Managed_t(char*) a0 = (Managed_t(char*))duManagedMalloc(128);
	if (a0 == NULL) {
		printf("Call to DuMalloc failed\n");
		exit(1);
	}
	printf("a0 memory addres is: %p\n", a0); //slight chance this is messed up
	duMemoryDump();

	printf("\nduMalloc a1\n");
	Managed_t(char*) a1 = (Managed_t(char*))duManagedMalloc(30);
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

	printf("\nduMalloc a3\n");
	Managed_t(char*) a3 = (Managed_t(char*))duManagedMalloc(72);
	if (a3 == NULL) {
		printf("Call to DuMalloc failed\n");
		exit(1);
	}
	duMemoryDump();

	printf("\nduMalloc a4\n");
	Managed_t(char*) a4 = (Managed_t(char*))duManagedMalloc(88);
	if (a4 == NULL) {
		printf("Call to DuMalloc failed\n");
		exit(1);
	}
	duMemoryDump();

	printf("\nduFree a0\n");
	duManagedFree((Managed_t(void*))a0);
	duMemoryDump();

	printf("\nduFree a2\n");
	duManagedFree((Managed_t(void*))a2);
	duMemoryDump();

	printf("\nduFree a3\n");
	duManagedFree((Managed_t(void*))a3);
	duMemoryDump();

	// a1(32) and a4(88) are live pointers

	// Do a minor collection
	printf("\n********* MINOR COLLECTION ***********\n");
	minorCollection();
	duMemoryDump();

	// See if the memory is still correct in a1 even though it moved
	printf("\nMemory access is: %s\n", Managed(a1));

	printf("\nduMalloc a5\n");
	Managed_t(char*) a5 = (Managed_t(char*))duManagedMalloc(64);
	if (a5 == NULL) {
		printf("Call to DuMalloc failed\n");
		exit(1);
	}
	duMemoryDump();

	printf("\nduFree a4\n");
	duManagedFree((Managed_t(void*))a4);
	duMemoryDump();

	// a1(32) and a5(64) are live pointers

	// Do a minor collection
	printf("\n********* MINOR COLLECTION ***********\n");
	minorCollection();
	duMemoryDump();

}

int main(int argc, char* argv[]) {

	// Must be first call in the program to get DuMalloc going
	printf("\nduInitMalloc\n");
	duManagedInitMalloc(FIRST_FIT);
	duMemoryDump();

	test();
}
