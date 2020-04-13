
#include "game.hpp"
#include "board.hpp"
#include "game_options.hpp"
#include "game_solver.hpp"
#include "input.hpp"
#include "main_menu.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>
#include "windows.h" 



namespace bulls_and_cows {

    void user_plays_against_computer(const GameOptions& game_options)
    {
        Board board = create_board(game_options);
        AttemptAndFeedback attempt_and_feedback;
        do
        {
            display_board(std::cout, game_options, board);
            attempt_and_feedback.attempt = ask_attempt(std::cout, std::cin, game_options, board);
            attempt_and_feedback.feedback =
                compare_attempt_with_secret_code(attempt_and_feedback.attempt, board.secret_code);
            board.attempts_and_feedbacks.push_back(attempt_and_feedback);
        } while (!is_end_of_game(game_options, board) && !is_win(game_options, board));
        display_board(std::cout, game_options, board);

        if (is_win(game_options, board))
        {
            std::cout << "Félicitations! Vous avez gagné !! Le code secret est: " << board.secret_code.value << std::endl;
            
        }
        else
        {
            std::cout << "Dommage! Vous avez perdu...le code secret est: " << board.secret_code.value << std::endl;
        }
       
    }

    void computer_plays_against_computer(const GameOptions& game_options)
    {
        std::cout << "\n" << std::endl;
        std::cout << "Mon nom est Julia.\n" << std::endl;
        std::cout << "Je suis une IA qui essaiera de gagner la partie.\n" << std::endl;
        std::cout << "Jouons ! \n" << std::endl;
        Sleep(2000);

        Board board = create_board(game_options);
        PossibleSolutions possible_solutions = generate_all_possible_codes(game_options);
        AttemptAndFeedback attempt_and_feedback;
        do
        {
            display_board(std::cout, game_options, board);
            std::cout << "Nombre de codes possibles restants: " << possible_solutions.codes.size() << std::endl;
            Sleep(2000);
            attempt_and_feedback.attempt = pick_random_attempt(possible_solutions);
            attempt_and_feedback.feedback =
                compare_attempt_with_secret_code(attempt_and_feedback.attempt, board.secret_code);
            board.attempts_and_feedbacks.push_back(attempt_and_feedback);
            remove_incompatible_codes_from_possible_solutions(attempt_and_feedback, possible_solutions);

        } while (!is_end_of_game(game_options, board) && !is_win(game_options, board));
        display_board(std::cout, game_options, board);
        if (is_win(game_options, board))
        {
            std::cout << "Julia a gagne!!" << std::endl;
        }
        else
        {
            std::cout << "Julia n'a pas trouve la solution..." << std::endl;
        }
    }

    void configure_game_options(GameOptions& game_options)
    {
        std::string path = "./save.txt";
        std::ofstream save_file;
        std::ifstream load_file(path);

        GameOptionsMenuChoice options;
        do
        {
            std::cout << "\n#################################\n";
            display_game_options(std::cout, game_options);
            std::cout << "\n#################################\n";
            display_game_options_menu(std::cout);

            GameOptionsMenuChoice choix = ask_game_options_menu_choice(std::cin);
            options = choix;

            switch (choix)
            {
            case GameOptionsMenuChoice::ModifyMaximumNumberOfAttempts:
                std::cout << "\n Entrez le nombre maximum de tentatives: ";
                game_options.max_number_of_attempts = ask_int_or_default(std::cin, game_options.max_number_of_attempts);
                break;

            case GameOptionsMenuChoice::ModifyNumberOfCharactersPerCode:
                std::cout << "\n Entrez le nombre maximum de caractère par code: ";
                game_options.number_of_characters_per_code = ask_int_or_default(std::cin, game_options.number_of_characters_per_code);
                break;

            case GameOptionsMenuChoice::ModifyMinimumAllowedCharacter:
                std::cout << "\n Entrez la borne inferieure de l'eventail de caracteres autorises: ";
                char min;
                min = ask_char_or_default(std::cin, game_options.minimum_allowed_character);
                if (min >= game_options.maximum_allowed_character)
                {
                    std::cout << "La borne inferieure ne peut pas etre superieure a la borne maximale";
                }
                else
                game_options.minimum_allowed_character = min;
                break;

            case GameOptionsMenuChoice::ModifyMaximumAllowedCharacter:
                std::cout << "\n Entrez la borne superieure de l'eventail de caracteres autorises: ";
                char max;
                max = ask_char_or_default(std::cin, game_options.maximum_allowed_character);
                if (max <= game_options.minimum_allowed_character)
                {
                    std::cout << "La borne superieure ne peut pas etre inferieure à la borne maximale";
                }
                else
                    game_options.maximum_allowed_character = max;
                break;

            case GameOptionsMenuChoice::SaveOptions:
                save_file.open(path);
                if (!save_game_options(save_file, game_options))
                {
                    std::cout << "Erreur d'ecriture dans le fichier.";
                }
                save_file.close();
                break;

            case GameOptionsMenuChoice::LoadOptions:
                load_file.seekg(0, std::ios::beg);
                if (!load_game_options(load_file, game_options))
                {
                    std::cout << "Erreur de lecture du fichier.";
                }
                break;

            case GameOptionsMenuChoice::Error:
                std::cout << "\n Vous n'avez pas entre un choix valide, veuillez reessayer. \n";
                break;
            }
        } while (options != GameOptionsMenuChoice::BackToMain);
        
    }

    void play_game()
    {
        GameOptions game_options{};

        while (true)
        {
            std::cout << "\n#################################\n";
            display_main_menu(std::cout);

            const auto user_choice = ask_main_menu_choice(std::cin);
            switch (user_choice)
            {
            case MainMenuChoice::Quit:
                std::cout << "\nBye bye!\n";
                return;
            case MainMenuChoice::UserPlaysAgainstComputer:
                user_plays_against_computer(game_options);
                break;
            case MainMenuChoice::ComputerPlaysAgainstComputer:
                computer_plays_against_computer(game_options);
                break;
            case MainMenuChoice::ConfigureOptions:
                configure_game_options(game_options);
                break;
            case MainMenuChoice::Error:
                std::cout << "\nYou did not enter a valid choice, please try again\n";
                break;
            }
        }
    }

} // namespace bulls_and_cows

