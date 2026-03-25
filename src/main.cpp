#include "ArgParser.hpp"
#include "FileParser.hpp"
#include "List.hpp"

// std
#include <iostream>

int main(int argc, char* argv[]) {
    AppConfig config = parse_arguments(argc, argv);

    if (config.showHelp) {
        print_help(argv[0]);
        return 0;
    }

    if (!config.isValid) {
        return 1;
    }

    List list;
    std::unordered_map<ListNode*, int32_t> randIndexMap;

    if (config.isDeserializeMode) {
        std::cout << "Запуск в режиме десериализации...\n";
        
        if (!list.deserialize_list(config.deserializeInput)) {
            std::cerr << "Критическая ошибка: не удалось десериализовать список.\n";
            return 1;
        }

        if (!list.save_to_text(config.deserializeOutput)) {
            std::cerr << "Критическая ошибка: не удалось сохранить текстовый файл.\n";
            return 1;
        }
        
        std::cout << "Успех! Данные восстановлены и сохранены в текст: " << config.deserializeOutput << "\n";

    } else {
        
        if (!populate_list_from_file(list, randIndexMap, config.inputFilename)) {
            return 1;
        }

        if (!list.serialize_list(randIndexMap, config.outputFilename)) {
            std::cerr << "Произошла ошибка сериализации" << std::endl;
            return 1;
        }

        std::cout << "Успех! Данные успешно сериализованы в файл: " << config.outputFilename << std::endl;
    }

    return 0;
}