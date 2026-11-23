#ifndef UI_HPP
#define UI_HPP

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <thread>
#include <chrono>

#ifdef _WIN32
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <windows.h>
#endif

namespace UI {
    // ANSI Color Codes
    inline const std::string RESET      = "\033[0m";
    inline const std::string BOLD       = "\033[1m";
    inline const std::string DIM        = "\033[2m";
    inline const std::string ITALIC     = "\033[3m";
    inline const std::string UNDERLINE  = "\033[4m";

    inline const std::string BLACK      = "\033[30m";
    inline const std::string RED        = "\033[91m";
    inline const std::string GREEN      = "\033[92m";
    inline const std::string YELLOW     = "\033[93m";
    inline const std::string BLUE       = "\033[94m";
    inline const std::string MAGENTA    = "\033[95m";
    inline const std::string CYAN       = "\033[96m";
    inline const std::string WHITE      = "\033[97m";
    inline const std::string GRAY       = "\033[90m";

    // Backgrounds
    inline const std::string BG_CYAN    = "\033[46;30m";
    inline const std::string BG_GREEN   = "\033[42;30m";
    inline const std::string BG_RED     = "\033[41;37m";
    inline const std::string BG_MAGENTA = "\033[45;37m";
    inline const std::string BG_DARK    = "\033[48;5;236m";

    inline void initTerminal() {
#ifdef _WIN32
        // Enable UTF-8 code page and virtual terminal processing for ANSI escape sequences
        SetConsoleOutputCP(CP_UTF8);
        HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
        if (hOut != INVALID_HANDLE_VALUE) {
            DWORD dwMode = 0;
            if (GetConsoleMode(hOut, &dwMode)) {
                dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
                SetConsoleMode(hOut, dwMode);
            }
        }
#endif
    }

    inline void clearScreen() {
        std::cout << "\033[2J\033[H";
    }

    inline void sleepMs(int ms) {
        std::this_thread::sleep_for(std::chrono::milliseconds(ms));
    }

    inline void printBanner() {
        std::cout << CYAN << BOLD;
        std::cout << R"(
    ██████╗  ███████╗ █████╗      ██████╗ ██╗   ██╗██╗███████╗    █████╗ ██████╗ ███████╗███╗   ██╗ █████╗ 
    ██╔══██╗██╔════╝██╔══██╗    ██╔═══██╗██║   ██║██║╚══███╔╝   ██╔══██╗██╔══██╗██╔════╝████╗  ██║██╔══██╗
    ██║  ██║███████╗███████║    ██║   ██║██║   ██║██║  ███╔╝    ███████║██████╔╝█████╗  ██╔██╗ ██║███████║
    ██║  ██║╚════██║██╔══██║    ██║▄▄ ██║██║   ██║██║ ███╔╝     ██╔══██║██╔══██╗██╔══╝  ██║╚██╗██║██╔══██║
    ██████╔╝███████║██║  ██║    ╚██████╔╝╚██████╔╝██║███████╗   ██║  ██║██║  ██║███████╗██║ ╚████║██║  ██║
    ╚═════╝ ╚══════╝╚═╝  ╚═╝     ╚══▀▀═╝  ╚═════╝ ╚═╝╚══════╝   ╚═╝  ╚═╝╚═╝  ╚═╝╚══════╝╚═╝  ╚═══╝╚═╝  ╚═╝
        )" << RESET << "\n";
        std::cout << GRAY << "        >>> High-Performance Competitive DSA Arena • Powered by Custom C++ Data Structures <<<\n" << RESET << "\n";
    }

    inline void printBox(const std::string& title, const std::vector<std::string>& lines, const std::string& borderColor = CYAN, int width = 74) {
        std::cout << borderColor << "  ╭─" << BOLD << " " << title << " " << RESET << borderColor;
        int titleLen = static_cast<int>(title.length()) + 2;
        for (int i = 0; i < width - titleLen - 4; ++i) std::cout << "─";
        std::cout << "╮" << RESET << "\n";

        for (const auto& line : lines) {
            std::cout << borderColor << "  │ " << RESET;
            std::cout << std::left << std::setw(width - 4) << line;
            std::cout << borderColor << " │" << RESET << "\n";
        }

        std::cout << borderColor << "  ╰";
        for (int i = 0; i < width - 2; ++i) std::cout << "─";
        std::cout << "╯" << RESET << "\n";
    }

    inline void printProgressBar(int current, int total, int barWidth = 30) {
        float progress = total > 0 ? (float)current / total : 0.0f;
        int pos = static_cast<int>(barWidth * progress);

        std::cout << "  Progress: " << CYAN << "[";
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos) std::cout << "█";
            else if (i == pos) std::cout << "▓";
            else std::cout << "░";
        }
        std::cout << "] " << BOLD << int(progress * 100.0) << "% " << RESET 
                  << "(" << current << "/" << total << ")\n";
    }

    inline void pressAnyKey() {
        std::cout << "\n" << GRAY << "  [Press Enter to continue...]" << RESET;
        std::cin.ignore(10000, '\n');
    }
}

#endif // UI_HPP
