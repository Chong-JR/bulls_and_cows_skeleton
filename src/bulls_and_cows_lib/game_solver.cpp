

#include "game_solver.hpp"
#include "random.hpp"
#include <iostream>
#include <vector>
#include <algorithm>


namespace bulls_and_cows {

    // fonction permettant, à partir d'un code, de générer le suivant
    // ex: AAAAA ----> AAAAB
    //     AAAAH ----> AAABA
    Code generate_next_code(const GameOptions& game_options, Code previous_code)
    {
        Code new_code;
        new_code.value = previous_code.value;
        for (unsigned int i = game_options.number_of_characters_per_code; i > 0; i--)
        {
            if (previous_code.value[i - 1] != game_options.maximum_allowed_character)
            {
                new_code.value[i - 1] = previous_code.value[i - 1] + 1;
                return new_code;
            }

            else
            {
                new_code.value[i - 1] = game_options.minimum_allowed_character;
            }
        }
        return new_code;
    }

    // Génération de tous les codes possibles
    // on générera tous les codes possibles avec la fct generate_next_code 
    // on ajoutera toutes ces codes au vecteur possible_solutions
    PossibleSolutions generate_all_possible_codes(const GameOptions& game_options)
    {
        PossibleSolutions possible_solutions;
        Code first_code;
        Code last_code;
        for (unsigned int i = 0; i < game_options.number_of_characters_per_code; i++)
        {
            first_code.value += game_options.minimum_allowed_character;
            last_code.value += game_options.maximum_allowed_character;
        }

        possible_solutions.codes.push_back(first_code);

        while (possible_solutions.codes.back().value != last_code.value)
        {
            first_code = generate_next_code(game_options, first_code);
            possible_solutions.codes.push_back(first_code);
        }

        return possible_solutions;
    }

    // fonction renvoyant une tentative aléatoire
    Code pick_random_attempt(const PossibleSolutions& possible_solutions)
    {
        return possible_solutions
            .codes[generate_random_integer(0, static_cast<int>(possible_solutions.codes.size() - 1))];
    }

    // fonction retirant au vecteur possible_solutions tous les codes incompatibles
    // on parcourera le vecteur possible_solutions, et on regardant le nbr de bulls&cows de la tentative,
    // on déterminera le nombre de bulls&cows de chaque code, et si il est différent de celui trouvé avec le code secret,
    // il sera retiré du vecteur possible_sol
    void remove_incompatible_codes_from_possible_solutions(const AttemptAndFeedback& attempt_and_feedback,
                                                           PossibleSolutions& possible_solutions)
    {
        possible_solutions.codes.erase(
            std::remove_if(possible_solutions.codes.begin(), possible_solutions.codes.end(),
                           [&](Code c) {
                               if (compare_attempt_with_secret_code(c, attempt_and_feedback.attempt).bulls !=
                                       attempt_and_feedback.feedback.bulls ||
                                   compare_attempt_with_secret_code(c, attempt_and_feedback.attempt).cows !=
                                       attempt_and_feedback.feedback.cows)
                               {
                                   return true;
                               }
                               return false;
                           }),
            possible_solutions.codes.end());
    }

} // namespace bulls_and_cows
