CXX = g++
CXXFLAGS = -std=c++11 -Wall
SRCDIR = src/Sources
INCDIR = src/Headers

SRCS = main.cpp $(SRCDIR)/Grafo.cpp $(SRCDIR)/Matriz.cpp $(SRCDIR)/FileManipulator.cpp
OBJS = $(SRCS:.cpp=.o)
TARGET = main

VPATH = $(SRCDIR):$(INCDIR)

all: $(TARGET) clean run

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(TARGET)

main.o: main.cpp Grafo.h Matriz.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

Grafo.o: Grafo.cpp Grafo.h
	$(CXX) $(CXXFLAGS) -c $< -o $@


Matriz.o: Matriz.cpp Matriz.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

FileManipulator.o: FileManipulator.cpp FileManipulator.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJS)