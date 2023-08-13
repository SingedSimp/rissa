cc = g++
ifeq ($(shell uname -s),Darwin)
	cc = g++-13
endif


main:
	$(cc) main.cpp -Wall -Wextra -g -O0 -std=gnu++20 -o task.out
	$(cc) youtube.cpp -Wall -Wextra -g -O0 -std=gnu++20 -o yt.out
yt: 	
	$(cc) youtube.cpp -Wall -Wextra -g -O0 -std=gnu++20 -o yt.out
task:
	$(cc) main.cpp -Wall -Wextra -g -O0 -std=gnu++20 -o task.out

