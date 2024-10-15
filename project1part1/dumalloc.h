#ifndef DUMALLOC_H
#define DUMALLOC_H

// The interface for DU malloc and free
void duInitMalloc(unsigned char fitType);
void* duMalloc(int size);
void duFree(void* ptr);

void duMemoryDump();

#endif