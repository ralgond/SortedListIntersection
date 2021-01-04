main: *.cpp *.c
	g++ -O2 -march=native *.cpp -o main

clean:
	rm -rf main
