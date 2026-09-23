gomoku: main.o
	g++ build/main.o build/other.o -o build/gomoku

main.o:
	g++ -c src/main.cpp -o build/main.o

other.o:
	g++ -c src/other.cpp -o build/other.o
