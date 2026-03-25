#include "ArgParser.hpp"
#include "FileParser.hpp"
#include "List.hpp"

// std
#include <iostream>

int main(int argc, char* argv[]) {
    // 1. Парсим аргументы
    AppConfig config = parse_arguments(argc, argv);

    if (config.showHelp) {
        print_help(argv[0]);
        return 0;
    }

    if (!config.isValid) {
        return 1;
    }

    // 2. Создаем список и словарь для маппинга
    List list;
    std::unordered_map<ListNode*, int32_t> randIndexMap;

    // 3. Заполняем список из файла
    if (!populate_list_from_file(list, randIndexMap, config.inputFilename)) {
        // Ошибки уже выведены внутри функции populate_list_from_file
        return 1;
    }

    // 4. Сериализуем данные
    if (!list.serialize_list(randIndexMap, config.outputFilename)) {
        std::cerr << "Произошла ошибка сериализации" << std::endl;
        return 1;
    }

    std::cout << "Успех! Данные успешно сериализованы в файл: " << config.outputFilename << std::endl;
    return 0;
}