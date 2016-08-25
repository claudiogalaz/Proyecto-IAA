all: main.cpp
	g++ main.cpp -Wall -lm -o main

clean: 
	$(RM) main