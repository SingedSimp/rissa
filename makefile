cc = g++
ifeq ($(shell uname -s),Darwin)
	cc = g++-13
endif


main:
	$(cc) main.cpp -Wall -Wextra -g -O0 -std=gnu++20 -o task.out 
