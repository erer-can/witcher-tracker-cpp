/**
 * @file Utils.cpp
 * @brief Implements parsing and validation utilities for command processing.
 *
 * Provides helper methods for tokenizing input, detecting input types,
 * and validating sentence/question structures in accordance with input grammar.
 */

#include "Utils.h"
#include <regex>
#include <string>
#include <iostream>

namespace Utils
{
    std::vector<std::string> words;
    int word_count = 0;

    /**
     * @brief Splits the line into words based on spaces, commas, and question marks.
     *
     * This function takes a line of text and splits it into individual words. Since we need commas to validity check,
     * and question marks to detect the type of input, we take them as separate word tokens as well.
     *
     * @param line The line of text to be split.
     * @return void
     *
     * @note This function modifies the global `words` vector and `word_count` variable.
     */
    void split_line(const std::string &line)
    {

        words.clear();
        std::regex pattern(R"([^\s,?]+|[?,])");

        auto begin = std::sregex_iterator(line.begin(), line.end(), pattern);
        auto end = std::sregex_iterator();

        for (auto it = begin; it != end; ++it)
        {
            words.push_back(it->str());
        }
        word_count = words.size();
    }

    /**
     * @brief Checks if the string contains only alphabetical characters.
     *
     * This function iterates through each character in the string and checks if it is an alphabetic character.
     *
     * @param str The string to be checked.
     * @return true if the string is alphabetical, false otherwise.
     *
     * @note This function does not modify any global state.
     */
    bool is_alphabetical(const std::string &str)
    {
        for (char c : str)
        {
            if (!isalpha(c))
            {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Checks if the string represents a valid integer.
     *
     * This function checks if the string consists only of digits, not any decimal points or non-numeric characters.
     *
     * @param str The string to be checked.
     * @return true if the string is a valid integer, false otherwise.
     *
     * @note This function does not modify any global state.
     */
    bool is_integer(const std::string &str)
    {
        for (char c : str)
        {
            if (!isdigit(c))
            {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Checks if the potion name is valid.
     *
     * This function checks if the potion name consists of only alphabetical words with only one space between them.
     * Does it by iterating through every word and checking them with is_alphabetical and each character in the potion name portion of the line and checking
     * character by character if a space is followed by another space.
     *
     * @param line The line of text to be checked.
     * @param start_index The starting index of the potion name in the words vector.
     * @param end_index The ending index of the potion name in the words vector.
     * @return true if the potion name is valid, false otherwise.
     *
     * @note This function does not modify any global state.
     */
    bool is_valid_potion_name(const std::string &line, int start_index, int end_index)
    {
        // first we find the potion name in the actual line because we need to check multiple spaces, and our words vector of course doesn't hold that information
        std::string start_word = words[start_index];
        std::string end_word = words[end_index];
        int start_index_in_line = line.find(start_word);                     // start of the potion name in the line
        int end_index_in_line = line.find(end_word) + end_word.length() - 1; // last character of the potion name in the line

        // all words must be alphabetical
        for (int i = start_index; i <= end_index; i++)
        {
            if (!is_alphabetical(words[i]))
            {
                return false;
            }
        }

        // there can't be any double spaces in the substring we found
        for (int i = start_index_in_line; i < end_index_in_line; i++)
        {
            if (line[i] == ' ' && line[i + 1] == ' ')
            {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Detects the type of input.
     *
     * This function tries to detect the type of input using the words vector. It is easy to detect exit type by simply
     * checking if one word line is "Exit.", and easy to detect question type by checking if the last word is "?".
     * The rest can be assumed as a candidate sentence. We say candidate because there is a much more chance that
     * these are invalid more than the detected questions. We apply necessary validity checks later, this function is
     * just for initial classification.
     *
     * @return int The type of input:
     *            -1: Invalid input
     *            0: Sentence
     *           1: Question
     *          2: Exit
     *
     * @note This function doesn't modify any global state.
     */
    int detect_type()
    {

        if (word_count == 0)
        {
            return -1; // Empty input
        }
        if (words[word_count - 1] == "?")
        {
            return 1; // Question
        }
        if (words[0] == "Exit" && word_count == 1)
        {
            return 2; // Exit
        }
        return 0; // Sentence
    }

    /**
     * @brief Checks if a candidate loot action sentence is valid.
     *
     * This function checks if a line follows the valid loot action sentence format. After Geralt loots, the sentence
     * follows a specific structure: "Geralt loots <count> <ingredient>, <count> <ingredient>, ...". We can check with
     * a modular approach if the sentence follows this structure.
     *
     * @return true if the sentence is valid, false otherwise.
     *
     * @note This function does not modify any global state.
     */
    bool is_valid_loot()
    {
        int curr_index = 2;
        // When calculated with necessary commas and the sentence structure a loot action sentence
        // must have 3k + 1 words, because each item has a count, a name and a comma except the last one.
        if (word_count % 3 != 1 || word_count < 4)
        {
            return false;
        }
        while (curr_index < word_count - 1)
        {
            // this loop checks the sentence structure in a modular manner
            // all words should follow the following pattern after the Geralt loots: "count", "ingredient", ","
            if (!is_integer(words[curr_index])) // check if the word is an integer to avoid double type numbers, stoi doesn't check that
            {
                return false;
            }
            int cnt = stoi(words[curr_index]);
            if (cnt <= 0)
            {
                return false;
            }
            std::string ing = words[curr_index + 1];
            if (!is_alphabetical(ing))
            {
                return false;
            }
            if (curr_index + 2 < word_count - 1 && words[curr_index + 2] != ",")
            {
                return false;
            }
            curr_index += 3;
        }
        return true;
    }

    /**
     * @brief Checks if a candidate trade action sentence is valid.
     *
     * This function checks if a line follows the valid trade action sentence format. After Geralt trades, the sentence
     * follows a specific structure: "Geralt trades <count> <trophy> trophy, <count> <trophy> trophy, ... for <count> <ingredient>, <count> <ingredient>, ...". We can check with
     * a modular approach if the sentence follows this structure.
     *
     * @return true if the sentence is valid, false otherwise.
     *
     * @note This function does not modify any global state.
     */
    bool is_valid_trade()
    {
        int curr_index = 2;
        // first we check the part trophies are given before the "for" keyword
        // when we encounter the "for" keyword, we break the loop and move on to the next part
        while (true)
        {
            if (!is_integer(words[curr_index]))
            {
                return false;
            }
            int cnt = stoi(words[curr_index]);
            if (cnt <= 0)
            {
                return false;
            }
            std::string ing = words[curr_index + 1];
            if (!is_alphabetical(ing))
            {
                return false;
            }

            if (words[curr_index + 3] == "for")
            {
                if (words[curr_index + 2] != "trophy")
                {
                    return false;
                }
                curr_index += 4; // to skip the "for" keyword to ingredients part
                break;
            }
            else if (words[curr_index + 2] != ",")
            {
                return false;
            }
            curr_index += 3;
        }

        // we completed the first part without encountering anything invalid
        // now we check the part for the ingredients after the "for" keyword
        // we can just use the same loop as the loot one for this part
        int cnt = word_count - curr_index;
        if (cnt % 3 != 2)
        {
            return false;
        }
        while (curr_index < word_count - 1)
        {
            if (!is_integer(words[curr_index]))
            {
                return false;
            }
            int cnt = stoi(words[curr_index]);
            if (cnt <= 0)
            {
                return false;
            }
            std::string ing = words[curr_index + 1];
            if (!is_alphabetical(ing))
            {
                return false;
            }
            if (curr_index + 2 < word_count - 1 && words[curr_index + 2] != ",")
            {
                return false;
            }
            curr_index += 3;
        }
        return true;
    }

    /**
     * @brief Checks if a candidate sign knowledge sentence is valid.
     *
     * This function checks if a line follows the valid sign knowledge sentence format. After Geralt learns a sign, the sentence
     * follows a specific structure: "Geralt learns <sign> sign is effective against <monster>". We can check with
     * a modular approach if the sentence follows this structure. Since monster and sign are both one word long, we can
     * use constant numbers as indexes to check them.
     *
     * @return true if the sentence is valid, false otherwise.
     *
     * @note This function does not modify any global state.
     */
    bool is_valid_sign_knowledge()
    {
        if (word_count != 8)
        {
            return false;
        }
        if (words[4] == "is" && words[5] == "effective" && words[6] == "against")
        {
            if (!is_alphabetical(words[2]))
            {
                return false;
            }
            if (!is_alphabetical(words[7]))
            {
                return false;
            }
        }
        else
        {
            return false;
        }
        return true;
    }

    /**
     * @brief Checks if a candidate potion knowledge sentence is valid.
     *
     * This function checks if a line follows the valid potion knowledge sentence format. After Geralt learns a potion, the sentence
     * follows a specific structure: "Geralt learns <potion> potion is effective against <monster>". We can check with
     * a modular approach if the sentence follows this structure. Since potion doesn't have to have a fixed length, we can't
     * use constant numbers as indexes to check them. We can use the word count to check if the potion name is valid.
     *
     * @param curr_index The index of the first word after "is" in the words vector.
     * @return true if the sentence is valid, false otherwise.
     *
     * @note This function does not modify any global state.
     */
    bool is_valid_potion_knowledge(int curr_index)
    {
        if (words[curr_index + 2] == "effective" && words[curr_index + 3] == "against")
        {
            if (words[curr_index + 4] != words[word_count - 1])
            {
                return false;
            }
            if (!is_alphabetical(words[curr_index + 4]))
            {
                return false;
            }
            return true;
        }
        return false;
    }

    /**
     * @brief Checks if a candidate potion recipe sentence is valid.
     *
     * This function checks if a line follows the valid potion recipe sentence format. After Geralt learns <potion> potion consists of,
     * the sentence follows a specific structure: "<count> <ingredient>, <count> <ingredient>, ... . We can check with
     * a modular approach if after the curr_index the structure is maintained.
     *
     * @param curr_index The index of the first word after "consists of" in the words vector.
     * @return true if the sentence is valid, false otherwise.
     *
     * @note This function does not modify any global state.
     */
    bool is_valid_potion_recipe(int curr_index)
    {
        // this calculates the number of words after the curr_index
        // to follow the ingredient list structure, the number of words must be 3k + 2
        int cnt = word_count - curr_index;
        if (cnt % 3 != 2)
        {
            return false;
        }

        // a similar loop to loot and trade actions for ingredient list checking
        while (curr_index <= word_count - 1)
        {
            if (!is_integer(words[curr_index]))
            {
                return false;
            }
            cnt = std::stoi(words[curr_index]);
            if (cnt <= 0)
            {
                return false;
            }
            std::string ing = words[curr_index + 1];
            if (!is_alphabetical(ing))
            {
                return false;
            }
            if (curr_index + 2 <= word_count - 1 && words[curr_index + 2] != ",")
            {
                return false;
            }
            curr_index += 3;
        }
        return true;
    }

    /**
     * @brief Checks if a candidate encounter action sentence is valid.
     *
     * This function checks if a line follows the valid encounter action sentence format. After Geralt encounters a, the sentence
     * follows a specific structure: "<monster>". We can check if the monster name is valid, and since monster names are one word long,
     * we can use constant numbers as indexes to check them. Also we should check if word_count matches as well.
     *
     * @return true if the sentence is valid, false otherwise.
     *
     * @note This function does not modify any global state.
     */
    bool is_valid_encounter()
    {
        if (word_count != 4)
        {
            return false;
        }
        if (!is_alphabetical(words[3]))
        {
            return false;
        }
        return true;
    }

    /**
     * @brief Detects the type of sentence.
     *
     * This method is responsible for detecting the type of sentence.
     * Does the validity checks by words, word_counts, valid name checks, valid number checks etc. Returns
     * type or -1 if invalid.
     *
     * @param line The line of text to be checked.
     * @return int The type of sentence:
     *            -1: Invalid sentence
     *           0: Loot action
     *          1: Trade action
     *         2: Brew action
     *        3: Sign knowledge action
     *       4: Potion knowledge action
     *      5: Potion recipe action
     *     6: Encounter action
     *
     * @note This function does not modify any global state.
     */
    int detect_sentence_type(const std::string &line)
    {
        // these are the trivial invalid types
        if (words[0] != "Geralt") // first word not Geralt
        {
            return -1;
        }
        if (word_count <= 2) // not enough words
        {
            return -1;
        }
        if (words[2] == ",") // not one valid type requires a comma in that position
        {
            return -1;
        }
        if (words[word_count - 1] == ",") // last word can't be a comma
        {
            return -1;
        }

        // now let's move on to detect the type and do specific validity checks accordingly
        if (words[1] == "loots") // possible loot sentence
        {
            if (!is_valid_loot())
            {
                return -1;
            }
            return 0;
        }

        else if (words[1] == "trades") // possible trade sentence
        {
            if (!is_valid_trade())
            {
                return -1;
            }
            return 1;
        }

        else if (words[1] == "brews") // possible brew sentence
        {
            // we need to check if the potion name is valid
            // the potion name is between the 3rd and the last word
            if (!is_valid_potion_name(line, 2, word_count - 1))
            {
                return -1;
            }
            return 2;
        }
        else if (words[1] == "learns")
        {
            if (word_count < 8)
            {
                return -1;
            }
            if (words[3] == "sign") // possible sign knowledge sentence
            {
                if (!is_valid_sign_knowledge())
                {
                    return -1;
                }
                return 3;
            }
            else // possible potion or potion recipe knowledge sentence
            {
                bool potion_found = false; // to check if word potion is found in the sentence, both potion learn sentences require
                for (int i = 2; i < word_count; ++i)
                {
                    if (words[i] == "potion")
                    {
                        potion_found = true;
                        break;
                    }
                }
                if (!potion_found) // if not this is a nothing sentence, i.e. invalid
                {
                    return -1;
                }

                // construct the potion name until the word "potion" is found
                // both potion learn sentences follow this format up until here, they will diverse after "potion"
                std::string potion_name = "";
                int curr_index = 2;
                while (words[curr_index] != "potion" && curr_index < word_count)
                {
                    potion_name += words[curr_index];
                    if (words[curr_index + 1] != "potion")
                    {
                        potion_name += " ";
                    }
                    curr_index++;
                }
                if (!is_valid_potion_name(line, 2, curr_index - 1))
                {
                    return -1;
                }
                if (words[curr_index] != "potion")
                {
                    return -1;
                }
                if (words[curr_index + 1] == "is") // possible monster knowledge
                {
                    if (!is_valid_potion_knowledge(curr_index))
                    {
                        return -1;
                    }
                    return 4;
                }
                else if (words[curr_index + 1] == "consists" && words[curr_index + 2] == "of") // possible potion recipe
                {
                    if (!is_valid_potion_recipe(curr_index + 3))
                    {
                        return -1;
                    }
                    return 5;
                }
            }
        }
        else if (words[1] == "encounters" && words[2] == "a") // possible encounter sentence
        {
            if (!is_valid_encounter())
            {
                return -1;
            }
            return 6;
        }
        return -1;
    }

    /**
     * @brief Detects the type of question.
     *
     * This method is responsible for detecting the type of question.
     * Does the validity checks by words, word_counts, valid name checks, valid number checks etc. Returns
     * type or -1 if invalid.
     *
     * @param line The line of text to be checked.
     * @return int The type of question:
     *            -1: Invalid question
     *            0: Total ingredient count
     *           1: Total ingredient count (?)
     *          2: Total potion count
     *         3: Total potion count (?)
     *        4: Total trophy count
     *       5: Total trophy count (?)
     *      6: Monster knowledge
     *     7: Potion formula knowledge
     *
     * @note This function does not modify any global state.
     */
    int detect_question_type(const std::string &line)
    {
        if (words[0] == "Total" && words[1] == "ingredient") // possible total ingredient or specific ingredient
        {
            if (words[2] != "?" && word_count == 4) // specific ingredient
            {
                if (!is_alphabetical(words[2]))
                {
                    return -1;
                }
                return 0;
            }
            if (words[2] == "?" && word_count == 3) // total ingredient
            {
                return 1;
            }
        }
        if (words[0] == "Total" && words[1] == "potion") // possible total potion or specific potion
        {
            if (word_count == 3 && words[2] == "?") // total potion
            {
                return 3;
            }

            // if specific construct potion name
            std::string potion_name = "";

            if (!is_valid_potion_name(line, 2, word_count - 2))
            {
                return -1;
            }
            if (words[word_count - 1] != "?")
            {
                return -1;
            }
            return 2;
        }
        if (words[0] == "Total" && words[1] == "trophy") // possible total trophy or specific trophy
        {
            if (word_count == 3 && words[2] == "?") // total trophy
            {
                return 5;
            }
            if (word_count == 4 && is_alphabetical(words[2])) // specific trophy
            {
                if (words[word_count - 1] != "?")
                {
                    return -1;
                }
                return 4;
            }
        }
        if (words[0] == "What" && words[1] == "is" && words[2] == "effective" && words[3] == "against") // monster knowledge question
        {
            if (word_count != 6)
            {
                return -1;
            }
            if (!is_alphabetical(words[4]))
            {
                return -1;
            }
            return 6;
        }
        if (words[0] == "What" && words[1] == "is" && words[2] == "in") // potion formula knowledge question
        {
            std::string potion_name = "";
            int curr_index = 3;
            while (words[curr_index] != "?" && curr_index < word_count)
            {
                potion_name += words[curr_index];
                if (words[curr_index + 1] != "?")
                {
                    potion_name += " ";
                }
                curr_index++;
            }
            if (!is_valid_potion_name(line, 3, curr_index - 1))
            {
                return -1;
            }
            if (words[curr_index] != "?")
            {
                return -1;
            }
            if (word_count == curr_index + 1)
            {
                return 7;
            }
        }
        return -1;
    }
};