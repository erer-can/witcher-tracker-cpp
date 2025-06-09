/**
 * @file main.cpp
 * @brief Entry point for the Witcher Tracker system.
 *
 * Reads user input lines, classifies them as sentence-type or question-type commands,
 * and delegates them to the Inventory system for processing. The system runs in a loop
 * until an "Exit" command or EOF is received.
 */

#include <iostream>
#include <string>
#include "Inventory.h"
#include "Utils.h"

Inventory inventory;

/**
 * @brief Parse and execute a single user command related to inventory state or knowledge.
 *
 * This function is responsible for interpreting and executing a line of input from the user.
 * It first splits the line into words and determines the type of command (question or action).
 * Based on the command type, it calls the appropriate methods on the inventory object.
 *
 * @param line The input line to execute.
 * @return void
 */
void execute_line(const std::string &line)
{
    Utils::split_line(line);
    Utils::word_count = Utils::words.size();

    int type = Utils::detect_type();

    if (type == 2) // Exit command, finish the run
    {
        exit(0);
    }

    if (type == 1) // Question command
    {
        int question_type = Utils::detect_question_type(line);
        if (question_type == -1)
        {
            std::cout << "INVALID" << "\n";
            return;
        }
        switch (question_type)
        {
        case 0: // specific ingredient count
        {
            int cnt = inventory.get_ingredient_count(Utils::words[2]);
            std::cout << cnt << "\n";
            break;
        }
        case 1: // total ingredient count
            inventory.print_ingredients();
            break;
        case 2: // specific potion count
        {
            std::cout << inventory.get_potion_count() << "\n";
            break;
        }
        case 3: // total potion count
            inventory.print_potions();
            break;
        case 4: // specific trophy count
        {
            int cnt = inventory.get_trophy_count(Utils::words[2]);
            std::cout << cnt << "\n";
            break;
        }
        case 5: // total trophy count
            inventory.print_trophies();
            break;
        case 6: // specific monster knowledge
            inventory.print_monster_knowledge(Utils::words[4]);
            break;
        case 7: // potion formula
            inventory.print_potion_formula();
            break;
        default:
            std::cout << "INVALID" << "\n";
            break;
        }
    }
    else if (type == 0) // Candidate sentence type command
    {
        int sentence_type = Utils::detect_sentence_type(line);
        if (sentence_type == -1)
        {
            std::cout << "INVALID" << "\n";
            return;
        }
        switch (sentence_type)
        {
        case 0: // loot
            inventory.handle_loot();
            break;
        case 1: // trade
            inventory.handle_trade();
            break;
        case 2: // brew
            inventory.handle_brew();
            break;
        case 3: // sign knowledge
            inventory.handle_sign_knowledge();
            break;
        case 4: // potion knowledge
            inventory.handle_potion_knowledge();
            break;
        case 5: // potion recipe
            inventory.handle_potion_recipe();
            break;
        case 6: // encounter
            inventory.handle_encounter();
            break;
        default:
            std::cout << "INVALID" << "\n";
            break;
        }
    }
    else
    {
        std::cout << "INVALID" << "\n";
    }
}

int main()
{
    std::string line;

    // Continuously read and execute commands until EOF or "Exit"
    while (true)
    {
        std::cout << ">> ";
        std::getline(std::cin, line);

        if (std::cin.eof() || line == "Exit")
            break;

        execute_line(line);
    }

    return 0;
}
