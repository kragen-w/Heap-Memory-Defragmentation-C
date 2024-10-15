#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dumalloc.h"
#define HEAP_SIZE (128*8)
#define FIRST_FIT 0
#define BEST_FIT 1
unsigned char FIT;
unsigned char MANAGED_SIZE;//might be wrong


unsigned char heap1[HEAP_SIZE];
unsigned char heap2[HEAP_SIZE];
unsigned char heap3[HEAP_SIZE];
unsigned char* heap[3] = {heap1, heap2, heap3};
unsigned char currentHeap = 0;


void duInitMalloc(unsigned char fitType);
void* duMalloc(int size, unsigned char currentHeap);
void duFree(void* ptr);
void mallocBlockDumper(int heapNum);
void mallocDumpStringHelper(int heapNum);
void freeList(int heapNum);


typedef struct memoryBlockHeader {
    int free; // 0 - used, 1 = free
    int size; // size of the reserved block
    int managedIndex; // the unchanging index in the managed array
    int survivalAmt; // the number of times the block has moved between young heaps
    struct memoryBlockHeader* next;
} memoryBlockHeader;


memoryBlockHeader* freeListHead1;
memoryBlockHeader* freeListHead2;
memoryBlockHeader* freeListHead3;


memoryBlockHeader* managedList[128];


void duManagedInitMalloc(int searchType){
    MANAGED_SIZE = 0;
    // there are some other things here i forgot to do for part 2, managed size and making managed list
    duInitMalloc(searchType);

}

void duInitMalloc(unsigned char fitType){
    for (int i = 0; i < HEAP_SIZE; i++){
        (heap[0])[i] = 0;
        (heap[1])[i] = 0;
        (heap[2])[i] = 0;
    }
    memoryBlockHeader* currentBlock = (memoryBlockHeader*)(heap[currentHeap]);
    currentBlock->size = HEAP_SIZE - sizeof(memoryBlockHeader);
    currentBlock->next = NULL;
    currentBlock->free = 1;

    if (currentHeap == 0){
        freeListHead1 = currentBlock;
    } else {
        freeListHead2 = currentBlock;
    }

    memoryBlockHeader* currentBlock2 = (memoryBlockHeader*)(heap[2]);
    currentBlock2->size = HEAP_SIZE - sizeof(memoryBlockHeader);
    currentBlock2->next = NULL;
    currentBlock2->free = 1;
    freeListHead3 = currentBlock2;
    
    

    if (fitType == 0){
        FIT = 0;
    } else {
        FIT = 1;
    }
    
   
    
}

void mallocBlockDumperHelper(int heapNum){
    printf("Memory Block:\n");
    memoryBlockHeader* currentHeader = (memoryBlockHeader*)(heap[heapNum]);
    while (currentHeader < (memoryBlockHeader*)(HEAP_SIZE + (unsigned char*)(heap[heapNum]))){
        if (currentHeader->free){//free
            printf("Free at ");
        } else {
            printf("Used at ");
        }
        printf("%p, size %d, surv: %d\n", ((memoryBlockHeader*)(currentHeader)) + 1, currentHeader->size, currentHeader->survivalAmt); //part 2 the +1 is to show address of actual data, not the header

        currentHeader = (memoryBlockHeader*)((unsigned char*)(currentHeader) + currentHeader->size + sizeof(memoryBlockHeader));

    }
}

void mallocDumpStringHelper(int heapNum){
    char string[128];
    unsigned char letter = 97;
    unsigned char count = 0;
    memoryBlockHeader* currentHeader = (memoryBlockHeader*)(heap[heapNum]);
    while (currentHeader < (memoryBlockHeader*)(HEAP_SIZE + (unsigned char*)(heap[heapNum]))){
            
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

void freeList(int heapNum){
    printf("Free list:\n");
    memoryBlockHeader* currentFreeBlock;
    if (heapNum == 0){
        currentFreeBlock = freeListHead1;
    } else if (heapNum == 1){
        currentFreeBlock = freeListHead2;
    } else {
        currentFreeBlock = freeListHead3;
    }
        while (1){
            printf("Free block at %p, size, %d.\n", (memoryBlockHeader*)(currentFreeBlock) + 1, currentFreeBlock->size);
            if (currentFreeBlock->next == NULL){
                break;
            }
            currentFreeBlock = currentFreeBlock->next;
        }
}

void duMemoryDump(){
    // printf("Free list 3 size: %d\n", freeListHead3->size);
    
    printf("MEMORY DUMP\n");
    printf("Current Young Heap: %d\n", currentHeap);
    mallocBlockDumperHelper(currentHeap);
    mallocDumpStringHelper(currentHeap);
    freeList(currentHeap);
    printf("Old Heap\n");
    int pastHeap = currentHeap;
    currentHeap = 2;
    mallocBlockDumperHelper(currentHeap);
    mallocDumpStringHelper(currentHeap);
    freeList(currentHeap);
    currentHeap = pastHeap;
    managedMemoryDump();

   
}

void** duManagedMalloc(int size){
    managedList[MANAGED_SIZE] = (memoryBlockHeader*)(duMalloc(size, currentHeap)); //might be wrong part 2, - memory block header size to point to header rather than data
    MANAGED_SIZE++;
    return (void**)(&(managedList[MANAGED_SIZE - 1]));

}


void* duMalloc(int size, unsigned char currentHeap){
    int nearestEight = (((size-1) / 8) + 1) * 8; //finds the byte value for just the data
    memoryBlockHeader* freeListHead;
    if (currentHeap == 0){
        freeListHead = freeListHead1;
    } else if (currentHeap == 1){
        freeListHead = freeListHead2;
    } else {
        freeListHead = freeListHead3;
    }
    
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
            newFreeBlock->survivalAmt = 0;
            
            

            newFreeBlock->size = (currentFreeBlock->size - nearestEight - sizeof(memoryBlockHeader));
            currentFreeBlock->size = nearestEight;
            currentFreeBlock->free = 0;

            newFreeBlock->next = currentFreeBlock->next;
            freeListHead = newFreeBlock;
            currentFreeBlock->next = NULL;

            if (currentHeap == 0){
                freeListHead1 = freeListHead;
            } else if (currentHeap == 1){
                freeListHead2 = freeListHead;
            } else {
                freeListHead3 = freeListHead;
            }
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
            newFreeBlock->survivalAmt = 0;
            

            currentFreeBlock->size = nearestEight;
            currentFreeBlock->free = 0;

            newFreeBlock->next = currentFreeBlock->next;
            previousFreeBlock->next=newFreeBlock;
            currentFreeBlock->next = NULL;


            if (currentHeap == 0){
                freeListHead1 = freeListHead;
            } else if (currentHeap == 1){
                freeListHead2 = freeListHead;
            } else {
                freeListHead3 = freeListHead;
            }

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
            newFreeBlock->survivalAmt = 0;
            

            
            closest_block->size = nearestEight;
            closest_block->free = 0;

            newFreeBlock->next = closest_block->next;
            freeListHead = newFreeBlock;
            closest_block->next = NULL;


            if (currentHeap == 0){
                freeListHead1 = freeListHead;
            } else {
                freeListHead2 = freeListHead;
            }

            closest_block->managedIndex = MANAGED_SIZE;
            return (void*)(closest_block + 1);
        }
        
       
        if (foundBlock == 0){
            return NULL;
        }
        // CASE IF THERE IS ROOM, PUTTING BLOCK BETWEEN TWO BLOCKS
        else{
            
           
            
            memoryBlockHeader* newFreeBlock = (memoryBlockHeader*)((unsigned char*)(closest_block) + nearestEight + sizeof(memoryBlockHeader));
            newFreeBlock->size = (closest_block->size - nearestEight - sizeof(memoryBlockHeader));
            newFreeBlock->free = 1;
            newFreeBlock->survivalAmt = 0;
            
            closest_block->size = nearestEight;
            closest_block->free = 0;

            memoryBlockHeader* previousFreeBlock = freeListHead;
            for (int i = 0; i < place_of_block - 1; i++){
                previousFreeBlock = previousFreeBlock->next;
            }

            newFreeBlock->next = closest_block->next;
            previousFreeBlock->next=newFreeBlock;
            closest_block->next = NULL;
            
            if (currentHeap == 0){
                freeListHead1 = freeListHead;
            } else if (currentHeap == 1){
                freeListHead2 = freeListHead;
            } else {
                freeListHead3 = freeListHead;
            }
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
    unsigned char oldFree = 0;
    memoryBlockHeader* freeListHead;
    if (currentHeap == 0){
        freeListHead = freeListHead1;
    } else if (currentHeap == 1){
        freeListHead = freeListHead2;
    }

    memoryBlockHeader* addressToFree = (memoryBlockHeader*)((unsigned char *)ptr - sizeof(memoryBlockHeader));

    if (addressToFree > (memoryBlockHeader*)(HEAP_SIZE + (unsigned char*)(heap[currentHeap])) || addressToFree < (memoryBlockHeader*)(heap[currentHeap])){
        // OLD HEAD TIME
        freeListHead = freeListHead3;
        oldFree = 1;
    }


    memoryBlockHeader* currentFreeBlock = freeListHead;
   
    if (currentFreeBlock > addressToFree){//freed block is head
       
        memoryBlockHeader** pointerToFreeHead = &freeListHead;
        addressToFree->next = *pointerToFreeHead;
        *pointerToFreeHead = addressToFree;
        addressToFree->free = 1;
        addressToFree->survivalAmt = 0;


    }else{
        while (currentFreeBlock->next < addressToFree){
            currentFreeBlock = currentFreeBlock->next;
        }
       
       

        memoryBlockHeader** pointerToCurrentBlock = &currentFreeBlock;
        addressToFree->next = (*pointerToCurrentBlock)->next;//putting it back
        addressToFree->free = 1;
        addressToFree->survivalAmt = 0;
        (*pointerToCurrentBlock)->next = addressToFree;
    }

    if (oldFree == 1){
        freeListHead3 = freeListHead;
    } else if (currentHeap == 0){
        freeListHead1 = freeListHead;
    } else if (currentHeap == 1){
        freeListHead2 = freeListHead;
    }  
    

  // FIRST BLOCK IS FREED
  
    

}

void minorCollection(){
    if (currentHeap == 0){
        currentHeap = 1;
    } else {
        currentHeap = 0;
    }
    memoryBlockHeader* placeToMove = (memoryBlockHeader*)(heap[currentHeap]);
    unsigned int space_left = HEAP_SIZE;
    for (int i = 0; i < MANAGED_SIZE; i++){
        if (managedList[i] != NULL){
            if (((memoryBlockHeader*)(managedList[i]) - 1)->survivalAmt >= 2){//should be 3 max
                // old head time
                memoryBlockHeader* mallocedActualDataLocation = (memoryBlockHeader*)(duMalloc(((memoryBlockHeader*)(managedList[i]) - 1)->size, 2));
                memcpy(mallocedActualDataLocation,(memoryBlockHeader*)(managedList[i]), ((memoryBlockHeader*)(managedList[i]) - 1)->size);//using the head that dumalloc gave us part 4
                managedList[i] = (memoryBlockHeader*)(mallocedActualDataLocation);



            } else {
                memcpy(placeToMove,(memoryBlockHeader*)(managedList[i]) - 1, ((memoryBlockHeader*)(managedList[i]) - 1)->size + sizeof(memoryBlockHeader));


                placeToMove->free = 0;
                placeToMove->size = ((memoryBlockHeader*)(managedList[i]) - 1)->size;
                placeToMove->survivalAmt += 1;
                managedList[i] = (memoryBlockHeader*)(placeToMove) + 1;
                space_left -= (placeToMove->size + sizeof(memoryBlockHeader));
                placeToMove = (memoryBlockHeader*)((unsigned char*)(placeToMove) + placeToMove->size + sizeof(memoryBlockHeader));
            }
            
            
            
        }
        
    }
    if (currentHeap == 0){
        freeListHead1 = placeToMove;
        freeListHead1->size = space_left;
        freeListHead1->free = 1;
        freeListHead1->next = NULL;
        freeListHead1->survivalAmt = 0;
    } else {
        freeListHead2 = placeToMove;
        freeListHead2->size = space_left;
        freeListHead2->free = 1;
        freeListHead2->next = NULL;
        freeListHead2->survivalAmt = 0;
    }

}

void majorCollection(){
    memoryBlockHeader* currentBlock = freeListHead3;
    




    while ((char*)currentBlock + currentBlock->size + sizeof(memoryBlockHeader) < (char*)(heap[2]) + HEAP_SIZE){
        char* nextHeader = (char*) currentBlock + currentBlock->size + sizeof(memoryBlockHeader);
        memoryBlockHeader* nextBlock = (memoryBlockHeader*) nextHeader;

        if (nextBlock->free == 1){// next block is free
            currentBlock->size += nextBlock->size + sizeof(memoryBlockHeader);//combining the two blocks
            currentBlock->next = nextBlock->next; //skipping over the next block
            nextBlock->next = NULL;
            continue;

        }
        memoryBlockHeader temp = *currentBlock;

        //Manually copying the header over 
        currentBlock->free = 0;
        currentBlock->managedIndex = nextBlock->managedIndex;
        currentBlock->size = nextBlock->size;
        currentBlock->next = NULL;
        currentBlock->survivalAmt = nextBlock->survivalAmt;

        char* firstTo = (char*) currentBlock + sizeof(memoryBlockHeader);
        char* firstFrom = (char*) nextBlock + sizeof(memoryBlockHeader);

        memcpy(firstTo, firstFrom, nextBlock->size);

        char* insertNewHeader = (char*) firstTo + currentBlock->size;
        memoryBlockHeader* newHeader = (memoryBlockHeader*) insertNewHeader;

        newHeader->free = 1;
        newHeader->size = temp.size;
        newHeader->managedIndex = temp.managedIndex;
        newHeader->survivalAmt = temp.survivalAmt;
        newHeader->next = temp.next;

        freeListHead2 = newHeader;
        currentBlock = newHeader;
            
        


    }

}
