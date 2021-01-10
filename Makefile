main: *.cpp *.c *.h
	g++ -O3 -march=native *.cpp -o main

clean:
	rm -rf main
