#ifndef AIRLINE_INTERACT_H
#define AIRLINE_INTERACT_H

#include <vector>
#include <list>
#include <string>
#include <functional>

class Menu;

using MenuOption = std::pair<std::string, std::function<void(const Menu*&)>>;

class MenuBlock {
    std::vector<MenuOption> options;

public:
    void addOption(const std::string &text, const std::function<void(const Menu*&)> &callback);
    std::vector<MenuOption> const &getOptions() const;
};

class Menu {
    std::string title;
    std::list<MenuBlock> blocks;
    MenuBlock special_block;

    void print() const;
    MenuOption getSelectedOption() const;

public:
    explicit Menu(const std::string &title);
    void addBlock(const MenuBlock &block);
    void setSpecialBlock(const MenuBlock &block);
    void show() const;
};


#endif //AIRLINE_INTERACT_H
