#pragma once

#include "types.h"

// --- Board Functions ---
void init_board(Board& board);
void print_board(const Board& board);
bool is_position_valid(int r, int c);
bool is_path_clear(const Board& board, int r1, int c1, int r2, int c2);

// --- Game Logic Functions ---
bool is_move_valid(const Board& board, const Move& move, int player_piece);
void apply_move(Board& board, const Move& move, int player_piece);
std::vector<Move> generate_valid_moves(const Board& board, int player_piece);
bool has_valid_moves(const Board& board, int player_piece);

// --- Save/Load Functions ---
bool save_game(const Board& board, int current_player, const std::string& filename = "amazons.sav");
bool load_game(Board& board, int& current_player, const std::string& filename = "amazons.sav");

// --- Utility ---
void clear_screen();