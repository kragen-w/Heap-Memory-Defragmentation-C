#include <stdio.h>
#include <stdlib.h>
#include "dumalloc.h"
#define HEAP_SIZE (128*8)
#define FIRST_FIT 0
#define BEST_FIT 1
unsigned char FIT;
unsigned char MANAGED_SIZE;//might be wrong


unsigned char heap[HEAP_SIZE];


void duInitMalloc(unsigned char fitType);
void* duMalloc(int size);
void duFree(void* ptr);
void mallocBlockDumper();
void mallocDumpStringHelper();



typedef struct memoryBlockHeader {
    int free; // 0 - used, 1 = free
    int size; // size of the reserved block
    int managedIndex; // the unchanging index in the managed array
    struct memoryBlockHeader* next;
} memoryBlockHeader;

memoryBlockHeader* freeListHead;
memoryBlockHeader* managedList[128];


void duManagedInitMalloc(int searchType){
    MANAGED_SIZE = 0;
    // there are some other things here i forgot to do for part 2, managed size and making managed list
    duInitMalloc(searchType);

}

void duInitMalloc(unsigned char fitType){
    
    for (int i = 0; i < HEAP_SIZE; i++){
        heap[i] = 0;
    }
    memoryBlockHeader* currentBlock = (memoryBlockHeader*)heap;
    currentBlock->size = HEAP_SIZE - sizeof(memoryBlockHeader);
    currentBlock->next = NULL;
    currentBlock->free = 1;
    freeListHead = currentBlock;
    if (fitType == 0){
        FIT = 0;
    } else {
        FIT = 1;
    }
    
   
    
    
}

void mallocBlockDumperHelper(){
    memoryBlockHeader* currentHeader = (memoryBlockHeader*)heap;
    while (currentHeader < (memoryBlockHeader*)(HEAP_SIZE + (unsigned char*)heap)){
        if (currentHeader->free){//free
            printf("Free at ");
        } else {
            printf("Used at ");
        }
        printf("%p, size %d\n", currentHeader + 1, currentHeader->size); //part 2 the +1 is to show address of actual data, not the header

        currentHeader = (memoryBlockHeader*)((unsigned char*)(currentHeader) + currentHeader->size + sizeof(memoryBlockHeader));


    }
}

void mallocDumpStringHelper(){
    char string[128];
    unsigned char letter = 97;
    unsigned char count = 0;
    memoryBlockHeader* currentHeader = (memoryBlockHeader*)heap;
    while (currentHeader < (memoryBlockHeader*)(HEAP_SIZE + (unsigned char*)heap)){
            
        for (int i = 0; i < (currentHeader->size/8) + (sizeof(memoryBlockHeader)/8); i++){
            if (currentHeader->free){//free
                string[count] = (char)letter;
            } else {
                string[count] = (char)letter - 32;
            }
            count++;
        }
        letter++;

        currentHeader = (memoryBlockHeader*)((unsigned char*)(currentHeader) + currentHeader->size + sizeof(memoryBlockHeader));

        
    }
    string[count] = '\0';
    printf("String: %s\n", string);

}

void managedMemoryDump(){
    printf("ManagedList\n");
    for (int i = 0; i < MANAGED_SIZE; i++){
        if (managedList[i] == NULL){
            printf("ManagedList[%d] = (nil)\n", i);
        } else{
            printf("ManagedList[%d] = %p\n", i, managedList[i]); 
        }
        
    }

}

void duMemoryDump(){
    printf("MEMORY DUMP\n");
    printf("Block list:\n");
    mallocBlockDumperHelper();
    mallocDumpStringHelper();
    printf("Free list:\n");
    
    memoryBlockHeader* currentFreeBlock = freeListHead;
    while (1){
        printf("Free block at %p, size, %d.\n", (memoryBlockHeader*)(currentFreeBlock) + 1, currentFreeBlock->size);
        if (currentFreeBlock->next == NULL){
            break;
        }
        currentFreeBlock = currentFreeBlock->next;
    }
    managedMemoryDump();
    

   
}

void** duManagedMalloc(int size){
    managedList[MANAGED_SIZE] = duMalloc(size); //might be wrong part 2, - memory block header size to point to header rather than data
    MANAGED_SIZE++;
    return (void**)(&(managedList[MANAGED_SIZE - 1]));

}


void* duMalloc(int size){
    int nearestEight = (((size-1) / 8) + 1) * 8; //finds the byte value for just the data
    memoryBlockHeader* currentFreeBlock = freeListHead;
    
    
    unsigned char endReached = 0;
    unsigned char foundBlock = 0;

    if (FIT == FIRST_FIT){
        
        if (currentFreeBlock->size >= nearestEight + sizeof(memoryBlockHeader)){
            foundBlock = 1; 
        }
        

        if (currentFreeBlock->next == NULL){
            endReached = 1;
        }
        
        //CASE OF FIRST ELEMENT
        if (foundBlock){ //if the one node works
            memoryBlockHeader* newFreeBlock = (memoryBlockHeader*)((unsigned char*)(currentFreeBlock) + nearestEight + sizeof(memoryBlockHeader));
            newFreeBlock->free = 1;


            newFreeBlock->size = (currentFreeBlock->size - nearestEight - sizeof(memoryBlockHeader));
            currentFreeBlock->size = nearestEight;
            currentFreeBlock->free = 0;

            newFreeBlock->next = currentFreeBlock->next;
            freeListHead = newFreeBlock;
            currentFreeBlock->next = NULL;
            currentFreeBlock->managedIndex = MANAGED_SIZE;
            return (void*)(currentFreeBlock + 1);
        }
        
        memoryBlockHeader* previousFreeBlock = currentFreeBlock;
        currentFreeBlock = currentFreeBlock->next;
        while (!endReached && !foundBlock){
            if (currentFreeBlock->size >= nearestEight + sizeof(memoryBlockHeader)){
                foundBlock = 1;
                break;
            }
            if (currentFreeBlock->next == NULL){
                endReached = 1;
                break;
            }
            currentFreeBlock = currentFreeBlock->next;
            previousFreeBlock = previousFreeBlock->next;
        }

        // CASE IF THERE IS NO ROOM
        if (endReached && !foundBlock){

            
            return NULL;
        }
        // CASE IF THERE IS ROOM, PUTTING BLOCK BETWEEN TWO BLOCKS
        else{
            memoryBlockHeader* newFreeBlock = (memoryBlockHeader*)((unsigned char*)(currentFreeBlock) + nearestEight + sizeof(memoryBlockHeader));
            newFreeBlock->size = currentFreeBlock->size - nearestEight - sizeof(memoryBlockHeader);
            newFreeBlock->free = 1;

            currentFreeBlock->size = nearestEight;
            currentFreeBlock->free = 0;

            newFreeBlock->next = currentFreeBlock->next;
            previousFreeBlock->next=newFreeBlock;
            currentFreeBlock->next = NULL;


         

            currentFreeBlock->managedIndex = MANAGED_SIZE;
            return currentFreeBlock + 1;
        }
    } else if (FIT == BEST_FIT){
        
        unsigned int place_of_block = 0;
        unsigned int counter = 0;
        unsigned int smallest_size = HEAP_SIZE;        
       
        while(currentFreeBlock->next != NULL){
            



            
            if (currentFreeBlock->size >= nearestEight + sizeof(memoryBlockHeader)){
                foundBlock = 1; 
            }
            currentFreeBlock = currentFreeBlock->next;
            counter ++;
            
            
           

            if (currentFreeBlock->size < smallest_size && currentFreeBlock->size >= nearestEight + sizeof(memoryBlockHeader)){
                    place_of_block = counter;
                    smallest_size = currentFreeBlock->size;
                } 
            
            
            
        }
        
        memoryBlockHeader* closest_block = freeListHead;
            for (int i = 0; i < place_of_block; i++){
                closest_block = closest_block->next;
            }
        if (closest_block == freeListHead){ //if best fit is head
            
            memoryBlockHeader* newFreeBlock = (memoryBlockHeader*)((unsigned char*)(closest_block) + nearestEight + sizeof(memoryBlockHeader));
            newFreeBlock->size = (closest_block->size - nearestEight - sizeof(memoryBlockHeader));
            newFreeBlock->free = 1;

            
            closest_block->size = nearestEight;
            closest_block->free = 0;

            newFreeBlock->next = closest_block->next;
            freeListHead = newFreeBlock;
            closest_block->next = NULL;

            closest_block->managedIndex = MANAGED_SIZE;
            return (void*)(closest_block + 1);
        }
        
       
        if (foundBlock == 0){
            printf("Null case 2\n");
            return NULL;
        }
        // CASE IF THERE IS ROOM, PUTTING BLOCK BETWEEN TWO BLOCKS
        else{
            
           
            
            memoryBlockHeader* newFreeBlock = (memoryBlockHeader*)((unsigned char*)(closest_block) + nearestEight + sizeof(memoryBlockHeader));
            newFreeBlock->size = (closest_block->size - nearestEight - sizeof(memoryBlockHeader));
            newFreeBlock->free = 1;
            closest_block->size = nearestEight;
            closest_block->free = 0;

            memoryBlockHeader* previousFreeBlock = freeListHead;
            for (int i = 0; i < place_of_block - 1; i++){
                previousFreeBlock = previousFreeBlock->next;
            }

            newFreeBlock->next = closest_block->next;
            previousFreeBlock->next=newFreeBlock;
            closest_block->next = NULL;
            
            
            closest_block->managedIndex = MANAGED_SIZE;
            return (closest_block + 1);
        }

    }
    
}

void duManagedFree(void** mptr){
    duFree(*mptr);
    *mptr = NULL; //might need to mess around with this part 2, its supposed to make the memory slot null

}

void duFree(void* ptr){
    memoryBlockHeader* addressToFree = (memoryBlockHeader*)((unsigned char *)ptr - sizeof(memoryBlockHeader));
    memoryBlockHeader* currentFreeBlock = freeListHead;
   
    if (currentFreeBlock > addressToFree){//freed block is head
       
        memoryBlockHeader** pointerToFreeHead = &freeListHead;
        addressToFree->next = *pointerToFreeHead;
        *pointerToFreeHead = addressToFree;
        addressToFree->free = 1;


    }else{
       
        while (currentFreeBlock->next < addressToFree){
            currentFreeBlock = currentFreeBlock->next;
        }
       
       

        memoryBlockHeader** pointerToCurrentBlock = &currentFreeBlock;
        addressToFree->next = (*pointerToCurrentBlock)->next;//putting it back
        addressToFree->free = 1;
        (*pointerToCurrentBlock)->next = addressToFree;
    }

  // FIRST BLOCK IS FREED
  
    

}
