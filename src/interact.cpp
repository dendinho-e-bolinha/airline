#include <iostream>
#include <stdexcept>
#include "interact.h"

using namespace std;

void MenuBlock::addOption(const std::string &text, const std::function<void(const Menu*&)> &callback) {
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

    this->special_block = block;
}

void Menu::print() const {
    cout << title << '\n' << endl;

    if (this->blocks.empty()) {
        cout << "Spooky... There is nothing to see here..." << endl;
        return;
    }

    size_t option_number = this->special_block.getOptions().size();
    for (const MenuBlock &block : this->blocks) {
        for (const MenuOption &option: block.getOptions()) {
            cout << '[' << option_number << "] "
                 << option.first << '\n';

            option_number++;
        }

        cout << endl;
    }

    option_number = 0;
    for (const MenuOption &option: this->special_block.getOptions()) {
        cout << '[' << option_number << "] "
             << option.first << '\n';

        option_number++;
    }

    cout << endl;
}

void Menu::show() const {
    this->print();
}
