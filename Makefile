all:
	g++ -c -Og -std=c++11 *.hpp *.cpp
	g++ *.o -o main -lsfml-graphics -lsfml-window -lsfml-system # -lBox2D -Llib/ -lpoly2tri -lclipper
opt:
	g++ -O3 -std=c++11 *.hpp *.cpp -o main_opt -lsfml-graphics -lsfml-window -lsfml-system # -lBox2D -Llib/ -lpoly2tri -lclipper
debug:
	g++ -g -Og -std=c++11 *.hpp *.cpp -o debug -lsfml-graphics -lsfml-window -lsfml-system # -lBox2D -Llib/ -lpoly2tri -lclipper
warnings:
	g++ -g -Og -std=c++11 -Wall -Wno-reorder *.hpp *.cpp -o debug -lsfml-graphics -lsfml-window -lsfml-system # -lBox2D -Llib/ -lpoly2tri -lclipper
gprof:
	g++ -pg -O3 -std=c++11 *.hpp *.cpp -o debug_gprof -lsfml-graphics -lsfml-window -lsfml-system # -lBox2D -Llib/ -lpoly2tri -lclipper

.PHONY: all debug opt warnings gprog