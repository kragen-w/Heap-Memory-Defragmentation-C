mallocTestVersion1: mallocTestVersion1.o duMalloc.o
	gcc mallocTestVersion1.o duMalloc.o -o mallocTestVersion1

# what can effect this? what is included in this? what gets pasted?

mallocTestVersion1.o: mallocTestVersion1.c dumalloc.h
	gcc mallocTestVersion1.c -c

duMalloc.o: duMalloc.c dumalloc.h
	gcc duMalloc.c -c

clean:
	rm -f *.o
	rm -f mallocTest
# this just removes all dat shit, -f gets rid of any warning

# gcc mallocTest.c -MM gives you all of the things that it takes to make mallocTest.c