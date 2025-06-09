/**
 * @file Monster.cpp
 * @brief Implements the Monster class methods for bestiary management.
 *
 * This file contains the definitions of simple accessor and mutator methods
 * for tracking signs and potions known to be effective against a monster.
 * These methods are used by the Inventory system when updating or querying
 * bestiary knowledge.
 */

#include "Inventory.h"
#include "Utils.h"
#include <string>
#include <map>
#include <set>

/**
 * @brief Get the set of effective signs against the monster.
 * @return A set containing the names of known effective signs.
 */
std::set<std::string> Monster::get_signs()
{
    return signs_against;
}

/**
 * @brief Get the set of effective potions against the monster.
 * @return A set containing the names of known effective potions.
 */
std::set<std::string> Monster::get_potions()
{
    return potions_against;
}

/**
 * @brief Add a sign to the monster's known weaknesses.
 * @param sign The sign name to add.
 */
void Monster::add_sign(std::string sign)
{
    signs_against.insert(sign);
}

/**
 * @brief Add a potion to the monster's known weaknesses.
 * @param potion The potion name to add.
 */
void Monster::add_potion(std::string potion)
{
    potions_against.insert(potion);
}
