all: library.o catch.o test-suite.o
	g++ library.o catch.o test-suite.o -o test-suite

library.o: library.cpp library.h
	g++ -c -g library.cpp

catch.o: catch.cpp catch.h
	g++ -c catch.cpp

test-suite.o: test-suite.cpp library.h catch.h
	g++ -c test-suite.cpp

clean:
	rm -rf *.o all
