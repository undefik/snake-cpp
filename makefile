all: remove snake
remove:
	rm -f snake

snake:
	g++ -o snake main.cpp -lSDL2 -lSDL2_ttf
