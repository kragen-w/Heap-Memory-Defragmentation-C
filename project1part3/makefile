mallocTestVersion3: mallocTestVersion3.o duMalloc.o
	gcc mallocTestVersion3.o duMalloc.o -o mallocTestVersion3

# what can effect this? what is included in this? what gets pasted?

mallocTestVersion3.o: mallocTestVersion3.c dumalloc.h
	gcc mallocTestVersion3.c -c

duMalloc.o: duMalloc.c dumalloc.h
	gcc duMalloc.c -c

clean:
	rm -f *.o
	rm -f mallocTest
# this just removes all dat shit, -f gets rid of any warning

# gcc mallocTest.c -MM gives you all of the things that it takes to make mallocTest.c