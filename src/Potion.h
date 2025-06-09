/**
 * @class Potion
 * @brief Represents a craftable alchemical potion in Geralt's inventory.
 *
 * This class is responsible for storing the ingredients required to craft the potion,
 * along with the number of times the potion has been brewed (its count in the inventory).
 * It provides methods to get and set the ingredients, as well as to increment or decrement
 * the potion count.
 * All internal data is encapsulated to prevent direct access or arbitrary modification from outside the class.
 */

#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

class Potion
{
private:
    std::vector<std::pair<std::string, int>> ingredients; // a vector to keep the ingredients and their counts necessary for crafting the potion
public:
    std::vector<std::pair<std::string, int>> get_ingredients();
    void set_ingredients(std::vector<std::pair<std::string, int>> ingredients);
};