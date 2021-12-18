#include <iostream>
#include <stdexcept>
#include <limits>
#include <sstream>
#include "interact.h"

using namespace std;

const char *end_of_file_exception::what() const noexcept {
    return "EOF";
}

void MenuBlock::addOption(const std::string &text, const std::function<void()> &callback) {
    MenuOption option = make_pair(text, callback);
    this->options.push_back(option);
}

const std::vector<MenuOption> &MenuBlock::getOptions() const {
    return this->options;
}

Menu::Menu(const std::string &title) : title(title) {}

void Menu::addBlock(const MenuBlock &block) {
    if (block.getOptions().empty())
        throw invalid_argument("Block cannot be empty");

    this->blocks.push_back(block);
}

void Menu::setSpecialBlock(const MenuBlock &block) {
    if (block.getOptions().empty())
        throw invalid_argument("Block cannot be empty");

    this->special_block = block;
}

void Menu::printOptions() const {
    size_t option_number = 1;
    for (const MenuBlock &block : this->blocks) {
        for (const MenuOption &option: block.getOptions()) {
            cout << '[' << option_number << "] "
                 << option.first << '\n';

            option_number++;
        }

        cout << endl;
    }

    if (!special_block.getOptions().empty()) {
        for (const MenuOption &option: this->special_block.getOptions()) {
            cout << '[' << option_number << "] "
                 << option.first << '\n';

            option_number++;
        }

        cout << endl;
    }
}

MenuOption const &Menu::getSelectedOption() const {
    size_t num_options = this->special_block.getOptions().size();
    for (const MenuBlock &block : this->blocks)
        num_options += block.getOptions().size();

    ostringstream prompt_stream;
    prompt_stream << "Your option [1";
    if (num_options > 1)
        prompt_stream << " - " << num_options;

    prompt_stream << "]: ";

    string prompt = prompt_stream.str();
    function<bool(size_t)> is_option = [num_options](size_t value) {
        return value >= 1 && value <= num_options;
    };

    size_t selected_option;
    read_value(prompt, "Please input a number from the list", selected_option, is_option);

    for (const MenuBlock &block : this->blocks) {
        if (selected_option > block.getOptions().size()) {
            selected_option -= block.getOptions().size();
            continue;
        }

        return block.getOptions().at(selected_option - 1);
    }

    return this->special_block.getOptions().at(selected_option - 1);
}

void Menu::show() const {
    cout << "\x1B[2J\x1B[;H"
         << title << '\n' << endl;

    if (this->blocks.empty()) {
        cout << "Spooky... There is nothing to see here..." << endl;
        return;
    }

    this->printOptions();
    MenuOption  const &option = this->getSelectedOption();
    cout << endl;

    option.second();
}
