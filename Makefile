RAYLIB := external/raylib/src/libraylib.a

build/gomoku: build/main.o build/other.o
	g++ build/main.o build/other.o $(RAYLIB) -lX11 -o build/gomoku

build/main.o:
	g++ -Iexternal/raylib/src -c src/main.cpp -o build/main.o

build/other.o:
	g++ -c src/other.cpp -o build/other.o

clean:
	rm -rf build/
