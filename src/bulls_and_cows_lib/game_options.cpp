

#include "game_options.hpp"
#include "input.hpp"
#include <fstream>
#include <iostream>
#include <string>


namespace bulls_and_cows {

    // Affichage des options du jeu
    // utilisation d'objets output stream
    void display_game_options(std::ostream& output_stream, const GameOptions& game_options)
    {
        output_stream << "Voici les options du jeu: " << std::endl;
        output_stream << "Le nombre maximal de tentatives est: " << game_options.max_number_of_attempts << std::endl;
        output_stream << "Le nombre maximal de caracteres est: " << game_options.number_of_characters_per_code
                       << std::endl;
        output_stream << "Eventail des caracteres autorises: entre " << game_options.minimum_allowed_character
                      << " et "
                      << game_options.maximum_allowed_character
                      << std::endl;
    }

    // Affichage du menu des options du jeu
    // utilisation d'objets output stream
    void display_game_options_menu(std::ostream& output_stream)
    {
        output_stream << "Configuration des options:" << std::endl;
        output_stream << "0 - Retour au menu principal" << std::endl;
        output_stream << "1 - Modifier le nombre maximal de tentative par jeu" << std::endl;
        output_stream << "2 - Modifier le nombre de caracteres du code" << std::endl;
        output_stream << "3 - Modifier l'eventail de caracteres autorises (borne inferieure)" << std::endl;
        output_stream << "4 - Modifier l'eventail de caractères autorises (borne superieure)" << std::endl;
        output_stream << "5 - Sauver les options" << std::endl;
        output_stream << "6 - Charger les options" << std::endl;
        output_stream << "Quel est votre choix ?" << std::endl;
    }

    //fonction demandant au joueur son choix dans le menu des options
    //on utilisera une syntaxe switch
    GameOptionsMenuChoice ask_game_options_menu_choice(std::istream& input_stream)
    {
        const int choix = ask_int_or_default(input_stream, -1);
        switch (choix)
        {
        case 0:
            return GameOptionsMenuChoice::BackToMain;
        case 1:
            return GameOptionsMenuChoice::ModifyMaximumNumberOfAttempts;
        case 2:
            return GameOptionsMenuChoice::ModifyNumberOfCharactersPerCode;
        case 3:
            return GameOptionsMenuChoice::ModifyMinimumAllowedCharacter;
        case 4:
            return GameOptionsMenuChoice::ModifyMaximumAllowedCharacter;
        case 5:
            return GameOptionsMenuChoice::SaveOptions;
        case 6:
            return GameOptionsMenuChoice::LoadOptions;
        }
        return GameOptionsMenuChoice::Error;
    }

    // Sauvegarde des options du jeu
    // on changera les valeurs dans le .txt lié aux options du jeu (avec des objets output_file_stream pour l'écriture)
    bool save_game_options(std::ostream& output_file_stream, const GameOptions& game_options)
    {
        if (output_file_stream)
        {
            output_file_stream << game_options.max_number_of_attempts << "\n"
                               << game_options.number_of_characters_per_code << "\n"
                               << game_options.minimum_allowed_character << "\n"
                               << game_options.maximum_allowed_character << std::endl;
            return true;
        }
        return false;
    }

    // Chargement des options du jeu
    // on ira cherecher dans le .txt les options, puis on les affichera
    // on utilisera des objects input file stream pour la lecture
    bool load_game_options(std::istream& input_file_stream, GameOptions& game_options)
    {
        if (input_file_stream)
        {
            std::string ligne{};

            std::getline(input_file_stream, ligne);
            unsigned int nb_attempt = std::stoi(ligne);
            game_options.max_number_of_attempts = nb_attempt;

            std::getline(input_file_stream, ligne);
            unsigned int nb_attempt1 = std::stoi(ligne);
            game_options.number_of_characters_per_code = nb_attempt1;

            std::getline(input_file_stream, ligne);
            char min_character = ligne[0];
            game_options.minimum_allowed_character = min_character;

            std::getline(input_file_stream, ligne);
            char max_character = ligne[0];
            game_options.maximum_allowed_character = max_character;

            return true;

        }
        return false;
    }

} // namespace bulls_and_cows

