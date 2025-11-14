#pragma once

#include <array>
#include <string>
#include <vector>

const int BOARD_SIZE = 8;

// Represents the state of a single tile
enum TileState {
    EMPTY = 0,
    BLACK_PIECE = 1,
    WHITE_PIECE = 2,
    BARRIER = 3
};

// Represents a complete move
struct Move {
    int from_r, from_c;
    int to_r, to_c;
    int arrow_r, arrow_c;
};

// Type alias for the 10x10 game board
using Board = std::array<std::array<int, BOARD_SIZE>, BOARD_SIZE>;