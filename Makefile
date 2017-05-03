# LIBS = -Ilib/json/src -lsfml-graphics -lsfml-window -lsfml-system
LIBS = -lsfml-graphics -lsfml-window -lsfml-system

all:
    g++ -std=c++14 *.hpp *.cpp -o main $(LIBS)
opt:
    g++ -O3 -std=c++14 *.hpp *.cpp -o main_opt $(LIBS)
debug:
    g++ -g -Og -std=c++14 *.hpp *.cpp -o debug $(LIBS)
warnings:
    g++ -g -Og -std=c++14 -Wall -Wno-reorder *.hpp *.cpp -o debug $(LIBS)
gprof:
    g++ -pg -O3 -std=c++14 *.hpp *.cpp -o debug_gprof $(LIBS)

.PHONY: all debug opt warnings gprog