/**
 * @class Monster
 * @brief Manages monster-related data and interactions.
 *
 * This class is responsible for maintaining information about monsters,
 * including their weaknesses to signs and potions. It provides methods
 * to add and retrieve this information.
 *
 * Data encapsulation is used to prevent direct access to internal data. All
 * modifications or retrievals are done through public getter and setters.
 *
 * @note This class assumes that all input data is valid and well-formed.
 */

#pragma once
#include <map>
#include <set>
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>

class Monster
{
private:
    std::set<std::string> signs_against;   // a set to keep track of signs can be used to defend the monster
    std::set<std::string> potions_against; // a set to keep track of potions can be used to defend the monster
public:
    std::set<std::string> get_signs();
    std::set<std::string> get_potions();
    void add_sign(std::string sign);
    void add_potion(std::string potion);
};