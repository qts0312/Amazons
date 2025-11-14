#include "ai.h"
#include "game.h"
#include <vector>
#include <limits>
#include <iostream>

// AI search depth (2-ply: 1 for AI, 1 for opponent)
const int MAX_DEPTH = 2;

// Simple evaluation function: (my moves - opponent moves)
// Based on "mobility" or "controlled area" idea.
int evaluate_board(const Board& board, int my_piece) {
    int opp_piece = (my_piece == BLACK_PIECE) ? WHITE_PIECE : BLACK_PIECE;

    int my_moves_count = generate_valid_moves(board, my_piece).size();
    int opp_moves_count = generate_valid_moves(board, opp_piece).size();

    if (my_moves_count == 0) return -1000000; // I lose
    if (opp_moves_count == 0) return 1000000; // I win

    return my_moves_count - opp_moves_count;
}

// NegaMax search with Alpha-Beta Pruning
int minimax_search(Board& board, int depth, int alpha, int beta, int current_player_piece) {
    
    // 1. Reached max depth or terminal node (handled below)
    if (depth == 0) {
        return evaluate_board(board, current_player_piece);
    }

    // 2. Check for game over
    std::vector<Move> moves = generate_valid_moves(board, current_player_piece);
    if (moves.empty()) {
        return -1000000; // Current player loses
    }

    // 3. Iterate through moves
    int best_value = -std::numeric_limits<int>::max();
    for (const auto& move : moves) {
        // A. Make the move
        Board next_board = board;
        apply_move(next_board, move, current_player_piece);

        // B. Recursive call (for opponent)
        int opp_piece = (current_player_piece == BLACK_PIECE) ? WHITE_PIECE : BLACK_PIECE;
        
        // NegaMax style: negate score and swap/negate alpha-beta
        int value = -minimax_search(next_board, depth - 1, -beta, -alpha, opp_piece);

        // C. Update best value and alpha
        best_value = std::max(best_value, value);
        alpha = std::max(alpha, value);

        // D. Alpha-Beta Pruning
        if (alpha >= beta) {
            break; // Beta cut-off
        }
    }
    return best_value;
}

// AI's top-level function to find the best move
Move get_minimax_move(const Board& board, int player_piece) {
    // std::cout << std::endl;
    // std::string player_name = (player_piece == BLACK_PIECE) ? "Black (B)" : "White (W)";
    // std::cout << player_name << " (AI) is thinking..." << std::endl;

    std::vector<Move> possible_moves = generate_valid_moves(board, player_piece);

    if (possible_moves.empty()) {
        return {-1, -1, -1, -1, -1, -1}; // No moves
    }

    Move best_move = possible_moves[0];
    int best_score = -std::numeric_limits<int>::max();
    int alpha = -std::numeric_limits<int>::max();
    int beta = std::numeric_limits<int>::max();

    // Iterate through all possible first moves
    for (const auto& move : possible_moves) {
        Board next_board = board;
        apply_move(next_board, move, player_piece);

        int opp_piece = (player_piece == BLACK_PIECE) ? WHITE_PIECE : BLACK_PIECE;
        
        // Call search for the opponent, negating the result
        int score = -minimax_search(next_board, MAX_DEPTH - 1, -beta, -alpha, opp_piece);

        if (score > best_score) {
            best_score = score;
            best_move = move;
        }
        alpha = std::max(alpha, best_score);
    }
    
    // std::cout << player_name << " (AI) chose move with score: " << best_score << std::endl;
    return best_move;
}