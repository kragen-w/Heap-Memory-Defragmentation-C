#ifndef DUMALLOC_H
#define DUMALLOC_H

// The interface for DU malloc and free

#define Managed(p) (*p) // these might be in the wrong spot part 2
#define Managed_t(t) t*

#define FIRST_FIT 0
#define BEST_FIT 1


void duManagedInitMalloc(int searchType);
void** duManagedMalloc(int size);
void duManagedFree(void** mptr);



void duMemoryDump();

#endif