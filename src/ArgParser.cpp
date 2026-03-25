#include "ArgParser.hpp"

// std
#include <iostream>
#include <algorithm>

std::string_view get_cmd_option(char** begin, char** end,  const std::string& option, const std::string& defaultValue) {
    char** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) {
        return std::string_view(*itr);
    }
    return defaultValue;
}

bool cmd_option_exists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}

void print_help(const char* program_name) {
    std::cout << "Usage: " << program_name << " [options]\n"
              << "Options:\n"
              << " -i <имя входного файла>            - Имя файла, с которого считывается информация для списка\n"
              << " -o <имя выходного бинарного файла> - Имя файла, в котором хранится результат сериализации\n\n"
              << "По умолчанию имя входного файла \"inlet.in\", выходного файла \"outlet.in\"\n";
}

AppConfig parse_arguments(int argc, char* argv[]) {
    AppConfig config;
    config.inputFilename = "inlet.in";   // Значения по умолчанию
    config.outputFilename = "outlet.in";

    if (cmd_option_exists(argv, argv + argc, "-h") || cmd_option_exists(argv, argv + argc, "--help")) { 
        config.showHelp = true;
        return config;
    }

    if (cmd_option_exists(argv, argv + argc, "-i")) {
        config.inputFilename = get_cmd_option(argv, argv + argc, "-i");
    }
    if (cmd_option_exists(argv, argv + argc, "-o")) {
        config.outputFilename = get_cmd_option(argv, argv + argc, "-o");
    }

    return config;
}