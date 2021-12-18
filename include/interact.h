#ifndef AIRLINE_INTERACT_H
#define AIRLINE_INTERACT_H

#include <vector>
#include <list>
#include <string>
#include <functional>
#include <limits>
#include <iostream>

class end_of_file_exception : public std::exception {
public:
    virtual const char* what() const noexcept;
};

using MenuOption = std::pair<std::string, std::function<void()>>;

class MenuBlock {
    std::vector<MenuOption> options;

public:
    void addOption(const std::string &text, const std::function<void()> &callback);
    std::vector<MenuOption> const &getOptions() const;
};

class Menu {
    std::string title;
    std::list<MenuBlock> blocks;
    MenuBlock special_block;

    void printOptions() const;
    MenuOption const &getSelectedOption() const;

public:
    explicit Menu(const std::string &title);
    void addBlock(const MenuBlock &block);
    void setSpecialBlock(const MenuBlock &block);
    void show() const;
};

class GetLine {
    std::string buffer;
public:
    friend std::istream& operator>>(std::istream& in, GetLine &value);
    friend std::ostream& operator<<(std::ostream& out, const GetLine &value);
    std::string operator()() const;
};

void waitForInput();

template <typename T>
void read_value(const std::string prompt, const std::string warning, T &result, const std::function<bool(T)> validator = [](T) { return true; }) noexcept(false) {
    std::cout << "\x1B[1;33m?\x1B[0m " << prompt << std::flush;
    while (true) {
        // Is true if, and only if, all the content present on the line is of the type T
        bool is_input_valid = false;
        const char *error = warning.c_str();

        try {
            is_input_valid = std::cin.peek() != '\n'
                             && std::cin >> result && std::cin.peek() == '\n'
                             && validator(result);
        } catch (const char *exception) {
            error = exception;
        }

        // Clear the buffer
        // We do this to avoid having "123a[EOF]" trigger the loop more than once.
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (is_input_valid) {
            std::cout << "\x1B[F\x1B[G\x1B[32m✓\x1B[0m\x1B[" << prompt.length() + 3 << "G\x1B[K" << result << "\n\x1B[K" << std::flush;
            return;
        } else {
            if (!std::cin.eof())
                std::cout << "\x1B[31m>>\x1B[0m " << error << "\x1B[K\x1B[F";

            std::cout << "\x1B[" << prompt.length() + 3 << "G\x1B[K";

            if (std::cin.eof()) {
                std::cout << "EOF\n\x1B[F\x1B[G\x1B[31m×\x1B[0m\x1B[E\x1B[K" << std::flush;
                throw end_of_file_exception();
            }
        }
    }
}

#endif //AIRLINE_INTERACT_H
