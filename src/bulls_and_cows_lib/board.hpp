
#pragma once

#include "game_options.hpp"
#include <string>
#include <vector>

namespace bulls_and_cows {

    struct Code
    {
        std::string value{}; // Sequence of allowed characters
    };

    struct Feedback
    {
        unsigned int bulls{}; // Number of good and well-placed characters
        unsigned int cows{};  // Number of good but not well-place characters
    };

    struct AttemptAndFeedback
    {
        Code attempt{};      // user-entered code
        Feedback feedback{}; // feedback telling how many bulls and cows the attempts contains
    };

    struct Board
    {
        Code secret_code{};                                       // computer-generated secret code
        std::vector<AttemptAndFeedback> attempts_and_feedbacks{}; // List of attempts of the user
    };

    // Create a board and with a new random secret code composed of allowed characters
    Board create_board(const GameOptions& game_options);

    // Validate that a user-entered code contains proper number of allowed characters
    bool validate_attempt(const GameOptions& game_options, const Code& attempt);

    // Compare a user-entered code with the secret code and give the corresponding number of bulls and cows
    Feedback compare_attempt_with_secret_code( Code attempt,  Code secret_code);

    // Test if this is the end of the game
    bool is_end_of_game(const GameOptions& game_options, const Board& board);

    // Test if the last attempt is a win
    bool is_win(const GameOptions& game_options, const Board& board);

    // Display the scret code and the list of attempts of the board
    void display_board(std::ostream& output_stream, const GameOptions& game_options, const Board& board);

    // Ask the user to provide another attempt
    Code ask_attempt(std::ostream& output_stream, std::istream& input_stream, const GameOptions& game_options,
                     const Board& board);

} // namespace bulls_and_cows

