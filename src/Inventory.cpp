/**
 * @file Inventory.cpp
 * @brief Implements inventory actions and knowledge updates for CMPE230 Witcher Tracker project.
 *
 * This file defines the behavior of Geralt's inventory system, including managing ingredients,
 * potions, trophies, and bestiary knowledge based on structured input commands.
 */

#include "Inventory.h"
#include "Utils.h"
#include <string>
#include <map>
#include <set>

/**
 * @brief Handles the loot command by adding ingredients to the inventory.
 *
 * This method processes the loot command and adds the specified ingredients
 * to the inventory. It expects the command to be well-formed and does not
 * perform extensive error checking.
 *
 * @note This method modifies the ingredients in place.
 */
void Inventory::handle_loot()
{
    int curr_index = 2;
    while (curr_index < Utils::word_count - 1)
    {
        int cnt = std::stoi(Utils::words[curr_index]);
        std::string ing = Utils::words[curr_index + 1];
        add_ingredient(ing, cnt);
        curr_index += 3;
    }
    std::cout << "Alchemy ingredients obtained\n";
}

/**
 * @brief Increases amount of an ingredient or adds it to the inventory.
 *
 * @param name The name of the ingredient to add.
 * @param count The amount of the ingredient to add.
 *
 * @note This method modifies the ingredients in place.
 */
void Inventory::add_ingredient(std::string name, int count)
{
    if (ingredients.find(name) != ingredients.end())
    {
        ingredients[name] += count;
    }
    else
    {
        ingredients[name] = count;
    }
}

/**
 * @brief Handles the trade command by exchanging trophies for ingredients.
 *
 * This method processes the trade command and updates the inventory accordingly. It first checks the trophies
 * to be traded to ensure that there are enough trophies available. If the trade is valid, it removes the trophies
 * from the inventory and adds the specified ingredients.
 *
 * @note This method modifies the inventory and trophies in place.
 */
void Inventory::handle_trade()
{
    int curr_index = 2;
    std::map<std::string, int> trophies_to_trade;
    while (true)
    {
        int cnt = std::stoi(Utils::words[curr_index]);
        std::string trophy = Utils::words[curr_index + 1];
        if (trophies.find(trophy) == trophies.end() || trophies[trophy] < cnt)
        {
            std::cout << "Not enough trophies\n";
            return;
        }
        trophies_to_trade[trophy] = cnt;
        if (Utils::words[curr_index + 3] == "for")
        {
            curr_index += 4;
            break;
        }
        curr_index += 3;
    }
    for (const auto &pair : trophies_to_trade)
    {
        decrease_trophy(pair.first, pair.second);
    }
    while (curr_index < Utils::word_count - 1)
    {
        int cnt = std::stoi(Utils::words[curr_index]);
        std::string ing = Utils::words[curr_index + 1];
        add_ingredient(ing, cnt);
        curr_index += 3;
    }
    std::cout << "Trade successful\n";
}

/**
 * @brief Decreases the count of a trophy in the inventory.
 *
 * @param name The name of the trophy to decrease.
 * @param count The amount to decrease the trophy by.
 *
 * @note This method modifies the trophies in place.
 */
void Inventory::decrease_trophy(std::string name, int count)
{
    if (trophies.find(name) != trophies.end())
    {
        trophies[name] -= count;
        if (trophies[name] <= 0)
        {
            trophies.erase(name);
        }
    }
}

/**
 * @brief Uses a specified amount of an ingredient from the inventory.
 *
 * @param name The name of the ingredient to use.
 * @param count The amount of the ingredient to use.
 *
 * @note This method modifies the ingredients in place.
 */
void Inventory::use_ingredient(std::string name, int count)
{
    if (ingredients.find(name) != ingredients.end())
    {
        ingredients[name] -= count;
        if (ingredients[name] <= 0)
        {
            ingredients.erase(name);
        }
    }
}

/**
 * @brief Handles the brewing of a potion.
 *
 * This method checks if the player has the necessary ingredients to brew a potion.
 * If the ingredients are sufficient, the potion is created and added to the inventory.
 *
 * @note This method modifies the ingredients and potions in place.
 */
void Inventory::handle_brew()
{
    // first we construct the potion name, since it can be multiple words
    // we need to start from the 2nd word and go until the last word, since the first two words are "brew" and "potion"
    std::string potion_name = "";
    for (int i = 2; i <= Utils::word_count - 1; ++i)
    {
        potion_name += Utils::words[i];
        if (i <= Utils::word_count - 2)
        {
            potion_name += " ";
        }
    }
    // if we don't know the potion formula, we can't brew it
    if (potions.find(potion_name) == potions.end())
    {
        std::cout << "No formula for " << potion_name << "\n";
        return;
    }
    std::vector<std::pair<std::string, int>> ingredients_needed = potions[potion_name].get_ingredients();
    // now we need to check if we have all ingredients, we will brew after making sure we have all necessary ingredients
    for (const auto &pair : ingredients_needed)
    {
        const std::string &name = pair.first;
        int count = pair.second;
        if (ingredients.find(name) == ingredients.end() || ingredients[name] < count)
        {
            std::cout << "Not enough ingredients\n";
            return;
        }
    }
    // if we reached here, we have all ingredients, we can brew the potion
    for (const auto &pair : ingredients_needed)
    {
        const std::string &name = pair.first;
        int count = pair.second;
        use_ingredient(name, count);
    }

    // now we can add the potion to the inventory
    potion_counts[potion_name]++;
    std::cout << "Alchemy item created: " << potion_name << "\n";
}

/**
 * @brief Handles the knowledge of a sign for a specific monster.
 *
 * This method updates the bestiary entry for a monster with a new sign.
 *
 * @note This method modifies the monsters map in place.
 */
void Inventory::handle_sign_knowledge()
{
    // we extract the sign and monster names from line structure with words index
    std::string sign_name = Utils::words[2];
    std::string monster_name = Utils::words[7];

    // if monster wasn't in our database before we create a new monster
    if (monsters.find(monster_name) == monsters.end())
    {
        std::cout << "New bestiary entry added: " << monster_name << "\n";
        Monster monster;
        monster.add_sign(sign_name);
        monsters[monster_name] = monster;
    }
    else
    {
        // if we saw the monster before, we get the information stored and move on from there
        std::set<std::string> signs = monsters[monster_name].get_signs();

        // if we don't know about the effectiveness of the sign against the monster, we add it to the bestiary
        if (signs.find(sign_name) == signs.end())
        {
            std::cout << "Bestiary entry updated: " << monster_name << "\n";
            monsters[monster_name].add_sign(sign_name);
        }
        // if we already knew, than we do nothing
        else
        {
            std::cout << "Already known effectiveness\n";
        }
    }
}

/**
 * @brief Handles the knowledge of a potion for a specific monster.
 *
 * This method updates the bestiary entry for a monster with a new potion.
 * Uses the same logic as handle_sign_knowledge but for potions.
 * We can't use constant indices because of the potion name.
 *
 * @note This method modifies the monsters map in place.
 */
void Inventory::handle_potion_knowledge()
{
    std::string potion_name = "";
    int curr_index = 2;
    while (Utils::words[curr_index] != "potion")
    {
        potion_name += Utils::words[curr_index];
        if (Utils::words[curr_index + 1] != "potion")
        {
            potion_name += " ";
        }
        curr_index++;
    }

    std::string monster_name = Utils::words[curr_index + 4];
    if (monsters.find(monster_name) == monsters.end())
    {
        std::cout << "New bestiary entry added: " << monster_name << "\n";
        Monster monster;
        monster.add_potion(potion_name);
        monsters[monster_name] = monster;
    }
    else
    {
        std::set<std::string> m_potions = monsters[monster_name].get_potions();
        if (m_potions.find(potion_name) == m_potions.end())
        {
            std::cout << "Bestiary entry updated: " << monster_name << "\n";
            monsters[monster_name].add_potion(potion_name);
        }
        else
        {
            std::cout << "Already known effectiveness\n";
        }
    }
}

/**
 * @brief Handles potion recipe information.
 *
 * This method adds given potion recipe to the related potion in the inventory.
 *
 * @note This method modifies the potions map in place.
 */
void Inventory::handle_potion_recipe()
{
    // similar potion name extraction as before
    std::string potion_name = "";
    int curr_index = 2;
    while (Utils::words[curr_index] != "potion")
    {
        potion_name += Utils::words[curr_index];
        if (Utils::words[curr_index + 1] != "potion")
        {
            potion_name += " ";
        }
        curr_index++;
    }
    if (potions.find(potion_name) != potions.end() && potions[potion_name].get_ingredients().size() > 0)
    {
        std::cout << "Already known formula\n";
    }
    // if we encounter the formula for the first time, we need to add this new information
    else
    {
        std::vector<std::pair<std::string, int>> formula_ingredients;
        curr_index += 3;
        while (curr_index < Utils::word_count - 1)
        {
            int cnt = std::stoi(Utils::words[curr_index]);
            std::string ing = Utils::words[curr_index + 1];
            formula_ingredients.push_back({ing, cnt});
            curr_index += 3;
        }
        if (potions.find(potion_name) == potions.end())
        {
            Potion potion;
            potion.set_ingredients(formula_ingredients);
            potions[potion_name] = potion;
        }
        else
        {
            potions[potion_name].set_ingredients(formula_ingredients);
        }
        std::cout << "New alchemy formula obtained: " << potion_name << "\n";
    }
}

/**
 * @brief Uses one potion of each type on the specified monster.
 *
 * @param monster_name The name of the monster to use potions on.
 *
 * @note This method modifies the potion_counts map in place.
 */
void Inventory::use_one_potion_each(std::string monster_name)
{
    for (std::string potion : monsters[monster_name].get_potions())
    {
        if (potion_counts.find(potion) != potion_counts.end() && potion_counts[potion] > 0)
        {
            potion_counts[potion]--;
            if (potion_counts[potion] == 0)
            {
                potion_counts.erase(potion);
            }
        }
    }
}

/**
 * @brief Handles a monster encounter.
 *
 * This method checks if the player is prepared for the encounter by verifying
 * the available signs and potions for the specified monster. If there is at least one known sign or
 * there is at least one potion available that is known to be effective against the monster, then we survive,
 * use one potion for each effective potion available, or use the sign.
 *
 * @note This method modifies the trophies map in place.
 */
void Inventory::handle_encounter()
{
    // first we extract the monster name from the line
    std::string monster_name = Utils::words[3];

    // if we don't know the monster, or we don't know any sign or potion against it, we are unprepared
    if (monsters.find(monster_name) == monsters.end() ||
        (monsters[monster_name].get_signs().size() == 0 && monsters[monster_name].get_potions().size() == 0))
    {
        std::cout << "Geralt is unprepared and barely escapes with his life\n";
        return;
    }
    // if we know the monster, and have some knowledge
    // if all we know is effective potions, we need to check if we have at least one of them
    if (monsters[monster_name].get_signs().size() == 0)
    {
        bool found = false;
        for (std::string potion : monsters[monster_name].get_potions())
        {
            if (potion_counts.find(potion) != potion_counts.end() && potion_counts[potion] > 0)
            {
                found = true;
                break;
            }
        }
        if (found)
        {
            use_one_potion_each(monster_name);
        }
        else
        {
            std::cout << "Geralt is unprepared and barely escapes with his life\n";
            return;
        }
    }
    else
    {
        use_one_potion_each(monster_name);
    }
    std::cout << "Geralt defeats " << monster_name << "\n";

    if (trophies.find(monster_name) == trophies.end())
    {
        trophies[monster_name] = 1;
    }
    else
    {
        trophies[monster_name]++;
    }
}

/**
 * @brief Gets the count of a specific ingredient in the inventory.
 *
 * @param name The name of the ingredient to check.
 * @return The count of the ingredient in the inventory, or 0 if it doesn't exist.
 *
 * @note This method does not modify the inventory.
 */
int Inventory::get_ingredient_count(std::string name)
{
    if (ingredients.find(name) != ingredients.end())
    {
        return ingredients[name];
    }
    return 0;
}

/**
 * @brief Prints the ingredients in the inventory.
 *
 * This method is used when we encounter a total ingredient count question. It iterates through the ingredients,
 * and prints them one by one. Because we keep them in a map, we can simply iterate through the map and print them.
 *
 * @note This method does not modify the inventory.
 */
void Inventory::print_ingredients()
{
    int ingredient_count = ingredients.size();
    if (ingredient_count == 0)
    {
        std::cout << "None\n";
        return;
    }

    int i = 0;
    for (const auto &pair : ingredients)
    {
        std::cout << pair.second << " " << pair.first;
        if (i < ingredient_count - 1)
        {
            std::cout << ", ";
        }
        i++;
    }
    std::cout << "\n";
}

/**
 * @brief Gets the count of a specific potion in the inventory.
 *
 * @return The count of the potion in the inventory, or 0 if it doesn't exist.
 *
 * @note This method does not modify the inventory.
 */
int Inventory::get_potion_count()
{
    std::string name = "";
    for (int i = 2; i < Utils::word_count - 1; ++i)
    {
        name += Utils::words[i];
        if (i < Utils::word_count - 2)
        {
            name += " ";
        }
    }
    if (potion_counts.find(name) != potion_counts.end())
    {
        return potion_counts[name];
    }
    return 0;
}

/**
 * @brief Prints the potions in the inventory.
 *
 * This method is used when we encounter a total potion count question. It iterates through the potions,
 * and prints them one by one. Because we keep them in a map, we can simply iterate through the map and print them.
 *
 * @note This method does not modify the inventory.
 */
void Inventory::print_potions()
{
    int potion_count = potion_counts.size();
    if (potion_count == 0)
    {
        std::cout << "None\n";
        return;
    }

    int i = 0;
    for (auto &pair : potion_counts)
    {
        std::cout << pair.second << " " << pair.first;
        if (i < potion_count - 1)
        {
            std::cout << ", ";
        }
        i++;
    }
    std::cout << "\n";
}

/**
 * @brief Gets the count of a specific trophy in the inventory.
 *
 * @param name The name of the trophy to check.
 * @return The count of the trophy in the inventory, or 0 if it doesn't exist.
 *
 * @note This method does not modify the inventory.
 */
int Inventory::get_trophy_count(std::string name)
{
    if (trophies.find(name) != trophies.end())
    {
        return trophies[name];
    }
    return 0;
}

/**
 * @brief Prints the trophies.
 *
 * This method is used when we encounter a total trophy count question. It iterates through the trophies,
 * and prints them one by one. Because we keep them in a map, we can simply iterate through the map and print them.
 *
 * @note This method does not modify the inventory.
 */
void Inventory::print_trophies()
{
    int trophy_count = trophies.size();
    if (trophy_count == 0)
    {
        std::cout << "None\n";
        return;
    }
    int i = 0;
    for (const auto &pair : trophies)
    {
        std::cout << pair.second << " " << pair.first;
        if (i < trophy_count - 1)
        {
            std::cout << ", ";
        }
        i++;
    }
    std::cout << "\n";
}

/**
 * @brief Prints the knowledge about a specific monster.
 *
 * This method is used when we encounter a monster knowledge question. It retrieves the sign and potion knowledge
 * for the specified monster and prints them in a comma-separated format, in a merged alphabetical order.
 *
 * @param monster_name The name of the monster whose knowledge is to be printed.
 *
 * @note This method does not modify the inventory.
 */
void Inventory::print_monster_knowledge(std::string monster_name)
{
    if (monsters.find(monster_name) == monsters.end())
    {
        std::cout << "No knowledge of " << monster_name << "\n";
        return;
    }
    std::set<std::string> signs = monsters[monster_name].get_signs();
    std::set<std::string> m_potions = monsters[monster_name].get_potions();

    if (signs.empty() && m_potions.empty())
    {
        std::cout << "No knowledge of " << monster_name << "\n";
        return;
    }

    std::vector<std::string> merged;
    merged.reserve(signs.size() + m_potions.size());
    std::merge(signs.begin(), signs.end(), m_potions.begin(), m_potions.end(), std::back_inserter(merged));

    for (size_t i = 0; i < merged.size(); ++i)
    {
        std::cout << merged[i];
        if (i < merged.size() - 1)
        {
            std::cout << ", ";
        }
    }
    std::cout << "\n";
}

/**
 * @brief Prints a potion formula.
 *
 * This method is used when we encounter a potion formula question. It retrieves the ingredients for the specified
 * potion and prints them in a comma-separated format. It prints them in a descending order of the count, if equal
 * it prints them in alphabetical order.
 *
 * @note This method does not modify the inventory.
 */
void Inventory::print_potion_formula()
{
    std::string potion_name = "";
    for (int i = 3; i < Utils::word_count - 1; ++i)
    {
        potion_name += Utils::words[i];
        if (i < Utils::word_count - 2)
        {
            potion_name += " ";
        }
    }
    if (potions.find(potion_name) == potions.end())
    {
        std::cout << "No formula for " << potion_name << "\n";
        return;
    }

    std::vector<std::pair<std::string, int>> ingredients = potions[potion_name].get_ingredients();
    if (ingredients.empty())
    {
        std::cout << "No formula for " << potion_name << "\n";
        return;
    }
    int ingredient_count = ingredients.size();
    int i = 0;
    for (const auto &pair : ingredients)
    {
        std::cout << pair.second << " " << pair.first;
        if (i < ingredient_count - 1)
        {
            std::cout << ", ";
        }
        i++;
    }
    std::cout << "\n";
}
