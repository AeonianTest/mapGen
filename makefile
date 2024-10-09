make:
	g++ -g -Wall -Werror -std=c++11 Cell.cpp test.cpp -o test

clean:
	rm test