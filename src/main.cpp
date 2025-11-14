#include "game.h"
#include "ai.h" // Include the new AI header
#include <iostream>
#include <limits>
#include <cstdlib> // for srand
#include <ctime>   // for time

// --- Human Player Input ---

// Gets move coordinates from the human player
bool get_human_move(Move& move) {
    using namespace std;
    cout << endl;
    cout << "Your turn (Black 'B')." << endl;
    cout << "Enter move (FromRow FromCol ToRow ToCol ArrowRow ArrowCol)" << endl;
    cout << "Or enter -1 to access in-game menu (Save/Exit): ";

    int fr;
    cin >> fr;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter numbers." << endl;
        return false;
    }

    if (fr == -1) {
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return false; // Signal for menu
    }

    int fc, tr, tc, ar, ac;
    cin >> fc >> tr >> tc >> ar >> ac;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter 6 numbers." << endl;
        return false;
    }

    move = {fr, fc, tr, tc, ar, ac};
    return true; // Move is ready
}

// --- Game Flow ---

// Shows the in-game menu
// Returns true if the user wants to exit to the main menu
bool show_ingame_menu(Board& board, int current_player) {
    using namespace std;
    cout << "--- In-Game Menu ---" << endl;
    cout << "1. Save Game" << endl;
    cout << "2. Exit to Main Menu (Game not saved)" << endl;
    cout << "3. Continue Playing" << endl;
    cout << "Enter choice: ";
    int choice;
    cin >> choice;

    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        choice = -1;
    }

    switch (choice) {
        case 1: 
            if (save_game(board, current_player)) {
                cout << "Game saved." << endl;
            } else {
                cout << "Error: Could not save game." << endl;
            }
            cout << "Press Enter to continue.";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
            return false;
        case 2: 
            return true; // Exit to main menu
        default:
            return false;
    }
}

// The primary game loop for a single match
void run_game_loop(Board& board, int current_player) {
    using namespace std;
    bool game_over = false;

    while (!game_over) {
        clear_screen();
        print_board(board);

        // Check for win condition
        if (!has_valid_moves(board, current_player)) {
            game_over = true;
            string winner = (current_player == BLACK_PIECE) ? "White (AI)" : "Black (Human)";
            cout << endl << "GAME OVER: " << winner << " wins!" << endl;
            break;
        }

        Move move;
        if (current_player == BLACK_PIECE) { // Human's turn
            bool move_entered = false;
            while (!move_entered) {
                if (!get_human_move(move)) {
                    if (show_ingame_menu(board, current_player)) {
                        return; // Exit to main menu
                    }
                    clear_screen();
                    print_board(board);
                    continue;
                }

                if (is_move_valid(board, move, current_player)) {
                    move_entered = true;
                } else {
                    cout << "That is not a valid move. Please try again." << endl;
                }
            }
        } else { // AI's turn
            move = get_minimax_move(board, current_player); // Use new AI function
        }

        apply_move(board, move, current_player);
        current_player = (current_player == BLACK_PIECE) ? WHITE_PIECE : BLACK_PIECE;
    }

    cout << "Press Enter to return to the main menu...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// --- Main Function ---

int main() {
    using namespace std;
    
    // Seed the random number generator (used by AI if multiple moves have same score)
    srand(static_cast<unsigned int>(time(nullptr)));

    bool running = true;
    while (running) {
        clear_screen();
        cout << "===== Amazon Chess (亚马逊棋) =====" << endl;
        cout << "1. New Game (Human vs AI)" << endl;
        cout << "2. Load Game" << endl;
        cout << "3. Exit" << endl;
        cout << "==================================" << endl;
        cout << "Enter choice: ";

        int choice;
        cin >> choice;

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            choice = -1;
        }

        Board game_board;
        int player;

        switch (choice) {
            case 1:
                init_board(game_board);
                player = BLACK_PIECE;
                run_game_loop(game_board, player);
                break;
            case 2:
                if (load_game(game_board, player)) {
                    cout << "Game loaded." << endl;
                    cout << "Press Enter to start...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.get();
                    run_game_loop(game_board, player);
                } else {
                    cout << "Error loading game (file 'amazons.sav' not found or corrupted)." << endl;
                    cout << "Press Enter to continue...";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    cin.get();
                }
                break;
            case 3:
                running = false;
                break;
            default:
                cout << "Invalid choice. Press Enter to try again...";
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cin.get();
                break;
        }
    }
    return 0;
}