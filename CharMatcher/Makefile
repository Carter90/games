CharMatcher:main.o CharMatcher.o
	g++ -std=c++11 -g -o CharMatcher main.o CharMatcher.o

main.o:main.cpp CharMatcher.h
	g++ -std=c++11 -g -c main.cpp 

CharMatcher.o: CharMatcher.cpp CharMatcher.h
	g++ -std=c++11 -g  -c CharMatcher.cpp

# clean up
clean:
	rm -f CharMatcher *.o *~
