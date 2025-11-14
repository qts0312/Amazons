CXX = g++
CXXFLAGS = -std=c++17 -Wall -g -O2
TARGET = amazons
SRC_DIR = src
SRCS = $(wildcard $(SRC_DIR)/*.cpp)

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)

.PHONY: all clean