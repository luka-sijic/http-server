CXX := g++
CXXFLAGS := -std=c++23 -O2 -Wall -Wextra -pedantic
SOURCES := main.cpp server.cpp
OBJECTS := $(SOURCES:.cpp=.o)
TARGET := server

.PHONY: all clean

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

%.o: %.cpp server.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

run: all
	./$(TARGET)

clean:
	rm -rf $(OBJECTS) $(TARGET)