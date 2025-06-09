/**
 * @file Potion.cpp
 * @brief Implements the Potion class methods used in alchemy and brewing logic.
 *
 * This file contains simple getter and setter logic for potion ingredients,
 * including a sorting routine to ensure correct formatting in responses.
 */

#include "Inventory.h"
#include "Utils.h"
#include <string>
#include <set>

/**
 * @brief Get the list of ingredients required for this potion.
 *
 * The list is sorted by descending quantity. If quantities are equal,
 * ingredients are sorted in ascending alphabetical order. This ensures
 * consistent output formatting for "What is in ..." queries.
 *
 * @return A sorted vector of ingredient-count pairs.
 */
std::vector<std::pair<std::string, int>> Potion::get_ingredients()
{
    std::sort(ingredients.begin(), ingredients.end(),
              [](const std::pair<std::string, int> &a, const std::pair<std::string, int> &b)
              {
                  if (a.second == b.second)
                  {
                      return a.first < b.first;
                  }
                  return a.second > b.second;
              });
    return ingredients;
}

/**
 * @brief Set the list of ingredients for this potion.
 * @param new_ingredients A vector containing pairs of ingredient names and their quantities.
 */
void Potion::set_ingredients(std::vector<std::pair<std::string, int>> new_ingredients)
{
    this->ingredients = new_ingredients;
}
