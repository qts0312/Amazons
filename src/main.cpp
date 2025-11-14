#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include <ctime>
#include "types.h" // Our game types (Board, Move, enums)
#include "game.h"  // Our game logic (apply_move, init_board)
#include "ai.h"    // Our AI (get_minimax_move)

// --- Coordinate System ---
// Botzone (x, y) = (column, row)
// Our AI (r, c) = (row, column)

// --- Color System ---
// Botzone (sample): 1 = Black, -1 = White
// Our AI: 1 (BLACK_PIECE) = Black, 2 (WHITE_PIECE) = White

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    Board game_board;
    init_board(game_board); // Use our 8x8 init

    int turnID;
    std::cin >> turnID;

    // --- History Playback Loop ---

    const int BOTZONE_BLACK = 1;
    const int BOTZONE_WHITE = -1;
    int bz_my_color = BOTZONE_WHITE; // Assume White
    int bz_opp_color;
    int ai_my_color;
    int ai_opp_color;

    int x0, y0, x1, y1, x2, y2; // Botzone (x, y) coords

    for (int i = 0; i < turnID; i++) {
        // 1. Read and apply opponent's move
        std::cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
        
        if (x0 == -1) {
            bz_my_color = BOTZONE_BLACK; // We are Black
        } else {
            bz_opp_color = -bz_my_color;
            ai_opp_color = (bz_opp_color == BOTZONE_BLACK) ? BLACK_PIECE : WHITE_PIECE;
            
            // Convert Botzone (x,y) to our (r,c)
            Move opp_move = { y0, x0, y1, x1, y2, x2 };
            apply_move(game_board, opp_move, ai_opp_color);
        }

        // 2. Read and apply my own (past) move
        if (i < turnID - 1) {
            std::cin >> x0 >> y0 >> x1 >> y1 >> x2 >> y2;
            if (x0 >= 0) {
                ai_my_color = (bz_my_color == BOTZONE_BLACK) ? BLACK_PIECE : WHITE_PIECE;
                
                Move my_past_move = { y0, x0, y1, x1, y2, x2 };
                apply_move(game_board, my_past_move, ai_my_color);
            }
        }
    }

    // --- End of History Playback ---
    
    ai_my_color = (bz_my_color == BOTZONE_BLACK) ? BLACK_PIECE : WHITE_PIECE;

    // Call our Minimax AI
    Move best_move = get_minimax_move(game_board, ai_my_color);

    // --- Output Decision ---
    // Convert our (r, c) back to Botzone's (x, y)
    std::cout << best_move.from_c << " " << best_move.from_r << " "
              << best_move.to_c   << " " << best_move.to_r   << " "
              << best_move.arrow_c << " " << best_move.arrow_r << std::endl;

    return 0;
}