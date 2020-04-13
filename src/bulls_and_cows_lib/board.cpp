#include "board.hpp"
#include "random.hpp"
#include "input.hpp"

namespace bulls_and_cows {
    // fonction générant un code secret
    // en fonction du nbr de caractères du code dans les options, on va créer un code aléatoirement
    // enfin on pourra donner un code secret à la structure board
    Board create_board(const GameOptions& game_options)    {
        Board board;
        std::string code{}; // code vide
        for (unsigned int i = 0; i < game_options.number_of_characters_per_code;i++)
        {

            code += generate_random_character(game_options.minimum_allowed_character,
                                           game_options.maximum_allowed_character);
         }
        board.secret_code.value = code;
        return board;
    }
    
    // fonction booléenne déterminant si la tentative du joueur est valide
    // càd si la tentative respecte les options du jeu
    // si un des caractères n'est pas compris dans l'éventail des caractères autorisés, on retourn false
    // sinon on retourne vrai
    bool validate_attempt(const GameOptions& game_options, const Code& attempt)
    {
        if (game_options.number_of_characters_per_code != attempt.value.size())
        {
            return false;
        }
        for (auto character : attempt.value)
        {
            if (character < game_options.minimum_allowed_character ||
                character > game_options.maximum_allowed_character)
            {
                return false;
            }
        }
        return true;
    }

    // fonction comparant le code secret avec la tentative du joueur
    // le reour sera sous format feedback, càd qu'on cherche à trouver le nbr de bulls et de cows
    // pour les bulls, on va simplement comparer les caractères un par un
    // si on trouve un bull on le remplacera par un caractère spécial pour qu'il ne soit pas aussi compté comme cow
    // pour les cows, on utilisera un pointeur qui déterminera si le caractère est présent dans le code secret
    Feedback compare_attempt_with_secret_code(Code attempt,  Code secret_code)
    {
        Feedback feedback;
        for (unsigned int i = 0; i < attempt.value.size(); i++)
        {
            if (secret_code.value[i] == attempt.value[i])
            {
                feedback.bulls++;
                secret_code.value.replace(i, 1, "?");
                attempt.value.replace(i, 1, "!");
            }
        }
        for (auto k : attempt.value)
        {
        auto iter = std::find(secret_code.value.begin(), secret_code.value.end(), k);
            if (iter != secret_code.value.end())
            {
                feedback.cows++;
                *iter = '?';
            }
                   
        }
        return feedback;
    }

    // fonction booléenne déterminant si la partie est finie
    // on utilisera le nombre de coups permis dans les options
    bool is_end_of_game(const GameOptions& game_options, const Board& board)
    {
        if (board.attempts_and_feedbacks.size() == game_options.max_number_of_attempts)
        {
            return true;
        }
        return false;
    }

    // fonction booléenne déterminant si la partie est gagnée
    bool is_win(const GameOptions& game_options, const Board& board)
    {
        if (board.attempts_and_feedbacks.empty())
        {
            return false;
        }

        if (board.attempts_and_feedbacks.back().attempt.value == board.secret_code.value)
        {
            return true;
        }
        return false;
    }

    // fonction affichant le tableau
    // on fera attention à ce que l'alignement soit respecté même en changeant les options
    void display_board(std::ostream& output_stream, const GameOptions& game_options, const Board& board)
    {

        if (is_win(game_options, board) || is_end_of_game(game_options, board))
        {
            output_stream << "-------------------------------------\n"
                          << "| SECRET   ";
            for (auto charcode : board.secret_code.value)
            {
                output_stream << charcode;
                output_stream << " ";
            }
        }
        else
        {

            output_stream << "-------------------------------------\n"
                          << "| SECRET   ";
            for (unsigned j = 0; j < game_options.number_of_characters_per_code; j++)
            {
                output_stream << "* ";
            }
        }
        output_stream << "|              |\n-------------------------------------\n"
                      << "| ATTEMPTS ";

        for (unsigned l = 0; l < game_options.number_of_characters_per_code; l++)
        {
            output_stream << "  ";
        }
        output_stream << "| BULLS | COWS |\n "
                      << "-------------------------------------\n";

        for (unsigned i = game_options.max_number_of_attempts; i > 0; i--)
        {
            std::string points{};
            for (unsigned k = 0; k < game_options.number_of_characters_per_code; k++)
            {
                points += ". ";
            }
            if (board.attempts_and_feedbacks.size() < i)
            {
                if (i < 10)
                    output_stream << "| #0";
                else
                    output_stream << "| #";
                output_stream << i << "      " << points << "|       |      |\n";
            }
            else
            {
                if (i < 10)
                    output_stream << "| #0";
                else
                    output_stream << "| #";
                output_stream << i << "      ";
                for (auto charbychar : board.attempts_and_feedbacks.at(i - 1).attempt.value)
                {
                    output_stream << charbychar;
                    output_stream << " ";
                }
                output_stream << "|   " << board.attempts_and_feedbacks.at(i - 1).feedback.bulls << "   |  "
                              << board.attempts_and_feedbacks.at(i - 1).feedback.cows << "   |\n";
            }
        }
        output_stream << "-------------------------------------\n";
    }

    // fonction demandant au joueur une nvelle tentative
    // on utilisera la fonction ask_string de input.hpp
    // on veillera à ce que l'input soit valide
    Code ask_attempt(std::ostream& output_stream, std::istream& input_stream, const GameOptions& game_options,
                     const Board& board)
    {
        Code attempt;
        output_stream << "Veuillez entrer un code de " << game_options.number_of_characters_per_code
                      << " caractères compris entre " << game_options.minimum_allowed_character << " et "
                      << game_options.maximum_allowed_character << std::endl;

       attempt.value = ask_string(input_stream);

        while (!validate_attempt(game_options, attempt))
        {
            output_stream << "Votre essai n'est pas valide" << std::endl;
            output_stream << "Veuillez entrer un code de " << game_options.number_of_characters_per_code
                          << " caractères compris entre " << game_options.minimum_allowed_character << " et "
                          << game_options.maximum_allowed_character << std::endl;

            attempt.value = ask_string(input_stream);
        }
        return attempt;
    }

} // namespace bulls_and_cows
