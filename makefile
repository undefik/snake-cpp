all: remove snake
remove:
	rm -f snake

snake:
	g++ -lSDL2 -lSDL2_ttf -o snake main.cpp
