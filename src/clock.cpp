#include <iostream>
#include <chrono>
#include <thread>
#include <vector>
#include <sstream>
#include <ctime>
#ifdef _WIN32
#include <conio.h>
#endif

#define CLOCK_STARTING_COLUMN 30 //Where the clock is placed (COLUMN, ROW)
#define CLOCK_STARTING_ROW 12

#define COLOR_ATTRIBUTE "0A"    //The color attribute used for the CMD command "color" 
#define ASCII_DIGITS_COLUMNS 8 //Specifies how many columns the ASCII ART number has
#define ASCII_DOTS_COLUMNS 4    //Same as above but for the ASCII ART that represents the dots
#define DOTS_INDEX 10           //The index of the `dots` element in the array

void hideCursor();
void showCursor();
void resetText();
void printTimeUnit(std::ostringstream& out, int timeUnit, int& x, int& y);
void printFirstDots(std::ostringstream& out, int& x, int& y);
void printSecondDots(std::ostringstream& out, int& x, int& y);
void printCurrentTime(std::ostringstream& out, int hour, int minute, int second);
void clearScreen();
void changeColors(const std::string& colorAttribute);
void moveCursor(std::ostringstream& out, int x, int y);

const std::vector<std::vector<std::string>> ascii_digits = {
    {
        "  ___   ",
        " / _ \\  ",
        "| | | | ",
        "| | | | ",
        "| |_| | ",
        " \\___/  "
    }, 
    {
        "   __   ",
        "  /_ |  ",
        "   | |  ",
        "   | |  ",
        "   | |  ",
        "   |_|  "
    }, 
    {
        "  ___   ",
        " |__ \\  ",
        "    ) | ",
        "   / /  ",
        "  / /_  ",
        " |____| "
    }, 
    {
        " ____   ",
        "|___ \\  ",
        "  __) | ",
        " |__ <  ",
        " ___) | ",
        "|____/  "
    }, 
    {
        " _  _   ",
        "| || |  ",
        "| || |  ",
        "|__  |  ",
        "   | |  ",
        "   |_|  "
    }, 
    {
        " _____  ",
        "| ____| ",
        "| |__   ",
        "|___ \\  ",
        " ___) | ",
        "|____/  "

    }, 
    {
        "   __   ",
        "  / /   ",
        " / /_   ",
        "| '_ \\  ",
        "| (_) | ",
        " \\___/  "
    },
    {
        " ______  ",
        "|____  | ",
        "    / /  ",
        "   / /   ",
        "  / /    ",
        " /_/     "
    }, 
    {
        "  ___   ",
        " / _ \\  ",
        "| (_) | ",
        " > _ <  ",
        "| (_) | ",
        " \\___/  "
    }, 
    {
        "  ___   ",
        " / _ \\  ",
        "| (_) | ",
        " \\__, | ",
        "   / /  ",
        "  /_/   "
    }, 
    {
        "    ",
        " _  ",
        "(_) ",
        "    ",
        " _  ",
        "(_) "
    }
};

int main() {
    int hour = 0, minute = 0, second = 0;

    #ifdef _WIN32
    system("chcp 65001");
    #endif

    #ifdef __linux__
    system("stty -echo");
    #endif

    clearScreen();
    hideCursor();
    changeColors(COLOR_ATTRIBUTE);

    while (true) {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);
        std::tm* local_time = std::localtime(&now_time);

        hour = local_time->tm_hour;
        minute = local_time->tm_min;
        second = local_time->tm_sec;

        std::ostringstream out;
        printCurrentTime(out, hour, minute, second);
        std::cout << out.str() << std::flush;

        now += std::chrono::seconds(1);
        std::this_thread::sleep_until(now);

        #ifdef _WIN32
        if (_kbhit()) {
            char ch = _getch();
            if (ch == 'q' || ch == 'Q') break;
        }
        #endif
    }

    showCursor();
    resetText();
    return 0;
}

void moveCursor(std::ostringstream& out, int x, int y) {
    out << "\033[" << y << ";" << x << "H";
}

void hideCursor() {
    std::cout << "\033[?25l";
}

void showCursor() {
    std::cout << "\033[?25h";
}

void resetText() {
    std::cout << "\033[0m";
}

void clearScreen() {
    std::cout << "\033[2J\033[H";
}

void changeColors(const std::string& colorAttribute) {
    #ifdef _WIN32
    system(("color " + colorAttribute).c_str());
    #elif __linux__
    std::cout << "\033[32m";
    #endif
}

void printTimeUnit(std::ostringstream& out, int timeUnit, int& x, int& y) {
    std::vector<int> digits;
    if (timeUnit == 0) {
        digits.push_back(0);
    } else {
        while (timeUnit > 0) {
            digits.insert(digits.begin(), timeUnit % 10);
            timeUnit /= 10;
        }
    }

    if (digits.size() == 1) {
        for (const auto& line : ascii_digits[0]) {
            moveCursor(out, x, y++);
            out << line;
        }
        x += ASCII_DIGITS_COLUMNS;
        y = CLOCK_STARTING_ROW;
    }

    for (int digit : digits) {
        for (const auto& line : ascii_digits[digit]) {
            moveCursor(out, x, y++);
            out << line;
        }
        if (digits.size() > 1) {
            x += ASCII_DIGITS_COLUMNS;
            y = CLOCK_STARTING_ROW;
        }
    }
}

void printFirstDots(std::ostringstream& out, int& x, int& y) {
    x = CLOCK_STARTING_COLUMN + ASCII_DIGITS_COLUMNS * 2 + 1;
    y = CLOCK_STARTING_ROW;
    for (const auto& line : ascii_digits[DOTS_INDEX]) {
        moveCursor(out, x, y++);
        out << line;
    }
    x += ASCII_DOTS_COLUMNS + 1;
    y = CLOCK_STARTING_ROW;
}

void printSecondDots(std::ostringstream& out, int& x, int& y) {
    x = CLOCK_STARTING_COLUMN + ASCII_DIGITS_COLUMNS * 2 + 1 + ASCII_DOTS_COLUMNS + 1 + ASCII_DIGITS_COLUMNS * 2 + 1;
    y = CLOCK_STARTING_ROW;
    for (const auto& line : ascii_digits[DOTS_INDEX]) {
        moveCursor(out, x, y++);
        out << line;
    }
    x += ASCII_DOTS_COLUMNS + 1;
    y = CLOCK_STARTING_ROW;
}

void printCurrentTime(std::ostringstream& out, int hour, int minute, int second) {
    int x = CLOCK_STARTING_COLUMN;
    int y = CLOCK_STARTING_ROW;

    printTimeUnit(out, hour, x, y);
    printFirstDots(out, x, y);
    printTimeUnit(out, minute, x, y);
    printSecondDots(out, x, y);
    printTimeUnit(out, second, x, y);
}


/*METADA FOR THIS FONT 
    CLOCK_STARTING_COLUMN -> 5
    CLOCK_STARTING_ROW    -> 11
    ASCII_DIGITS_COLUMNS  -> 15
    ASCII_DOTS_COLUMNS    -> 9
*/
/*const std::vector<std::vector<std::string>> ascii_digits = {
    {
        "░▒▓████████▓▒░ ",
        "░▒▓█▓▒░░▒▓█▓▒░ ",
        "░▒▓█▓▒░░▒▓█▓▒░ ",
        "░▒▓█▓▒░░▒▓█▓▒░ ",
        "░▒▓█▓▒░░▒▓█▓▒░ ",
        "░▒▓█▓▒░░▒▓█▓▒░ ",
        "░▒▓████████▓▒░ "
    }, 
    {
        "    ░▒▓██▓▒░   ",
        " ░▒▓█████▓▒░   ",
        "    ░▒▓██▓▒░   ",
        "    ░▒▓██▓▒░   ",
        "    ░▒▓██▓▒░   ",
        "    ░▒▓██▓▒░   ",
        "    ░▒▓██▓▒░   "
    }, 
    {
        "░▒▓███████▓▒░  ",
        "       ░▒▓█▓▒░ ",
        "       ░▒▓█▓▒░ ",
        " ░▒▓██████▓▒░  ",
        "░▒▓█▓▒░        ",
        "░▒▓█▓▒░        ",
        "░▒▓████████▓▒░ "
    }, 
    {
        "░▒▓███████▓▒░  ",
        "       ░▒▓█▓▒░ ",
        "       ░▒▓█▓▒░ ",
        "░▒▓███████▓▒░  ",
        "       ░▒▓█▓▒░ ",
        "       ░▒▓█▓▒░ ",
        "░▒▓███████▓▒░  "
    }, 
    {
        "░▒▓█▓▒░░▒▓█▓▒░ ",
        "░▒▓█▓▒░░▒▓█▓▒░ ",
        "░▒▓█▓▒░░▒▓█▓▒░ ",
        "░▒▓████████▓▒░ ",
        "       ░▒▓█▓▒░ ",
        "       ░▒▓█▓▒░ ",
        "       ░▒▓█▓▒░ "
    }, 
    {
        "░▒▓████████▓▒░ ",
        "░▒▓█▓▒░        ",
        "░▒▓█▓▒░        ",
        "░▒▓███████▓▒░  ",
        "       ░▒▓█▓▒░ ",
        "       ░▒▓█▓▒░ ",
        "░▒▓███████▓▒░  "

    }, 
    {
        " ░▒▓███████▓▒░ ",
        "░▒▓█▓▒░        ",
        "░▒▓█▓▒░        ",
        "░▒▓███████▓▒░  ",
        "░▒▓█▓▒░░▒▓█▓▒░ ",
        "░▒▓█▓▒░░▒▓█▓▒░ ",
        " ░▒▓██████▓▒░  "
    },
    {
        "░▒▓████████▓▒░ ",
        "░▒▓█▓▒░░▒▓█▓▒░ ",
        "       ░▒▓█▓▒░ ",
        "      ░▒▓█▓▒░  ",
        "      ░▒▓█▓▒░  ",
        "     ░▒▓█▓▒░   ",
        "     ░▒▓█▓▒░   "
    }, 
    {
        " ░▒▓██████▓▒░  ",
        "░▒▓█▓▒░░▒▓█▓▒░ ",
        "░▒▓█▓▒░░▒▓█▓▒░ ",
        " ░▒▓██████▓▒░  ",
        "░▒▓█▓▒░░▒▓█▓▒░ ",
        "░▒▓█▓▒░░▒▓█▓▒░ ",
        " ░▒▓██████▓▒░  "
    }, 
    {
        " ░▒▓██████▓▒░  ",
        "░▒▓█▓▒░░▒▓█▓▒░ ",
        "░▒▓█▓▒░░▒▓█▓▒░ ",
        " ░▒▓███████▓▒░ ",
        "       ░▒▓█▓▒░ ",
        "       ░▒▓█▓▒░ ",
        " ░▒▓██████▓▒░  "
    }, 
    {
        "         ",
        "░▒▓██▓▒░ ",
        "░▒▓██▓▒░ ",
        "         ",
        "░▒▓██▓▒░ ",
        "░▒▓██▓▒░ "
    }
};*/
