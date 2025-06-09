/**
 * @namespace Utils
 * @brief Utility functions for text processing and validation.
 *
 * This namespace contains helper functions used for processing and validating input lines.
 * It includes logic for splitting input into tokens, verifying syntax and grammar rules,
 * and detecting the type of input (sentence, question, or exit command).
 *
 * These functions do not modify the inventory state. They operate solely on the global
 * `words` vector and `word_count` variable to assist in pre-validation and classification
 * before any action is executed.
 */

#pragma once
#include <string>
#include <vector>

namespace Utils
{
    extern std::vector<std::string> words;
    extern int word_count;

    void split_line(const std::string &line);
    bool is_alphabetical(const std::string &str);
    bool is_integer(const std::string &str);
    int detect_type();
    int detect_sentence_type(const std::string &line);
    bool is_valid_potion_name(const std::string &line, int start_index, int end_index);

    int detect_question_type(const std::string &line);
}
