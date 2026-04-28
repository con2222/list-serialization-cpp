#include "FileParser.hpp"

// std
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>

bool populate_list_from_file(List& list, std::unordered_map<ListNode*, int32_t>& randIndexMap, std::string_view filename) {
    std::ifstream file(filename.data());
    if (!file.is_open()) {
        std::cout << "Файл не найден. Проверьте его наличие в папке с программой" << std::endl;
        return false;
    }

    if (file.peek() == std::ifstream::traits_type::eof()) {
        std::cout << "Файл пуст" << std::endl;
        return false;
    }

    std::vector<ListNode*> nodes;
    std::vector<int32_t> randIndexes;
    int32_t counter = 0;
    std::string line;

    while (std::getline(file, line)) {
        int32_t pos = line.rfind(';');
        if (pos == -1) {
            std::cout << "Неправильный формат текста в файле, должен быть: <строка>;<индекс>\n";
            return false;
        }

        int32_t n;
        try {
            n = std::stoi(&line[pos + 1]);
        }
        catch (const std::invalid_argument& e) {
            std::cerr << "Ошибка: индекс введен неверно во входном файле" << std::endl;
            return false;
        } 
        catch (const std::out_of_range& e) {
            std::cerr << "Ошибка: число слишком большое для int32!" << std::endl;
            return false;
        }

        line.resize(pos);

        ListNode* newNode = list.push_back(std::move(line));
        randIndexMap.insert(std::make_pair(newNode, counter++));
        nodes.push_back(newNode);
        randIndexes.push_back(n);
    }
    
    list.add_links(nodes, randIndexes);
    return true;
}
