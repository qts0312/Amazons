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

MERGE_FILES = \
	src/types.h \
	src/game.h \
	src/ai.h \
	src/game.cpp \
	src/ai.cpp \
	src/main.cpp

MERGED_FILE = botzone_main.cpp

merge: $(MERGED_FILE)

$(MERGED_FILE): $(MERGE_FILES)
	@echo "" > $(MERGED_FILE)
	@for file in $(MERGE_FILES); do \
		sed '/^#include "/d' $$file >> $(MERGED_FILE); \
		echo "" >> $(MERGED_FILE); \
	done
	@echo "Merge complete: $(MERGED_FILE)"

.PHONY: all clean merge