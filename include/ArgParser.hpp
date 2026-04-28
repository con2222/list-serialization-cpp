#pragma once

#include <string>

struct AppConfig {
    std::string_view inputFilename;
    std::string_view outputFilename;
    
    bool isDeserializeMode = false;
    std::string_view deserializeInput;
    std::string_view deserializeOutput;

    bool showHelp = false;
    bool isValid = true;
};

std::string_view get_cmd_option(char** begin, char** end,  const std::string& option, std::string_view defaultValue = "");
bool cmd_option_exists(char** begin, char** end, const std::string& option);

AppConfig parse_arguments(int argc, char* argv[]);
void print_help(const char* program_name);
