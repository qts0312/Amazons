#pragma once

#include "types.h"
#include "game.h"

/**
 * @brief AI's public interface.
 * * @param board The current board state.
 * @param player_piece The piece ID for the AI.
 * @return The best Move found by the AI.
 */
Move get_minimax_move(const Board& board, int player_piece);