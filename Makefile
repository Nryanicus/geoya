CC = g++
CC_FLAGS = -Og
LIBS = -lsfml-graphics -lsfml-window -lsfml-system
 
# File names
EXEC = main
DEBUG = debug
SOURCES = $(wildcard *.cpp)
OBJECTS = $(SOURCES:.cpp=.o)
 
# Main target
$(EXEC): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(EXEC) $(LIBS)

# debug target
$(DEBUG):
	rm debug
	g++ -g -Og *.hpp *.cpp -o debug $(LIBS)
 
# To obtain object files
%.o: %.cpp
	$(CC) -c $(CC_FLAGS) -MMD $< -o $@
 
# To remove generated files
clean:
	rm -f $(EXEC) $(OBJECTS)

-include *.d
