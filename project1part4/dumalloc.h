#ifndef DUMALLOC_H
#define DUMALLOC_H

#define Managed(p) (*p) // these might be in the wrong spot part 2
#define Managed_t(t) t*

#define FIRST_FIT 0
#define BEST_FIT 1

// The interface for DU malloc and free



void duManagedInitMalloc(int searchType);
void** duManagedMalloc(int size);
void duManagedFree(void** mptr);

void minorCollection();
void majorCollection();



void duMemoryDump();

#endif