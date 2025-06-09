# Witcher Tracker (C++ Version)

**Course:** CMPE230 – Systems Programming, Spring 2025

**Authors:** Salih Can Erer (2022400174), Beyza Nur Deniz (2021400285)

---

## Table of Contents

1. [Introduction](#introduction)
2. [Features](#features)
3. [Prerequisites](#prerequisites)
4. [Project Structure](#project-structure)
5. [Building and Running](#building-and-running)
6. [Testing & Grading](#testing--grading)
7. [Usage Examples](#usage-examples)
8. [Class Descriptions](#class-descriptions)
   - [Utils](#utils)
   - [Inventory](#inventory)
   - [Potion](#potion)
   - [Monster](#monster)
   - [main.cpp](#maincpp)
9. [Grammar and Validation](#grammar-and-validation)
10. [License](#license)

---

## Introduction

The **Witcher Tracker** is a command-line based interpreter and inventory-event tracking system inspired by the Witcher universe. Geralt of Rivia can:

- Loot alchemical ingredients
- Trade monster trophies for ingredients
- Brew potions using known recipes
- Learn signs and potion effectiveness against monsters
- Encounter monsters, consume effective counters, and collect trophies
- Answer structured queries about inventory, bestiary, and alchemy

This C++ implementation emphasizes modular, object-oriented design, strict grammar enforcement based on the assignment's BNF, and interactive REPL usability.

## Features

- **Loot Ingredients**: Add one or more ingredients with quantities.
- **Trade Trophies**: Exchange trophies for ingredients in multi-item trades.
- **Brew Potions**: Brew if Geralt knows the formula and has required ingredients.
- **Learn Knowledge**: Store sign or potion effectiveness against monsters.
- **Learn Recipes**: Store new potion formulas in the alchemy base.
- **Encounters**: Survive encounters using signs/potions and earn trophies.
- **Queries**: Retrieve totals of ingredients, potions, trophies, bestiary knowledge, and potion formulas.

## Prerequisites

- C++12 or later compiler (e.g., `g++`)
- GNU Make
- Standard C++ STL (`<map>`, `<vector>`, `<set>`, `<regex>`)

## Project Structure

```
witcher-tracker-cpp/
├── Makefile                    # Builds the `witchertracker` executable
├── README.md                   # Project overview and instructions
├── .gitignore                  # Common C/C++ ignores
├── LICENSE                     # MIT License file
├── docs/
│   └── report.pdf              # Detailed project report
├── test/                       # Unit tests or integration tests
│   ├── checker.py
│   └── grader.py
├── test-cases/                 # Sample input/output test files
│   ├── input1.txt
│   ├── output1.txt
│   ├── input2.txt
│   ├── output2.txt
│   └── ...
└── src/                        # Source code directory
    ├── main.cpp                # Entry point and REPL loop
    ├── Inventory.h             # Inventory class declarations
    ├── Inventory.cpp           # Inventory implementation
    ├── Potion.h                # Potion class declarations
    ├── Potion.cpp              # Potion implementation
    ├── Monster.h               # Monster class declarations
    ├── Monster.cpp             # Monster implementation
    ├── Utils.h                 # Parsing and validation declarations
    └── Utils.cpp               # Grammar enforcement and parsing
```

## Building and Running

1. **Build**
   ```bash
   make
   ```
   Produces the `witchertracker` executable.

2. **Run**
   ```bash
   ./witchertracker
   ```

3. **Exit** by typing `Exit` or pressing `Ctrl+D` (EOF).

## Testing & Grading

Run automated grading and test-case checks:

```bash
# Run unit/integration tests
make grade

# Check a single test case
python3 test/checker.py checker.py <executable> <input_file> <output_file> <expected_output_file>

# Grade with specified paths
python3 test/grader.py grader.py <executable-path> <test-cases-path>
```

## Usage Examples

```bash
>> Geralt loots 5 Rebis, 3 Vitriol
Alchemy ingredients obtained

>> Geralt learns Swallow potion consists of 3 Vitriol, 2 Rebis
New alchemy formula obtained: Swallow

>> Geralt brews Swallow
Alchemy item created: Swallow

>> Total ingredient ?
2 Rebis, 0 Vitriol

>> Geralt learns Igni sign is effective against Harpy
New bestiary entry added: Harpy

>> Geralt encounters a Harpy
Geralt defeats Harpy

>> Total trophy Harpy ?
1

>> Exit
```

## Class Descriptions

### Utils

- **Purpose**: Tokenize input lines, detect command types (sentence/question/exit), and enforce grammar based on BNF.
- **Key Functions**:
  - `split_line(line)`: Splits raw input into tokens on spaces, commas, and question marks.
  - `detect_type()`: Returns code for Sentence (0), Question (1), Exit (2), or invalid.
  - `detect_sentence_type(line)`: Identifies specific sentences: loot, trade, brew, learn, encounter.
  - `detect_question_type(line)`: Identifies query patterns: totals, bestiary, formula.
  - Helper validators: `is_integer`, `is_alphabetical`, `is_valid_potion_name`.

### Inventory

- **Purpose**: Manages Geralt’s in-memory state of ingredients, potions, trophies, recipes, and bestiary.
- **Data Members**:
  - `std::map<std::string,int> ingredients;`
  - `std::map<std::string,int> potion_counts;`
  - `std::map<std::string,int> trophies;`
  - `std::map<std::string,Potion> potions;`
  - `std::map<std::string,Monster> monsters;`
- **Core Methods**:
  - `handle_loot()`, `handle_trade()`, `handle_brew()`, `handle_sign_knowledge()`,
    `handle_potion_knowledge()`, `handle_potion_recipe()`, `handle_encounter()`.
  - Query methods: `print_ingredients()`, `get_ingredient_count()`, `print_potions()`,
    `get_potion_count()`, `print_trophies()`, `get_trophy_count()`,
    `print_monster_knowledge()`, `print_potion_formula()`.

### Potion

- **Purpose**: Represents and stores a potion’s formula.
- **Members**:
  - `std::vector<std::pair<std::string,int>> ingredients;`
- **Methods**:
  - `get_ingredients()`: Returns ingredients sorted by descending quantity, then name.
  - `set_ingredients(...)`: Updates the recipe.

### Monster

- **Purpose**: Tracks effective signs and potions against a monster.
- **Members**:
  - `std::set<std::string> signs_against;`
  - `std::set<std::string> potions_against;`
- **Methods**:
  - `add_sign(sign)`, `add_potion(potion)`; retrieval via `get_signs()`, `get_potions()`.

### main.cpp

- **Purpose**: Entry point with interactive REPL.
- **Execution Flow**:
  1. Prompt `>> ` and read line with `getline`.
  2. Call `execute_line(line)`:
     - Tokenize via `Utils`.
     - Classify as question, sentence, or exit.
     - Dispatch to `Inventory` handlers or query methods.
  3. Loop until `Exit` or EOF.

## Grammar and Validation

All input is validated according to the project’s BNF grammar. Invalid or malformed inputs result in `INVALID`. See `Utils.cpp/.h` for the complete grammar enforcement.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.
