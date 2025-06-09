/**
 * @class Inventory
 * @brief Manages Geralt's inventory and processes commands and queries related to it.
 *
 * This class encapsulates the state of Geralt’s inventory in the Witcher Tracker system,
 * including alchemical ingredients, brewed potions, monster trophies, potion formulas,
 * and bestiary knowledge.
 *
 * It exposes public methods to process well-formed sentence-type commands (e.g., looting,
 * brewing, trading, learning) and question-type queries (e.g., total counts, effectiveness).
 * Internal state is fully encapsulated, and updates are only allowed through class methods
 * to maintain data consistency.
 *
 * @note This class assumes all input commands are syntactically and semantically valid.
 *       No input validation is performed internally.
 */

#pragma once
#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include "Potion.h"
#include "Monster.h"

class Inventory
{
public:
    void handle_loot();
    void handle_trade();
    void handle_brew();
    void handle_sign_knowledge();
    void handle_potion_knowledge();
    void handle_potion_recipe();
    void handle_encounter();
    int get_ingredient_count(std::string name);
    void print_ingredients();
    int get_potion_count();
    void print_potions();
    int get_trophy_count(std::string name);
    void print_trophies();
    void print_monster_knowledge(std::string monster_name);
    void print_potion_formula();

private:
    std::map<std::string, int> ingredients;   // a map to keep count of ingredients in alphabetical order
    std::map<std::string, int> trophies;      // a map to keep count of trophies in alphabetical order
    std::map<std::string, int> potion_counts; // a map to keep count of potions in alphabetical order
    std::map<std::string, Potion> potions;    // a map to keep count of potions in alphabetical order
    std::map<std::string, Monster> monsters;  // a map to keep count of monsters in alphabetical order
    void add_ingredient(std::string name, int count);
    void decrease_trophy(std::string name, int count);
    void use_ingredient(std::string name, int count);
    void use_one_potion_each(std::string monster_name);
};