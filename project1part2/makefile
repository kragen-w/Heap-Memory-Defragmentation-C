mallocTestVersion2: mallocTestVersion2.o duMalloc.o
	gcc mallocTestVersion2.o duMalloc.o -o mallocTestVersion2

# what can effect this? what is included in this? what gets pasted?

mallocTestVersion2.o: mallocTestVersion2.c dumalloc.h
	gcc mallocTestVersion2.c -c

duMalloc.o: duMalloc.c dumalloc.h
	gcc duMalloc.c -c

clean:
	rm -f *.o
	rm -f mallocTest
# this just removes all dat shit, -f gets rid of any warning

# gcc mallocTest.c -MM gives you all of the things that it takes to make mallocTest.c