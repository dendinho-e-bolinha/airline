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

class validation_error : public std::exception {
    const char* reason;
public:
    validation_error(const char* what);
    virtual const char* what() const noexcept;
};


// Type alias
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

    /**
    * @brief Displays all the options on the console according to the menu
    */
    void printOptions() const;

    /**
    * @brief Reads user's selected option by input
    * @return The selected option, if valid
    */
    MenuOption const &getSelectedOption() const;

public:

    /**
     * @brief Creates a Menu instance
     * @param title The menu title
     */
    explicit Menu(const std::string &title);

    /**
    * @brief Adds the given block to the menu
    * @param block The block to add to the menu
    */
    void addBlock(const MenuBlock &block);

    /**
    * @brief Adds the given special block to the menu
    * @param block The block to add to the menu
    */
    void setSpecialBlock(const MenuBlock &block);

    /**
    * @brief Display's the menu's title and options, if they exist
    */
    void show() const;

    /**
    * @brief Display's the menu's title and subtitle and options, if they exist
    * @param subtitle The menu's subtitle
    */
    void show(const std::string& subtitle) const;
};

class GetLine : public std::string {
public:
    friend std::istream& operator>>(std::istream& in, GetLine &value);
};

/**
 * @brief Waits for any user input
 */
void waitForInput();


/**
 * @brief Prompts the user to provide a value through console input.
 * On interactive terminals, it will be done in a "fancy" fashion and in a single line.
 * Otherwise, it will be done in a more "traditional" manner. 
 * 
 * @param prompt The text that will be used to prompt the user for a value.
 * @param warning The error message that will be presented to the user if the value could not be parsed or if the validator function returns false.
 * @param validator An optional function that determines if the value is valid.
 * This function is only executed after the parsing of the value was successful.
 * If this function throws a string, it will be presented to the user as an error message.
 * If this function returns false, the warning message will be presented to the user.
 * 
 * @return The value which was read from the input. 
 */
template <typename T>
T readValue(const std::string prompt, const std::string warning, const std::function<bool(const T&)> validator = [](const T&) { return true; }) noexcept(false) {
    T result;

    std::cout << "\x1B[1;33m?\x1B[0m " << prompt << std::flush;
    while (true) {
        // Is true if, and only if, all the content present on the line is of the type T
        bool is_input_valid = false;
        const char *error = warning.c_str();

        try {
            is_input_valid = std::cin.peek() != '\n'
                             && std::cin >> result && std::cin.peek() == '\n'
                             && validator(result);
        } catch (validation_error exception) {
            error = exception.what();
        }

        // Clear the buffer
        // We do this to avoid having "123a[EOF]" trigger the loop more than once.
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (is_input_valid) {
            std::cout << "\x1B[F\x1B[G\x1B[32m✓\x1B[0m\x1B[" << prompt.length() + 3 << "G\x1B[K" << result << "\n\x1B[K" << std::flush;
            return result;
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
