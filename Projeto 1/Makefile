make:
	g++ -std=c++11 -O3 -Wall -g main.cpp -lm

# to execute: ./a.out < testfile
run:
	./a.out < testfile

clean:
	rm a.out

# to execute: ./randomDAG N p [seed] > testfile
rndDAG:
	g++ -O3 -Wall -o randomDAG randomDAG.cpp -lm
