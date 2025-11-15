#include "game.h"
#include "colors.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>
#include <vector>

// --- Board Functions Implementation ---

void init_board(Board& board) {
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            board[r][c] = EMPTY;
        }
    }
    // Black pieces
    board[0][2] = BLACK_PIECE;
    board[0][5] = BLACK_PIECE;
    board[2][0] = BLACK_PIECE;
    board[2][7] = BLACK_PIECE;
    // White pieces
    board[5][0] = WHITE_PIECE;
    board[5][7] = WHITE_PIECE;
    board[7][2] = WHITE_PIECE;
    board[7][5] = WHITE_PIECE;
}

void print_board(const Board& board) {
    using namespace std;
    
    cout << C_GRAY;

    cout << endl << "     ";
    for (int c = 0; c < BOARD_SIZE; ++c) {
        cout << " " << c << "  ";
    }
    cout << endl;

    cout << "   +";
    for (int c = 0; c < BOARD_SIZE; ++c) {
        cout << "---+";
    }
    cout << endl;

    for (int r = 0; r < BOARD_SIZE; ++r) {
        cout << " " << r << " |";
        for (int c = 0; c < BOARD_SIZE; ++c) {
            
            string piece_str = " ";
            string piece_color = "";

            switch (board[r][c]) {
                case EMPTY:
                    piece_str = " ";
                    break;
                case BLACK_PIECE:
                    piece_str = u8"●";
                    piece_color = C_BLUE;
                    break;
                case WHITE_PIECE:
                    piece_str = u8"●";
                    piece_color = C_YELLOW;
                    break;
                case BARRIER:
                    piece_str = u8"■";
                    piece_color = C_RED;
                    break;
            }
            
            if (piece_color.empty()) {
                cout << " " << piece_str << " |";
            } else {
                cout << " " << piece_color << piece_str << C_GRAY << " |";
            }
        }
        cout << endl;

        cout << "   +";
        for (int c = 0; c < BOARD_SIZE; ++c) {
            cout << "---+";
        }
        cout << endl;
    }
    
    cout << C_RESET;
}

bool is_position_valid(int r, int c) {
    return r >= 0 && r < BOARD_SIZE && c >= 0 && c < BOARD_SIZE;
}

// Checks for obstructions between two points (queen-like move)
bool is_path_clear(const Board& board, int r1, int c1, int r2, int c2) {
    int dr = (r2 > r1) ? 1 : ((r2 < r1) ? -1 : 0);
    int dc = (c2 > c1) ? 1 : ((c2 < c1) ? -1 : 0);
    int dist = std::max(std::abs(r2 - r1), std::abs(c2 - c1));

    for (int i = 1; i < dist; ++i) {
        int r = r1 + i * dr;
        int c = c1 + i * dc;
        if (!is_position_valid(r, c) || board[r][c] != EMPTY) {
            return false;
        }
    }
    return true;
}

// --- Game Logic Functions Implementation ---

// Validates all 3 parts of a move (from, to, arrow)
bool is_move_valid(const Board& board, const Move& move, int player_piece) {
    if (!is_position_valid(move.from_r, move.from_c) ||
        !is_position_valid(move.to_r, move.to_c) ||
        !is_position_valid(move.arrow_r, move.arrow_c)) {
        return false;
    }

    if (board[move.from_r][move.from_c] != player_piece) return false;
    if (board[move.to_r][move.to_c] != EMPTY) return false;
    if (!is_path_clear(board, move.from_r, move.from_c, move.to_r, move.to_c)) return false;

    bool is_queen_move = (move.arrow_r == move.to_r) || (move.arrow_c == move.to_c) || 
                         (std::abs(move.arrow_r - move.to_r) == std::abs(move.arrow_c - move.to_c));
    if (!is_queen_move) return false;

    Board temp_board = board;
    temp_board[move.from_r][move.from_c] = EMPTY;
    temp_board[move.to_r][move.to_c] = player_piece;

    if (temp_board[move.arrow_r][move.arrow_c] != EMPTY) return false;
    if (!is_path_clear(temp_board, move.to_r, move.to_c, move.arrow_r, move.arrow_c)) return false;
    
    return true;
}

void apply_move(Board& board, const Move& move, int player_piece) {
    board[move.from_r][move.from_c] = EMPTY;
    board[move.to_r][move.to_c] = player_piece;
    board[move.arrow_r][move.arrow_c] = BARRIER;
}

// Helper for 8-way queen movement
const std::vector<std::pair<int, int>>& get_directions() {
    static const std::vector<std::pair<int, int>> directions = {
        {0, 1}, {0, -1}, {1, 0}, {-1, 0}, 
        {1, 1}, {1, -1}, {-1, 1}, {-1, -1}
    };
    return directions;
}

// Generates all possible valid moves for a player
std::vector<Move> generate_valid_moves(const Board& board, int player_piece) {
    std::vector<Move> valid_moves;

    for (int r1 = 0; r1 < BOARD_SIZE; ++r1) {
        for (int c1 = 0; c1 < BOARD_SIZE; ++c1) {
            if (board[r1][c1] != player_piece) continue;

            for (auto& dir : get_directions()) {
                for (int k = 1; ; ++k) {
                    int r2 = r1 + k * dir.first;
                    int c2 = c1 + k * dir.second;
                    if (!is_position_valid(r2, c2) || board[r2][c2] != EMPTY) break;

                    Board temp_board = board;
                    temp_board[r1][c1] = EMPTY;
                    temp_board[r2][c2] = player_piece;

                    for (auto& arrow_dir : get_directions()) {
                        for (int j = 1; ; ++j) {
                            int r3 = r2 + j * arrow_dir.first;
                            int c3 = c2 + j * arrow_dir.second;
                            if (!is_position_valid(r3, c3) || temp_board[r3][c3] != EMPTY) break;
                            valid_moves.push_back({r1, c1, r2, c2, r3, c3});
                        }
                    }
                }
            }
        }
    }
    return valid_moves;
}

// Optimized check to see if *any* move exists
bool has_valid_moves(const Board& board, int player_piece) {
    for (int r1 = 0; r1 < BOARD_SIZE; ++r1) {
        for (int c1 = 0; c1 < BOARD_SIZE; ++c1) {
            if (board[r1][c1] != player_piece) continue;

            for (auto& dir : get_directions()) {
                for (int k = 1; ; ++k) {
                    int r2 = r1 + k * dir.first;
                    int c2 = c1 + k * dir.second;
                    if (!is_position_valid(r2, c2) || board[r2][c2] != EMPTY) break;

                    Board temp_board = board;
                    temp_board[r1][c1] = EMPTY;
                    
                    for (auto& arrow_dir : get_directions()) {
                        int r3 = r2 + arrow_dir.first;
                        int c3 = c2 + arrow_dir.second;
                        if (is_position_valid(r3, c3) && temp_board[r3][c3] == EMPTY) {
                            return true; // Found one valid move
                        }
                    }
                }
            }
        }
    }
    return false;
}

// --- Save/Load Functions Implementation ---

bool save_game(const Board& board, int current_player, const std::string& filename) {
    std::ofstream file(filename);
    if (!file) return false;
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            file << board[r][c] << " ";
        }
        file << "\n";
    }
    file << current_player << "\n";
    file.close();
    return true;
}

bool load_game(Board& board, int& current_player, const std::string& filename) {
    std::ifstream file(filename);
    if (!file) return false;
    for (int r = 0; r < BOARD_SIZE; ++r) {
        for (int c = 0; c < BOARD_SIZE; ++c) {
            if (!(file >> board[r][c])) return false;
        }
    }
    if (!(file >> current_player)) return false;
    file.close();
    return true;
}

// --- Utility Functions Implementation ---

void clear_screen() {
    #ifdef _WIN32
        std::system("cls");
    #else
        std::system("clear");
    #endif
}